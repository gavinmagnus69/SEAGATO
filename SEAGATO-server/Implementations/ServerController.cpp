#include "ServerController.h"
#include "Track.h"
#include "User.h"
#include <grpcpp/security/server_credentials.h>
#include <memory>

int Controller::ServerController::start()
{
    std::cout << "start server\n";
    
    
    grpc::ServerBuilder builder;
    builder.AddListeningPort("localhost:1488", grpc::InsecureServerCredentials());
    builder.RegisterService(&this->AudioImpl);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
    
    std::cout << "server dropped\n";
}
//TODO
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
Entity::User Controller::ServerController::send_track(Entity::Track& track)
{
    return Entity::User();
}
