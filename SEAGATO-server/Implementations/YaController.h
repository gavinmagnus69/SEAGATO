#ifndef YACONTROLLER_H
#define YACONTROLLER_H

#include "../Interfaces/IClient.h"
#include "../Interfaces/IController.h"
#include <memory>



namespace Controller
{
    class YaController : public Interface::IController
    {
        private:
            //
            std::unique_ptr<Interface::IClient> _client;
            std::string _ip_port;
        public:
            //сюда засунуть реализацию IClient то есть Client из grpc.h
            YaController(Interface::IClient* cli ,std::string ip_port = "localhost:9999");    
        public:
            Entity::Track fetch_track(std::string song_id, Entity::User& usr) override;
            int connect(std::string ip_port, Entity::User& usr) override;
            int disconnect(Entity::User& usr) override;
            bool is_connected() override; 
    }; 

}


#endif