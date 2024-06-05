#include "ServerAuth.h"
#include "proto/auth.pb.h"

ServerAuth::ServerAuth(std::shared_ptr<grpc::Channel> channel) :
        _stub(AuthServices::NewStub(channel)) {}

int ServerAuth::addUser(const std::string& token){
    grpc::ClientContext context;
    ::addUserRequest request;
    request.set_token(token);
    ::addUserResponse response;

    ::grpc::Status status = _stub->addUser(&context, request, &response);

    if(status.ok()){
        std::cout << "response recieved\n";
        return response.code();
    }
    else{
        std::cout << "response did not recieved "+status.error_message()+" and "+std::to_string(status.error_code())+"\n";
        return -228;
    }

}

int ServerAuth::addUserData(const std::vector<std::string>& args){
    grpc::ClientContext context;
    ::addUsersDataRequest request;
    request.set_token(args[0]);
    request.set_ytoken(args[1]);
    request.set_chat_id(args[2]);

    ::addUsersDataResponse response;

    ::grpc::Status status = _stub->addData(&context, request, &response);

    if(status.ok()){
        std::cout << "response recieved\n";
        return response.code();
    }
    else{
        std::cout << "response did not recieved "+status.error_message()+" and "+std::to_string(status.error_code())+"\n";
        return -228;
    }
}