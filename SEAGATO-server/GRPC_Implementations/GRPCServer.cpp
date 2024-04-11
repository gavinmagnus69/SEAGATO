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

::grpc::Status GRPCServer::sendAudio(::grpc::ServerContext* context, const ::Request* request, ::Audio* response)
{
    std::cout << "requested\n";
    Entity::User usr;
    std::string str = request->song_id();
    Entity::Track trc = ya_controller->fetch_track(str, usr);
    response->set_data(trc.get_track_bytes(), trc.get_track_bytes_len());
    return grpc::Status::OK;
}


::grpc::Status GRPCServer::sendTrackList(::grpc::ServerContext* context, const ::Request_list* request, ::Tracks_list* response)
{

    std::cout << "list requested";
    response->add_tracks_names("amogus");
    response->add_tracks_names("amigo");
    response->add_tracks_names("balik");
    return grpc::Status::OK;
}


::grpc::Status GRPCServer::sendTracks(::grpc::ServerContext* context, const ::Tracks_list* request, ::Tracks* response)
{
    std::cout << "sendTracks called\n";
    Entity::User usr;
    for(auto track_name : request->tracks_names())
    {
        std::cout << track_name << '\n';
        Entity::Track trc = ya_controller->fetch_track(track_name, usr);
        //std::cout << trc.get_track_bytes_len() << '\n';
        ::Track tmp_track;
        tmp_track.set_track_name(trc.get_track_name());
        tmp_track.set_data(trc.get_track_bytes(), trc.get_track_bytes_len());
        tmp_track.set_song_id(trc.get_track_name());
        response->mutable_tracks()->Add(std::move(tmp_track));
    }

    return grpc::Status::OK;
}

  ::grpc::Status GRPCServer::sendTrackStream(::grpc::ServerContext* context, const ::Tracks_list* request, ::grpc::ServerWriter< ::Track>* writer)
  {
    std::cout << "sendTracksStream called\n";

    std::vector<::Track> tracks;
    Entity::User usr;

    for(const auto& track_name : request->tracks_names())
    {

        std::cout << track_name << "\n";
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