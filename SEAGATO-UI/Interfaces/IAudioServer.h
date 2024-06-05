#ifndef IAUDIOSERVER_H
#define IAUDIOSERVER_H

#include "proto/hello.pb.h"

class IAudioServer{
    public:
        virtual void getList(const std::string& token, std::vector<std::string>& tracks, bool tp) = 0;
        virtual void getTracksByBatch( const std::string& tkn, std::vector<std::string>& request, std::vector<std::string>& response, bool tp) = 0;
        virtual ~IAudioServer(){};
};


#endif