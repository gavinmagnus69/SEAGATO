#include <fstream>
#include <grpcpp/security/credentials.h>
#include <iostream>
#include "Interactor.h"
#include "YaController.h"
#include "ServerController.h"
#include "grpc.h"


int main()
{
    //test port c++ 9999
    //python port 2888
    //INIT ORDER: YACONTROLLER TGCONTROLLER SERVERCONTROLLER

    Service::Interactor server(new Controller::YaController(
        new Client(grpc::CreateChannel("localhost:2888", grpc::InsecureChannelCredentials()))),
        new Controller::YaController(
        new Client(grpc::CreateChannel("localhost:9998", grpc::InsecureChannelCredentials()))),
        new Controller::ServerController()
         );
    server.start();

    
    
    
    return 0;
}