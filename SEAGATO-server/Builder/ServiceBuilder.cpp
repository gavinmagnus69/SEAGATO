#include "Config.h"
#include "Interactor.h"
#include "ServiceBuilder.h"



ServiceBuilder::ServiceBuilder(const Config& cfg) : cfg(cfg){}


Service::Interactor ServiceBuilder::get_instance(){
    return Service::Interactor(cfg ,new Controller::YaController(
        new Client("localhost:" + cfg.port_yandex)),
        new Controller::YaController(
        new Client("localhost:" + cfg.port_tg)),
        new Controller::ServerController("localhost:" + cfg.port_audio, "localhost:" + cfg.port_auth),
        new Controller::DBController(cfg.storage_path)
         );
}