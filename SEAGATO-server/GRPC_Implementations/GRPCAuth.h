#ifndef GRPCAUTH_H
#define GRPCAUTH_H

#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include <memory>
#include "../Interfaces/IDBController.h"
#include "proto/auth.grpc.pb.h"
#include "proto/auth.pb.h"



class GRPCAuth : public AuthServices::Service{
    private:
        std::shared_ptr<Interface::IBDController> db_context;
    public:
        void set_DBController(std::shared_ptr<Interface::IBDController>);
    public:
        ::grpc::Status addUser(::grpc::ServerContext* context, const ::addUserRequest* request, ::addUserResponse* response) override;
        ::grpc::Status addData(::grpc::ServerContext* context, const ::addUsersDataRequest* request, ::addUsersDataResponse* response) override;

};

#endif