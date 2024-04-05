#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <memory>
#include <queue>
#include "../Interfaces/IController.h"



namespace Service
{    
    class Interactor
    {
        private:
            Entity::User current_user;
            std::queue<Entity::Track> current_tracks;
            
            //TODO write controller interface
        private:

            //std::shared_ptr<Interface::IController> dB_controller;
            //TODO make unique_ptrs
            std::shared_ptr<Interface::IController> yandex_controller;

            std::shared_ptr<Interface::IController> tg_controller;
        public:

            //REALIZATION of YaController and TgController
            Interactor(Interface::IController* ya_contrl, Interface::IController* tg_controller);
 
        public:
            int start();
            int stop();
            //returns an error code?
            int get_tracks_yandex();
            int get_tracks_tg();
            int send_tracks();
    };
}

#endif