#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <memory>
#include <queue>
#include "../Interfaces/IController.h"
#include "IServer.h"
#include "User.h"



namespace Service
{    
    class Interactor
    {
        private:
            Entity::User current_user;


            //TRACKS TO FILL?? чзх
            std::queue<Entity::Track> _current_tracks;

            //LIST OF USERS 
            std::queue<Entity::User> _request_queue; 
            
            //TODO write controller interface
        private:

            //std::shared_ptr<Interface::IController> dB_controller;
            //TODO make unique_ptrs???
            std::shared_ptr<Interface::IController> yandex_controller;

            std::shared_ptr<Interface::IController> tg_controller;

            std::shared_ptr<Interface::IServer> server_controller;

            //std::shared_ptr<>
            
        public:

            //REALIZATION of YaController and TgController
            Interactor(Interface::IController* ya_contrl, Interface::IController* tg_controller, Interface::IServer* server_controller);
 
        public:
            int start();
            int stop();
            //returns an error code?
            // int get_tracks_yandex();
            // int get_tracks_tg();
            // int send_tracks();
    };
}

#endif