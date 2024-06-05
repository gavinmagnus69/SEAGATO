#include <fstream>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <iostream>
#include "DBController.h"
#include "Interactor.h"
#include "YaController.h"
#include "ServerController.h"
#include "grpc.h"
#include "Config.h"
#include "ServiceBuilder.h"


int main()
{
    //test port c++ 9999
    //python port 2888
    //tg port 2889
    //server port audio 1488
    //server port auth 1489

    Config cfg("/home/roman/Desktop/project/SEAGATO/SEAGATO-server/config.yaml");

    ServiceBuilder builder(cfg);
    Service::Interactor server = builder.get_instance();
    server.start();

    return 0;
}