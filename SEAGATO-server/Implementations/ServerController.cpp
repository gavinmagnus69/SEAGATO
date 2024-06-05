#include "ServerController.h"
#include "Track.h"
#include "User.h"
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server_builder.h>
#include <memory>
#include <thread>



//starts AudioServer
void Controller::ServerController::AudioServ(){
    std::cout << "start server\n";
    

    grpc::ServerBuilder builder;
    builder.AddListeningPort(this->_adressAudio, grpc::InsecureServerCredentials());
    builder.RegisterService(&this->AudioImpl);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
    
    std::cout << "server dropped\n";
}


//starts AuthServer
void Controller::ServerController::AuthServ(){
    std::cout << "auth serv starts\n";
    grpc::ServerBuilder builder;
    builder.AddListeningPort(this->_adressAuther, grpc::InsecureServerCredentials());
    builder.RegisterService(&this->_autherHandler);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
    std::cout << "auth dropped\n";
}

//STARTS SERVER MAIN FUNCTION
int Controller::ServerController::start()
{
    std::thread th1([this](){ this->AudioServ();});
    std::thread th2([this](){ this->AuthServ();});

    th1.join();
    th2.join();
}
//TODO need?
int Controller::ServerController::stop()
{
    return 0;
}
//TODO
int Controller::ServerController::set_user(Entity::User& user)
{
    return 0;
}
void Controller::ServerController::set_tgcontroller(std::shared_ptr<Interface::IController> contrl)
{
    this->AudioImpl.set_tgcontroller(contrl);
}

void Controller::ServerController::set_ycontroller(std::shared_ptr<Interface::IController> contrl)
{
    this->AudioImpl.set_ycontroller(contrl);
}

//TODO заглушка (хз что с этим делать)
Entity::User Controller::ServerController::send_track(Entity::Track& track)
{
    return Entity::User();
}

void Controller::ServerController::set_dbcontext(std::shared_ptr<Interface::IBDController> cntrl){
    this->_autherHandler.set_DBController(cntrl);
    this->AudioImpl.set_DBController(cntrl);
}

Controller::ServerController::ServerController(const std::string& adrAud, const std::string& adrAut){
    this->_adressAudio = adrAud;
    this->_adressAuther = adrAut;
}