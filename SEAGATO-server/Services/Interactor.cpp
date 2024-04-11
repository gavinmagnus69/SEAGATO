#include "../Services/Interactor.h"


Service::Interactor::Interactor(Interface::IController* y_cntrl, Interface::IController* tg_cntrl, 
Interface::IServer* s_cntrl) :
yandex_controller(y_cntrl), tg_controller(tg_cntrl), server_controller(s_cntrl){}

int Service::Interactor::start()
{
    server_controller->set_ycontroller(yandex_controller);
    server_controller->start();
}

//TODO
int Service::Interactor::stop()
{
    return 0;
}
            