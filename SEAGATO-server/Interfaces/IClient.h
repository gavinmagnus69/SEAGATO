#ifndef ICLIENT_H
#define ICLIENT_H


#include <string>
#include "../Entities/User.h"
#include <vector>

namespace Interface {
    class IClient
    {
        //TODO virtual destructor
        public:
            virtual std::string sendData(std::string song_id, Entity::User& usr) = 0;
            virtual void getTracksByStream(const std::vector<std::string>& request, std::vector<std::string>& response) = 0;
            //virtual std::list<Entity::Track> 
    };
}


#endif