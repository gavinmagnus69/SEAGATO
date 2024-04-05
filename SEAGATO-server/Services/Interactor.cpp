#include "../Services/Interactor.h"


Service::Interactor::Interactor(Interface::IController* y_cntrl, Interface::IController* tg_cntrl) :
yandex_controller(y_cntrl), tg_controller(tg_cntrl){}

