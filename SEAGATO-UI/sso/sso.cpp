#include "sso.h"
#include <string>

ssoGRPC::ssoGRPC(std::shared_ptr<grpc::Channel> channel) : 
    _stub(auth::Auth::NewStub(channel)){}

ssoGRPC::~ssoGRPC(){};

std::variant<std::string, int> ssoGRPC::Login(const std::string& login, const std::string& pass){
    grpc::ClientContext context;
    auth::LoginRequest request;
    auth::LoginResponse resp;
    request.set_email(login);
    request.set_password(pass);
    request.set_app_id(default_app_id);

    ::grpc::Status status = _stub->Login(&context, request, &resp);
    std::variant<std::string, int> ans;
    //suc returns jwt-token
    if(status.ok()){
        std::cout << "succesfull login\n";
        ans = resp.token();
    }
    else{
        std::cout << "failed to log in\n";
        ans = 0;
    }

    return ans;
}
std::variant<std::string, int64_t> ssoGRPC::Register(const std::string& login, const std::string& password){
    grpc::ClientContext context;
    auth::RegisterRequest request;
    request.set_email(login);
    request.set_password(password);
    auth::RegisterResponse response;
    ::grpc::Status status = _stub->Register(&context, request, &response);

    std::variant<std::string, int64_t> ans;
    //int64_t reg suc returns user_id
    //string reg failed
    if(status.ok()){
        std::cout << "user registered\n";
        ans = response.user_id();  
    }
    else{
        ans = std::to_string(status.error_code()) + ' ' + status.error_message();       
    }
    return ans;
}