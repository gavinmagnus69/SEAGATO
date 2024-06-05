#include "grpc.h"
#include "audio.grpc.pb.h"
#include "audio.pb.h"
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/support/status.h>
#include <memory>
#include <string>


Client::Client(const std::string& adr) :
    _stub(AudioServices::NewStub(grpc::CreateChannel(adr, grpc::InsecureChannelCredentials()))){}

std::string Client::sendData(std::string song_id, Entity::User& usr)
{
    ::Request req;
    req.set_song_id(song_id);

    ::Audio response;
    
    grpc::ClientContext context;

    grpc::Status status = _stub->sendAudio(&context,
     req, &response);

     if(status.ok())
     {
        return response.data();
     }
     else
     {

        std::cout << "grpc.cpp; sendData(); error in grpc client\n";
        std::cout << status.error_code() << ":" << status.error_message() << std::endl;
        return "";
     }
}

 void Client::getTracksByStream(const std::string& token, std::vector<std::string>& request, std::vector<std::string>& response )
{
    ClientContext context;
    ::tracksRequest req;
    ::Batch batch;
    req.set_token(token);
    for(const auto& str : request)
    {
        //std::cout << str;
        req.add_tracks_names(str);
    }

    request.clear();
    std::unique_ptr<grpc::ClientReader<Batch>> reader(_stub->SendTracksButched(&context, req));
    std::string tmp = "";    
    while (reader->Read(&batch)) 
    {
        if (batch.code() == 0){
                response.emplace_back(batch.data());
                request.push_back(batch.track_name());
        }
        if (batch.code() == 1){
            tmp += std::string(batch.data());
        }
        if (batch.code() == 2){
            response.push_back(tmp);
            request.push_back(batch.track_name());
            tmp = "";
        }
    }   

    Status status = reader->Finish();
    if(status.ok())
    {
        std::cout << "everything is ok\n";
    }
    else
    {
        std::cout << "grpc.cpp; getTracksByStream; something went wrong; "+std::to_string(status.error_code())+"; "+status.error_message()+" \n";
    } 

}

int Client::getList(const std::string& token, std::vector<std::string>& tracks){
    ClientContext context;
    ::tracksListRequest request;
    ::Tracks_list response;

    request.set_token(token);

    grpc::Status status = _stub->sendTrackList(&context, request, &response);
    int ans = 0;
    if(status.ok()){
        for(const auto& track : response.tracks_names()){
            tracks.push_back(track);
        }
        ans = 0;
    }
    else{
        std::cout << "grpc.cpp; getList(); -1 code; "+std::to_string(status.error_code())+";"+status.error_message()+"\n";
        ans = -1;
    }
    return ans;

}        