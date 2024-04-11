#ifndef GRPCSERVER_H
#define GRPCSERVER_H

#include "../Interfaces/IController.h"
#include <grpcpp/grpcpp.h>
#include "proto/audio.grpc.pb.h"
#include "proto/audio.pb.h"
#include <memory>



class GRPCServer : public AudioServices::Service
{
    private:
        std::shared_ptr<Interface::IController> ya_controller;
        std::shared_ptr<Interface::IController> tg_controller;
    public:
        void set_ycontroller(std::shared_ptr<Interface::IController> ycntrl);
        void set_tgcontroller(std::shared_ptr<Interface::IController> tgcntrl);
        ::grpc::Status sendAudio(::grpc::ServerContext* context, const ::Request* request, ::Audio* response);
        ::grpc::Status sendTrackList(::grpc::ServerContext* context, const ::Request_list* request, ::Tracks_list* response);
        ::grpc::Status sendTracks(::grpc::ServerContext* context, const ::Tracks_list* request, ::Tracks* response);
}; 




#endif