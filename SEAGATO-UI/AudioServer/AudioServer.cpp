#include "AudioServer.h"
#include <grpcpp/support/status.h>
#include <grpcpp/support/sync_stream.h>

AudioServer::AudioServer(std::shared_ptr<grpc::Channel> channel) :
        _stub(AudioServices::NewStub(channel)) {}


void AudioServer::getList(const std::string& token, std::vector<std::string>& tracks, bool tp){
    ::tracksListRequest req;
    req.set_type(tp);
    req.set_token(token);
    ::Tracks_list res;
    ::grpc::ClientContext context;
    ::grpc::Status status = _stub->sendTrackList(&context, req, &res);
    if(status.ok()){
        //std::cout << response.data();
        for(const auto& str : res.tracks_names())
        {
            tracks.push_back(str);
        }
    }
    else{
        std::cout << status.error_code() << ":" << status.error_message() << std::endl;
    }
};
void AudioServer::getTracksByBatch( const std::string& tkn, std::vector<std::string>& request, std::vector<std::string>& response, bool tp) {

    //std::cout << "getTracksByBatch called\n";
    grpc::ClientContext context;
    ::tracksRequest req;
    req.set_token(tkn);

    ::Batch batch;

    req.set_type(tp);
    for(const auto& str : request)
    {
        req.add_tracks_names(str);
    }

    std::unique_ptr<grpc::ClientReader<Batch>> reader(
    _stub->SendTracksButched(&context, req));

    request.clear();
    std::string tmp = "";    
    int counter = 0;
    while (reader->Read(&batch)) 
    {
        if (batch.code() == 0){
            //std::cout << "batch code 0\n";
                response.emplace_back(batch.data());
                request.push_back(batch.track_name());
                tmp = "";
        }
        if (batch.code() == 1){
            //std::cout << "batch code 1\n";
            tmp += std::string(batch.data());
            std::string tps = std::string(batch.data());
            //to_bin("batches"+std::to_string(counter)+"", tps.data(), tps.size());
            counter++;
            
        }
        if (batch.code() == 2){
            //std::cout << "batch code 2\n";
            response.push_back(tmp);
            request.push_back(batch.track_name());
            tmp = "";
        }
        if(batch.code() == -1){
            std::cout << "class client; getTracksByBatch; error happend: -1 code\n";
        }
    }   

    grpc::Status status = reader->Finish();

    if(status.ok())
    {
        std::cout << "everything is ok\n";
    }
    else
    {
        std::cout << status.error_code() << ":" << status.error_message() << std::endl;
        std::cout << "something went wrong\n";
    } 
};
AudioServer::~AudioServer(){}; 