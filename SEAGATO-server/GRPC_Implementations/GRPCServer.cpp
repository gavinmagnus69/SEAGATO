
#include "GRPCServer.h"
#include "tools.h"


#include <grpcpp/support/status.h>
#include <string>
#include <vector>

void GRPCServer::set_ycontroller(std::shared_ptr<Interface::IController> ycntrl)
{
    ya_controller = ycntrl;

}

void GRPCServer::set_tgcontroller(std::shared_ptr<Interface::IController> ycntrl)
{
    tg_controller = ycntrl;
}




//SEND SINGLE TRACK TO CLIENT
::grpc::Status GRPCServer::sendAudio(::grpc::ServerContext* context, const ::Request* request, ::Audio* response)
{
    std::cout << "requested single track\n";
    Entity::User usr;
    std::string str = request->song_id();
    Entity::Track trc = ya_controller->fetch_track(str, usr);
    response->set_data(trc.get_track_bytes());
    return grpc::Status::OK;
}




//TODO стоит заглушка (стоит задуматься)
::grpc::Status GRPCServer::sendTrackList(::grpc::ServerContext* context, const ::tracksListRequest* request, ::Tracks_list* response)
{
    std::cout << "trackslist handeler\n";
    std::string login = tools::get_login(request->token());

    std::vector<std::string> tracks_names;

    int code = -1;
    //TYPE TRUE: yandex
    //TYPE FALSE: telegramm
    if(request->type()){
        std::string token = this->db_context->get_token(login);
        if(token == "nil"){
            std::cout << "error in db\n";
            response->add_tracks_names("");
            return grpc::Status::OK;
        }
        code = ya_controller->getTracksNames(token, tracks_names);
    }
    else{
        std::string chat_id = this->db_context->get_chatid(login);
        if(chat_id == "nil"){
            std::cout << "error in db\n";
            response->add_tracks_names("");
            return grpc::Status::OK;
        }
        code = tg_controller->getTracksNames(chat_id, tracks_names);
    }

    if(code == -1){
        std::cout << "grpcServer.cpp; sendTrackList(); err occured\n";
        return grpc::Status::OK;
    }
    for (const auto& track : tracks_names){
        response->add_tracks_names(track);
    }
    return grpc::Status::OK;
}


   ::grpc::Status GRPCServer::SendTracksButched(::grpc::ServerContext* context, const ::tracksRequest* request, ::grpc::ServerWriter< ::Batch>* writer)
   {
        //std::cout << "SendTracksButched called\n";

        std::vector<std::string> track_names;
        for(const std::string& tr : request->tracks_names())
        {
            track_names.push_back(tr);
        }

        //get tracks from yandex-client
        std::string login = tools::get_login(request->token());
       
        //TYPE TRUE: yandex
        //TYPE FALSE: telegramm
        std::vector<Entity::Track> tracks;

        if(request->type()){
            std::string ytoken = db_context->get_token(login);
            tracks = ya_controller->fetch_tracks_batches(ytoken, track_names);
        }
        else if (!request->type()){
            //std::cout << "fetch tg\n";
             std::string chat_id = db_context->get_chatid(login);
            tracks = tg_controller->fetch_tracks_batches(chat_id, track_names);
        }    
        for(auto& ent : tracks)
        {          
            //std::cout << std::format("track name = {0}, bytes size = {1}\n", ent.get_track_name(), ent.get_track_bytes().length());
            ::Batch batch_to_send;

            //::Batch batch_to_send;
            if (ent.data().length() > 4000000)
            {
                //code 1: send batch
                //code 2: signal of ending
                //code 2 that is all
                //code 0 is full track that fills in 4000000 bytes
                std::string tmp;
                int cnt = 0;
                for(int i = 0; i < ent.data().length(); ++i, ++cnt)
                {

                    tmp += ent.get_track_bytes()[i];
                    if(cnt == 4000000)
                    {
                       
                        batch_to_send.set_track_name(ent.get_track_name());
                        batch_to_send.set_code(1);
                        batch_to_send.set_data(tmp);
                        if (writer->Write(batch_to_send))
                        {
                            //std::cout << "batch code 1 sended\n";
                        } else
                        {
                            batch_to_send.set_code(-1);
                            //std::cout << "batch code 1 not sended\n";
                        }
                        tmp = "";
                        cnt = 0;
                    }
                }
                //send what is left
                if(cnt != 0)
                {
                   
                    batch_to_send.set_track_name(ent.get_track_name());
                    batch_to_send.set_code(1);
                    batch_to_send.set_data(tmp);
                    if (writer->Write(batch_to_send))
                    {
                        //std::cout << "batch code 1 left sended\n";
                    } else
                    {
                        batch_to_send.set_code(-1);
                         //std::cout << "batch code 1 left not sended\n";
                    }
                    tmp = "";
                }
                
                //send code 2 that is all
                batch_to_send.set_track_name(ent.get_track_name());
                batch_to_send.set_code(2);
                batch_to_send.set_data("amogus");
                if (writer->Write(batch_to_send))
                {
                    //std::cout << "batch code 2 sended\n";
                } else
                {
                    batch_to_send.set_code(-1);
                     //std::cout << "batch code 2 not sended\n";    
                }
            }
            else
            {

                //code 0 is full track that fills in 4000000 bytes
                
                batch_to_send.set_track_name(ent.get_track_name());
                batch_to_send.set_code(0);
                batch_to_send.set_data(ent.get_track_bytes());
                if (writer->Write(batch_to_send))
                {
                    //std::cout << "batch code 0 sended\n";
                }
                else
                {
                    batch_to_send.set_code(-1);
                    //std::cout << "batch code 0 not sended\n";
                }
            }
        }

        return grpc::Status::OK;

   }


   void GRPCServer::set_DBController(std::shared_ptr<Interface::IBDController> ctx){
        this->db_context = ctx;
   }

