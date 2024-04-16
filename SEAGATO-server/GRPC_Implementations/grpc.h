#ifndef GRPC_H
#define GRPC_H

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/async_stream.h>
#include <memory>
#include "audio.grpc.pb.h"
#include "audio.pb.h"
#include "../Interfaces/IClient.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;


// CLASS NAME????????????
class Client : public Interface::IClient 
{
    private:
        std::unique_ptr<AudioServices::Stub> _stub;
    public:
        Client(std::shared_ptr<Channel>);
    public:
        std::string sendData(std::string song_id, Entity::User& usr) override;


        //batches!!!!!!!!!
        void getTracksByStream(const std::vector<std::string>& request, std::vector<std::string>& response) override;  
};



#endif