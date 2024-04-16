#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include "IServer.h"
#include <grpcpp/server_builder.h>
#include <memory>
#include "GRPCServer.h"
#include "User.h"

namespace Controller {
//do not touch 
//call () set_ycontroller() start()
class ServerController : public Interface::IServer
{
    private:
        GRPCServer AudioImpl;
        //
    public:

        int start() override;
        int stop() override;
        int set_user(Entity::User& user) override;
        void set_tgcontroller(std::shared_ptr<Interface::IController> contrl) override;
        void set_ycontroller(std::shared_ptr<Interface::IController> contrl) override;
        Entity::User send_track(Entity::Track& track) override;
};

}

#endif