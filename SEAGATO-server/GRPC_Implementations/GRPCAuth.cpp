#include "GRPCAuth.h"
#include <grpcpp/support/status.h>
#include "../tools/tools.h"

//code 0 - OK
//code -1 - NOT OK

::grpc::Status GRPCAuth::addUser(::grpc::ServerContext* context, const ::addUserRequest* request, ::addUserResponse* response){
    std::cout << "auth handler\n";
    std::string token = request->token();
    std::string login = tools::get_login(token);
    if(login == ""){
        response->set_code(-4);
        return grpc::Status::OK;
    }
    std::cout << login << '\n';
    int code = this->db_context->add({login, "nil", "nil"});

    std::cout << "operation code = " << code << '\n';
    if(code == 0){
        response->set_code(0);
    }
    else if(code == -3){
        response->set_code(-3);
    }
    else {
        std::cout << "grpcauth.cpp; addUser(); -1 code";
        response->set_code(-1);
    }
    return grpc::Status::OK;
}

void GRPCAuth::set_DBController(std::shared_ptr<Interface::IBDController> ctx){
    this->db_context = ctx;
}

 ::grpc::Status GRPCAuth::addData(::grpc::ServerContext* context, const ::addUsersDataRequest* request, ::addUsersDataResponse* response){
    
    std::string chat_id = request->chat_id();
    std::string ytoken = request->ytoken();
    std::string login = tools::get_login(request->token());

    int code = this->db_context->update({login, chat_id, ytoken});

    if(code == 0){
        response->set_code(0);
        std::cout << "suc update\n";
    }
    else{
        response->set_code(-1);
        std::cout << "grpcauth.cpp; addData(); -1 code";
        std::cout << "sucked err\n";
    }

    return grpc::Status::OK;

 }