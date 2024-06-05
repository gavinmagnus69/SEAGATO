#include "../Services/Interactor.h"
#include "IDBController.h"


Service::Interactor::Interactor(const Config& cfg, Interface::IController* y_cntrl, Interface::IController* tg_cntrl, 
Interface::IServer* s_cntrl, Interface::IBDController* db) :
yandex_controller(y_cntrl), tg_controller(tg_cntrl), server_controller(s_cntrl), db_context(db){
    
}

int Service::Interactor::start()
{
    server_controller->set_ycontroller(yandex_controller);
    server_controller->set_tgcontroller(tg_controller);
    server_controller->set_dbcontext(db_context);
    server_controller->start();
}

//TODO
int Service::Interactor::stop()
{
    return 0;
}
            