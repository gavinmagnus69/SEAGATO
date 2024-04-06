#ifndef ISERVER_H
#define ISERVER_H

#include "../Entities/User.h"
#include "../Entities/Track.h"
#include "IController.h"
#include <memory>

namespace Interface
{
    class IServer
    {
        public:
            virtual int start() = 0;
            virtual int stop() = 0;
            virtual int set_user(Entity::User& user) = 0;
            virtual void set_tgcontroller(std::shared_ptr<Interface::IController> contrl) = 0;
            virtual void set_ycontroller(std::shared_ptr<Interface::IController> contrl) = 0;
            virtual Entity::User send_track(Entity::Track& track) = 0;
    };

}


#endif