#ifndef SERVERAUTH_H
#define SERVERAUTH_H
#include "../Interfaces/IServerAuth.h"
#include <memory>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include "proto/auth.pb.h"
#include "proto/auth.grpc.pb.h"

class ServerAuth : public IServerAuth{
    private:
        std::unique_ptr<AuthServices::Stub> _stub;
    public:
        ServerAuth(std::shared_ptr<grpc::Channel> channel);
    public:
        int addUser(const std::string&) override;
        int addUserData(const std::vector<std::string>&) override;
        ~ServerAuth(){};
};
#endif