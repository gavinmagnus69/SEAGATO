#ifndef GRPCSERVER_H
#define GRPCSERVER_H

#include "../Interfaces/IController.h"
#include <grpcpp/grpcpp.h>
#include "proto/audio.grpc.pb.h"
#include "proto/audio.pb.h"
#include <memory>
#include "Track.h"
#include "User.h"
#include <grpcpp/support/status.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <format>
#include "IDBController.h"
#include "../tools/tools.h"


class GRPCServer : public AudioServices::Service
{
    private:
        std::shared_ptr<Interface::IController> ya_controller;
        std::shared_ptr<Interface::IController> tg_controller;
        std::shared_ptr<Interface::IBDController> db_context;

    public:
        void set_ycontroller(std::shared_ptr<Interface::IController> ycntrl);
        void set_tgcontroller(std::shared_ptr<Interface::IController> tgcntrl);
        void set_DBController(std::shared_ptr<Interface::IBDController>);
    public:
        ::grpc::Status sendAudio(::grpc::ServerContext* context, const ::Request* request, ::Audio* response) override;
        ::grpc::Status sendTrackList(::grpc::ServerContext* context, const ::tracksListRequest* request, ::Tracks_list* response) override;
        ::grpc::Status SendTracksButched(::grpc::ServerContext* context, const ::tracksRequest* request, ::grpc::ServerWriter< ::Batch>* writer) override;

}; 




#endif