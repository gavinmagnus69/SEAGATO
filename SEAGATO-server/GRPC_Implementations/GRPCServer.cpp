#include "GRPCServer.h"
#include "Track.h"
#include "User.h"
#include "audio.pb.h"
#include <grpcpp/support/status.h>
#include <string>
#include <vector>
#include <fstream>

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
    std::cout << "requested\n";
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
        std::vector<Entity::Track> tracks = ya_controller->fetch_tracks_batches(track_names);





        return grpc::Status::OK;
    

   }