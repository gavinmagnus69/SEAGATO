#include "GRPCServer.h"
#include "Track.h"
#include "User.h"
#include <grpcpp/support/status.h>


void GRPCServer::set_ycontroller(std::shared_ptr<Interface::IController> ycntrl)
{
    ya_controller = ycntrl;
}

void GRPCServer::set_tgcontroller(std::shared_ptr<Interface::IController> ycntrl)
{
    tg_controller = ycntrl;
}

::grpc::Status GRPCServer::sendAudio(::grpc::ServerContext* context, const ::Request* request, ::Audio* response)
{

    std::cout << "requested\n";
    Entity::User usr;
    Entity::Track trc = ya_controller->fetch_track("228", usr);
    response->set_data(trc.get_track_bytes(), trc.get_track_bytes_len());
    return grpc::Status::OK;

}
