#include <fstream>
#include <grpcpp/security/credentials.h>
#include <iostream>
#include "Interactor.h"
#include "YaController.h"
#include "ServerController.h"
#include "grpc.h"

void to_bin(Entity::Track& ent)
{
    std::ofstream output_file("1.mp3", std::ios::binary);
    output_file.write(ent.get_track_bytes(), ent.get_track_bytes_len());
    output_file.close();
}


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