#ifndef SSO_H
#define SSO_H

#include "../Interfaces/Isso.h"
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>

#include "proto/sso.grpc.pb.h"
#include "proto/sso.pb.h"

class ssoGRPC : public Isso
{
    private:
        std::unique_ptr<auth::Auth::Stub> _stub;
        int default_app_id = 1;

    public:
        ssoGRPC(std::shared_ptr<grpc::Channel> channel);
    public:

        std::variant<std::string, int> Login(const std::string&, const std::string&) override;
        std::variant<std::string, int64_t> Register(const std::string&, const std::string&) override;
        ~ssoGRPC() override;
};





#endif