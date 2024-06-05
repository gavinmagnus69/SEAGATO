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
            //GRPC CLIENT REALIZATION
            std::unique_ptr<Interface::IClient> _client;
            std::string _ip_port;
        public:
            //сюда засунуть реализацию IClient т.е. Client из grpc.h
            YaController(Interface::IClient* cli ,std::string ip_port = "localhost:9999");    
        public:
            Entity::Track fetch_track(std::string song_id, Entity::User& usr) override;
            std::vector<Entity::Track> fetch_tracks_batches(const std::string& token, const std::vector<std::string>& track_names) override;
            int getTracksNames(const std::string&, std::vector<std::string>&) override;
            
            int connect(std::string ip_port, Entity::User& usr) override;
            int disconnect(Entity::User& usr) override;
            bool is_connected() override;

            //TEST
            //std::list<Entity::Track> fetch_tracks(std::list<std::string>& song_names, Entity::User& cur_usr) override;  
    }; 

}


#endif