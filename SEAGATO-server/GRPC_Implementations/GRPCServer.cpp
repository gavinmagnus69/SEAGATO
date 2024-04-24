#include "GRPCServer.h"
#include "Track.h"
#include "User.h"
#include "audio.pb.h"
#include <grpcpp/support/status.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <format>

//TOOLS////////////////////////////////////
void to_bin(const std::string& tr_name, char* bytes, int sz)
{

    std::ofstream output_file(tr_name + ".mp3", std::ios::binary);
    output_file.write(bytes, sz);
    output_file.close();

}
void vector_to_bin(std::vector<std::string> &sngs,
                   std::vector<std::string> &bytes_tracks) {
    int i = 0;
    for (auto it = bytes_tracks.begin(); it != bytes_tracks.end(); ++it) {
        to_bin(sngs[i], it->data(), it->length());
        ++i;
    }
}
///////////////////////////////////////////


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
::grpc::Status GRPCServer::sendTrackList(::grpc::ServerContext* context, const ::Request_list* request, ::Tracks_list* response)
{

    std::cout << "list requested";
    response->add_tracks_names("amogus");
    response->add_tracks_names("amigo");
    response->add_tracks_names("balik");
    return grpc::Status::OK;
}



//THIS FUNCTION SENDS TRACKS FROM SERVICES TO CLIENT
//OUTDATED!!!!!!!!

  ::grpc::Status GRPCServer::sendTrackStream(::grpc::ServerContext* context, const ::Tracks_list* request, ::grpc::ServerWriter< ::Track>* writer)
  {
    //std::cout << "sendTracksStream called\n";

    std::vector<::Track> tracks;
    Entity::User usr;

    for(const auto& track_name : request->tracks_names())
    {

        //std::cout << track_name << "\n";
        Entity::Track trc = ya_controller->fetch_track(track_name, usr);
        ::Track proto_track;
        proto_track.set_track_name(trc.get_track_name());
        proto_track.set_data(trc.get_track_bytes());
        proto_track.set_song_id(trc.get_track_name());
        if(writer->Write(proto_track))
        {
            std::cout << "sended\n";
        }
        else
        {
            std::cout << "not sended\n";
        }
    }
    return grpc::Status::OK;
  }




   ::grpc::Status GRPCServer::SendTracksButched(::grpc::ServerContext* context, const ::Tracks_list* request, ::grpc::ServerWriter< ::Batch>* writer)
   {
        std::vector<std::string> track_names;
        for(const std::string& tr : request->tracks_names())
        {
            track_names.push_back(tr);
        }

        //get tracks from yandex-client
        std::vector<Entity::Track> tracks = ya_controller->fetch_tracks_batches(track_names);
       
        for(auto& ent : tracks)
        {          
            std::cout << std::format("track name = {0}, bytes size = {1}\n", ent.get_track_name(), ent.get_track_bytes().length());
            ::Batch batch_to_send;

            //::Batch batch_to_send;
            if (ent.data().length() > 4000000)
            {
                //code 1: send batch
                //code 2: signal of ending
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
                            std::cout << "batch code 1 sended\n";
                        } else
                        {
                             std::cout << "batch code 1 not sended\n";
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
                        std::cout << "batch code 1 left sended\n";
                    } else
                    {
                         std::cout << "batch code 1 left not sended\n";
                    }
                    tmp = "";
                }
                
                //send code 2 that is all
                batch_to_send.set_track_name(ent.get_track_name());
                batch_to_send.set_code(2);
                batch_to_send.set_data("amogus");
                if (writer->Write(batch_to_send))
                {
                    std::cout << "batch code 2 sended\n";
                } else
                {
                     std::cout << "batch code 2 not sended\n";    
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
                    std::cout << "batch code 0 sended\n";
                }
                else
                {
                    std::cout << "batch code 0 not sended\n";
                }
            }
        }

        return grpc::Status::OK;

   }

