#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QDebug>
#include <memory>
#include <qthread.h>
#include <string>
#include <fstream>
#include "../Interfaces/IAudioServer.h"
#include "../AudioServer/AudioServer.h"




class Worker : public QObject {
    Q_OBJECT

public:
    explicit Worker(QObject *parent = nullptr) : QObject(parent) {
        audio = std::make_shared<AudioServer>(grpc::CreateChannel("localhost:1488", grpc::InsecureChannelCredentials()));
    }
private:
    std::shared_ptr<IAudioServer> audio;
    int id = 0;
    std::vector<std::string> bytes;
    std::vector<std::string> tracks_local;
    std::string jwt;
    bool tp;

public:
    void to_bin2(const std::string& name, char* bytes, int sz)
    {
        
        std::ofstream output_file("./Music/" + name + ".mp3", std::ios::binary);
        output_file.write(bytes, sz);
        output_file.close();

    }


    void vector_to_bin2(std::vector<std::string> &sngs,
                    std::vector<std::string> &bytes_tracks) {
        int i = 0;
        for (auto it = bytes_tracks.begin(); it != bytes_tracks.end(); ++it) {
            to_bin2(sngs[i], it->data(), it->length());
            ++i;
        }
    }
    void set_id(int id){
        this->id = id;
    }
    void set_args(const std::string& jwt, const std::vector<std::string>& tracks_local, bool tp){
        this->tracks_local.clear();
        this->tracks_local = tracks_local;
        this->jwt = jwt;
        this->tp = tp;
    }    
public slots:
    void doWork() {
        // Simulate a time-consuming task
        bytes.clear();
        
        audio->getTracksByBatch(jwt, tracks_local, bytes, tp);
        vector_to_bin2(tracks_local, bytes);
        emit workFinished();
    }



signals:
    void workFinished();
};

#endif // WORKER_H