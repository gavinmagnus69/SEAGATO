#ifndef ICLIENT_H
#define ICLIENT_H


#include <string>
#include "../Entities/User.h"
#include <list>

namespace Interface {
    class IClient
    {
        //TODO virtual destructor
        public:
            virtual std::string sendData(std::string song_id, Entity::User& usr) = 0;
            //virtual std::list<Entity::Track> 
    };
}


#endif