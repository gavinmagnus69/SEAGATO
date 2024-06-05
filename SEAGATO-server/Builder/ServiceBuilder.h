#ifndef SERVICEBUILDER_H
#define SERVICEBUILDER_H

#include "Interactor.h"
#include "Config.h"
#include "ServerController.h"
#include "DBController.h"
#include "YaController.h"
#include "grpc.h"

class ServiceBuilder{
    private:
        Config cfg;
    public: 
        ServiceBuilder(const Config&);
    public:    
        Service::Interactor get_instance();
        
};



#endif