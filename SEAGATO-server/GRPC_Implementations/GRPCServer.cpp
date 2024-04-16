#include "GRPCServer.h"
#include "Track.h"
#include "User.h"
#include "audio.pb.h"
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
    std::cout << "requested\n";
    Entity::User usr;
    std::string str = request->song_id();
    Entity::Track trc = ya_controller->fetch_track(str, usr);
    response->set_data(trc.get_track_bytes(), trc.get_track_bytes_len());
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
        proto_track.set_data(trc.get_track_bytes(), trc.get_track_bytes_len());
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