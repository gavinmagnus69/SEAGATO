#ifndef AUDIOSERVER_H
#define AUDIOSERVER_H

#include "../Interfaces/IAudioServer.h"
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include "proto/hello.pb.h"
#include "proto/hello.grpc.pb.h"

class AudioServer : public IAudioServer{
    private:
        std::unique_ptr<AudioServices::Stub> _stub;
    public:
        AudioServer(std::shared_ptr<grpc::Channel> channel);
    public:
        void getList(const std::string& token, std::vector<std::string>& tracks, bool tp) override;
        void getTracksByBatch( const std::string& tkn, std::vector<std::string>& request, std::vector<std::string>& response, bool tp) override;
        ~AudioServer() override;       
};


#endif