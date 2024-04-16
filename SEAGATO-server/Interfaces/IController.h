#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "../Entities/Track.h"
#include "../Entities/User.h"
#include <list>
#include <string>
#include <vector>
namespace Interface
{
    class IController
    {   
        public:
            //TODO virtual destructor  
            //TODO
            //PROBABLY CHANGE RETURN VAL TO PTR
            virtual Entity::Track fetch_track(std::string song_id, Entity::User& cur_usr) = 0;

            virtual std::vector<Entity::Track> fetch_tracks_batches(const std::vector<std::string>& track_names) = 0; 
              
            virtual int connect(std::string ip_port, Entity::User& cur_usr) = 0;
            virtual int disconnect(Entity::User& cur_usr) = 0;
            virtual bool is_connected() = 0; 
            
            
            //TEST
            //virtual std::list<Entity::Track> fetch_tracks(std::list<std::string>& song_names, Entity::User& cur_usr) = 0;
    };
}

#endif