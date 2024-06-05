//
// Created by roman on 5/20/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_player.h" resolved

#include "player.h"
#include "Streamer.h"
#include "Worker.h"
#include "ui_player.h"
#include "login.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include <future>
#include <qbrush.h>
#include <qbuffer.h>
#include <qchar.h>
#include <qcolor.h>
#include <qdebug.h>
#include <qglobal.h>
#include <qicon.h>
#include <qlineedit.h>
#include <qlistwidget.h>
#include <qnamespace.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qregion.h>
#include <qstringliteral.h>
#include <qstyle.h>
#include <qt6/QtCore/qlogging.h>
#include <qt6/QtCore/qobject.h>
#include <qt6/QtGui/qmovie.h>
#include <qthread.h>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <vector>
#include "../ServerAuth/ServerAuth.h"
#include "../AudioServer/AudioServer.h"
#include <fstream>
#include <filesystem>
#include <QtConcurrent/QtConcurrent>
#include <random>

void to_bin(const std::string& name, char* bytes, int sz)
{

    std::ofstream output_file("./Music/" + name + ".mp3", std::ios::binary);
    output_file.write(bytes, sz);
    output_file.close();

}


void vector_to_bin(std::vector<std::string> &sngs,
                   std::vector<std::string> &bytes_tracks) {
    int i = 0;
    for (auto it = bytes_tracks.begin(); it != bytes_tracks.end(); ++it) {
        to_bin(sngs[i], it->data(), it->length());
        ++i;
    }
}


int getRandomInt(int l, int r){
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> distr(l, r);
    return distr(gen);
}

void generateRandomSequence(std::queue<int>& sequence, int n){
    std::set<int> vals;
    for(int i = 0; i < n; ++i){
        while (true) {
            int tmp = getRandomInt(0, n);
            if(!vals.contains(tmp)){
                //qDebug() << tmp << ' ';
                vals.insert(tmp);
                sequence.push(tmp);
                break;
            }
        }
    }
}

//CONSTRUCTOR
player::player(const QString& str, QWidget *parent) :
        QMainWindow(parent), ui(new Ui::player),
         auth(new ServerAuth(grpc::CreateChannel("localhost:1489", grpc::InsecureChannelCredentials()))),
         audio(new AudioServer(grpc::CreateChannel("localhost:1488", grpc::InsecureChannelCredentials())))
{
        ui->setupUi(this);

        this->jwt_token = str;
        
        int w = 1600;
        int h = 900;

        this->setFixedSize(w, h);
        

        playing = false;
        is_muted = false;
        repeat = false;
        shuffle = false;
        MPlayer = new QMediaPlayer();
        movik = new QMovie("../anime1.gif");
        ui->labelAnime->setMovie(movik);
        movik->start();
        ui->labelAnime->hide();
        

        icon.addPixmap(QPixmap("../repeat-icon-512x430-89aic2ws.png"), QIcon::Normal, QIcon::Off);
        icon2.addPixmap(QPixmap("../shuffle.png"), QIcon::Normal, QIcon::Off);
        icon3.addPixmap(QPixmap("../repeat.png"), QIcon::Normal, QIcon::Off);        
        icon4.addPixmap(QPixmap("../shuffle-false.png"), QIcon::Normal, QIcon::Off);


        
        ui->pushButton_Play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        ui->pushButton_Seek_Back->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
        ui->pushButton_Seek_Forward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

        ui->pushButton_Seek_Repeat->setIcon(icon);
        ui->pushButton_Seek_Shuffle->setIcon(icon4);

        connect(this->ui->pushButton, &QPushButton::clicked, this, &player::remove_clicked);

        connect(this->ui->pushButton_Seek_Back, &QPushButton::clicked, this, &player::back_clicked);
        connect(this->ui->pushButton_Seek_Forward, &QPushButton::clicked, this, &player::forward_clicked);

        connect(this->ui->pushButton_Play, &QPushButton::clicked, this, &player::play_clicked);
        connect(this->ui->pushButton_Volume, &QPushButton::clicked, this, &player::muted_clicked);

        connect(this->ui->actionOpen_Audio_FIle, &QAction::triggered, this, &player::on_actionOpen_Audio_File_triggered);

        connect(this->ui->horizontalSlider_Audio_File_Duration, &QSlider::sliderReleased, this, &player::duration_valueChanged);
        connect(this->ui->horizontalSlider_Audio_Volume, &QSlider::valueChanged, this, &player::volume_valueChanged);
        connect(MPlayer, &QMediaPlayer::durationChanged, this, &player::duration_changed);
        connect(MPlayer, &QMediaPlayer::positionChanged, this, &player::positin_changed);

        connect(this->ui->pushButton_setYToken, &QPushButton::clicked, this, &player::ytoken_clicked);
        connect(this->ui->pushButton_setChatId, &QPushButton::clicked, this, &player::chatid_clicked);

        connect(this->ui->listWidget_Tracks, &QListWidget::itemDoubleClicked, this, &player::selection_clicked);

        connect(this->ui->pushButton_refresh, &QPushButton::clicked, this, &player::refresh_clicked);
        connect(this->ui->lineEdit_search, &QLineEdit::textEdited, this, &player::refresh_clicked1);

        connect(this->ui->pushButton_Seek_Repeat, &QPushButton::clicked, this, &player::repeat_clicked);
        connect(this->ui->pushButton_Seek_Shuffle, &QPushButton::clicked, this, &player::shuffle_clicked);


        ui->horizontalSlider_Audio_Volume->setMaximum(100);
        ui->horizontalSlider_Audio_Volume->setMinimum(0);
        ui->horizontalSlider_Audio_Volume->setValue(30);



        MPlayer->setVolume(ui->horizontalSlider_Audio_Volume->value());

        ui->horizontalSlider_Audio_File_Duration->setRange(0, MPlayer->duration() / 1000);

        std::filesystem::path dir = "./Music";
        if(!std::filesystem::exists(dir)){
            std::filesystem::create_directory(dir);
        }

        for (const auto& entry : std::filesystem::recursive_directory_iterator(dir))
        {
            if (entry.is_regular_file())
            {
                std::string ext = entry.path().extension().string(); 
                if( ext != ".mp3" && ext != ".waw" && ext != ".aac" && ext != ".ogg"){
                    continue;
                }

               tracks.insert(entry.path().filename().stem().string());
               ui->listWidget_Tracks->addItem(QString::fromStdString(entry.path().filename().stem().string()));
            }
        }

        arr = new QByteArray();
        buf = new QBuffer();
        streamer = new Streamer();



        //EXPEREMENTAL
        workerThread = new QThread();
        worker = new Worker();
        worker->moveToThread(workerThread);

        // streamer->moveToThread(workerThread);
        //EXPEREMENTAL
        

        // connect(workerThread, &QThread::started, streamer, &Streamer::getTracksByBatch);
        // connect(streamer, &Streamer::workFinished, this, &player::onWorkFinished);
        // connect(streamer, &Streamer::workFinished, workerThread, &QThread::quit);
        // connect(streamer, &Streamer::sendBatch, this, &player::batch);

        connect(workerThread, &QThread::started, worker, &Worker::doWork);
        connect(worker, &Worker::workFinished, this, &player::onWorkFinished);
        connect(worker, &Worker::workFinished, workerThread, &QThread::quit);

        downloaderThread = new QThread();
        downloader = new Worker();
        downloader->moveToThread(downloaderThread);

        connect(downloaderThread, &QThread::started, downloader, &Worker::doWork);
        connect(downloader, &Worker::workFinished, this, &player::onDownloadFinished);
        connect(downloader, &Worker::workFinished, downloaderThread, &QThread::quit);










        // connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        // connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);



        //refresh_clicked();
}


void player::remove_clicked(){
    this->workerThread->quit();
    this->downloaderThread->quit();
    std::filesystem::path dir = "./Music";
    for (const auto& entry : std::filesystem::recursive_directory_iterator(dir))
        {
            if (entry.is_regular_file())
            {
                std::string ext = entry.path().extension().string(); 
                if( ext != ".mp3" && ext != ".waw" && ext != ".aac" && ext != ".ogg"){
                    continue;
                }
                std::filesystem::remove(entry.path());

            }
        }
    tracks.clear();
    yandex_tracks.clear();
    tg_tracks.clear();
    ui->listWidget_Tracks->clear();    
}

void player::batch(const QString& tps){
    this->tmp = tps;
    // QByteArray byteArray = tmp.toUtf8();
    *arr = tmp.toUtf8();
    //QByteArray byteArray(tps.toStdString().data(), tps.size() - 1);
    // arr->setRawData(tmp.toStdString().c_str(), tmp.toStdString().size());
    buf->setBuffer(arr);

    //qDebug() << buf->data();
    
    // QBuffer buffer;
    // buffer.setBuffer(&byteArray);
    
    
    
    //ByteStreamDevice *byteStreamDevice = new ByteStreamDevice(&buffer);
    buf->open(QIODevice::ReadOnly);
    //buffer.open(QIODevice::ReadWrite);
    //buffer.seek(qint64(0));

    QAudioFormat format;
    format.setSampleRate(44100); // Sample rate
    format.setChannelCount(2); // Stereo
    format.setSampleSize(16); // 16-bit sample size
    format.setCodec("audio/pcm"); // Codec
    format.setByteOrder(QAudioFormat::LittleEndian); // Byte order
    format.setSampleType(QAudioFormat::SignedInt); // Sample type

    // Create QAudioOutput
    QAudioOutput audioOutput(format);
    audioOutput.setBufferSize(tps.size()); // Set buffer size
    audioOutput.start(buf);
    qDebug() << "play clicked";


}

//workerThread
void player::onWorkFinished() {
    tracks.insert(this->selected.toStdString());
    if(yandex_tracks.contains(this->selected.toStdString())){
        yandex_tracks.erase(this->selected.toStdString());
    }
    if(tg_tracks.contains(this->selected.toStdString())){
        tg_tracks.erase(this->selected.toStdString());
    } 
    ui->label_filename->setText(this->selected);
    QString FileName = QString::fromStdString("./Music/" + this->selected.toStdString() + ".mp3");

    QFileInfo fileInfo(FileName);

    MPlayer->setMedia(QUrl("file://" + fileInfo.absoluteFilePath()));
    ui->labelAnime->hide();
    this->playing = false;
    play_clicked();
    return;
}



void player::download_next(){
    if(ui->listWidget_Tracks->count() == 0){
        return;
    }
    if(shuffle){
        int pos = this->playing_sequence.front();
        this->toDownload = ui->listWidget_Tracks->item(pos)->text();
    }
    else{
        int new_row =  ui->listWidget_Tracks->currentRow() + 1;

        if(new_row >= ui->listWidget_Tracks->count()){
            new_row = ui->listWidget_Tracks->count() - 1;
        }

        this->toDownload = ui->listWidget_Tracks->item(new_row)->text();
    }
    if(tracks.contains(this->toDownload.toStdString())){
        return;
    }
    if (!downloaderThread->isRunning()) {
        downloaderThread->quit();
        downloaderThread->wait();
        std::vector<std::string> tracks_local;
        tracks_local.push_back(this->toDownload.toStdString());
        downloader->set_args(jwt_token.toStdString(), tracks_local, true);
        downloaderThread->start();
    }
    return;
    
}
//downloaderThread
void player::onDownloadFinished(){
    tracks.insert(this->toDownload.toStdString());
    if(yandex_tracks.contains(this->selected.toStdString())){
        yandex_tracks.erase(this->selected.toStdString());
    }
    if(tg_tracks.contains(this->selected.toStdString())){
        tg_tracks.erase(this->selected.toStdString());
    }
    return; 
}

player::~player() {
    if (workerThread->isRunning()) {
        workerThread->quit();
        workerThread->wait();
    }
    delete worker;
    delete workerThread;
    delete ui;
}

void player::shuffle_clicked(){
    
    
    if(shuffle){
        ui->pushButton_Seek_Shuffle->setIcon(icon4);
        shuffle = false;
    }
    else{
        ui->pushButton_Seek_Shuffle->setIcon(icon2);
        shuffle = true;
        while (!playing_sequence.empty()){
            playing_sequence.pop();
        }
        int n = ui->listWidget_Tracks->count();
        generateRandomSequence(playing_sequence, n - 1);
    }
}


void player::repeat_clicked(){
    if(repeat){
        ui->pushButton_Seek_Repeat->setIcon(icon);
        repeat = false;
    }
    else{
        ui->pushButton_Seek_Repeat->setIcon(icon3);
        repeat = true;
    }
}



void player::forward_clicked(){
    if(!ui->listWidget_Tracks->count()){
        return;
    }
    if(!shuffle){
        int new_row =  ui->listWidget_Tracks->currentRow() + 1;

        if(new_row >= ui->listWidget_Tracks->count()){
            new_row = ui->listWidget_Tracks->count() - 1;
        }

        auto itm = ui->listWidget_Tracks->item(new_row);

        selection_clicked(itm);
    }
    else{
        int rows = ui->listWidget_Tracks->count();
        int cur = playing_sequence.front();
        playing_sequence.pop();
        playing_sequence.push(cur);
        auto itm = ui->listWidget_Tracks->item(cur);
        selection_clicked(itm);
    }
    
}
void player::back_clicked(){
    if(!ui->listWidget_Tracks->count()){
        return;
    }
    if(!shuffle){
        int new_row =  ui->listWidget_Tracks->currentRow() - 1;
        if(new_row < 0){
            new_row = 0;
        }
        auto itm = ui->listWidget_Tracks->item(new_row);
        selection_clicked(itm);
    }
    else{
        int rows = ui->listWidget_Tracks->count();
        int cur = playing_sequence.front();
        playing_sequence.pop();
        playing_sequence.push(cur);
        auto itm = ui->listWidget_Tracks->item(cur);
        selection_clicked(itm);
    }
}
bool search_track(const std::string& track_name){
    std::filesystem::path dir = "./Music";
    for (const auto& entry : std::filesystem::recursive_directory_iterator(dir))
    {
        if (entry.is_regular_file() && entry.path().filename() == track_name + ".mp3")
        {
            return true;
        }
    }
    return false;
}

void player::selection_clicked(QListWidgetItem* itm){
    
    ui->listWidget_Tracks->setCurrentItem(itm);
    QString name = itm->text();

   
    //qDebug() << itm->text();

    itm->background().color().blue();


    if(!tracks.contains(name.toStdString())){

        //qDebug() << "not contains\n";            
        if(yandex_tracks.contains(name.toStdString())){
            
            if (!workerThread->isRunning()) {
                ui->labelAnime->show();
                this->selected = name;
                workerThread->quit();
                workerThread->wait();
                std::vector<std::string> tracks_local;
                tracks_local.push_back(name.toStdString());
                worker->set_args(jwt_token.toStdString(), tracks_local, true);
                workerThread->start();
            }
            return;
        }
        else if(tg_tracks.contains(name.toStdString())){          
                  
             if (!workerThread->isRunning()) {
                ui->labelAnime->show();
                this->selected = name;
                workerThread->quit();
                workerThread->wait();
                std::vector<std::string> tracks_local;
                tracks_local.push_back(name.toStdString());
                qDebug() << name.toStdString().data();
                worker->set_args(jwt_token.toStdString(), tracks_local, false);
                workerThread->start();
            }
            return;
                
        }    
    }
    ui->label_filename->setText(name);

    QString FileName = QString::fromStdString("./Music/" + name.toStdString() + ".mp3");

    QFileInfo fileInfo(FileName);
    MPlayer->setMedia(QUrl("file://" + fileInfo.absoluteFilePath()));

    this->playing = false;
    play_clicked();
    return;
}

void player::play_clicked() {
    
    if (is_playing()) {
        MPlayer->pause();
        ui->pushButton_Play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        this->playing = false;
    }
    else{
        MPlayer->play();
        ui->pushButton_Play->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        this->playing = true;
    }
    download_next();
}



bool player::is_playing() const
{
    return this->playing;
}



void player::muted_clicked() {

    if(!MPlayer->isMuted()){
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        MPlayer->setMuted(true);
    }
    else{
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        MPlayer->setMuted(false);
    }

}

void player::on_actionOpen_Audio_File_triggered(bool chk) {
    QString FileName = QFileDialog::getOpenFileName(this, tr("Select Audio File"), "");

    QFileInfo fileInfo(FileName);

    MPlayer->setMedia(QUrl("file://" + FileName));
    ui->label_filename->setText(fileInfo.fileName());
    this->playing = false;
    play_clicked();
}

void player::duration_valueChanged() {
    MPlayer->setPosition(ui->horizontalSlider_Audio_File_Duration->value() * 1000);
}

void player::volume_valueChanged(int value) {
    if(value == 0){
        MPlayer->setMuted(true);
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        return;
    }
    MPlayer->setMuted(false);
    ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    MPlayer->setVolume(value);
}

void player::duration_changed(qint64 duration)  {
    Mduration = duration / 1000;
    ui->horizontalSlider_Audio_File_Duration->setMaximum(Mduration);
}

void player::positin_changed(qint64 progress) {
    if(!ui->horizontalSlider_Audio_File_Duration->isSliderDown()){
        ui->horizontalSlider_Audio_File_Duration->setValue(progress / 1000);
    }
    updateduration(progress / 1000);

}

void player::updateduration(qint64 duration) {
    QString timestr;
    if(duration || Mduration){
        QTime currentTime((duration / 3600) %60, (duration / 60) % 60, duration % 60, (duration * 1000) % 1000 );
        QTime totalTime((Mduration / 3600) %60, (Mduration / 60) % 60, Mduration % 60, (Mduration * 1000) % 1000 );
        QString format = "mm:ss";
        if(Mduration > 3600){
            format = "hh:mm:ss";
        }
        ui->label_value_1->setText(currentTime.toString(format));
        ui->label_value_2->setText(totalTime.toString(format));

        if(duration == Mduration - 1){
            if(repeat){
                MPlayer->setPosition(0);
                return;
            }
            forward_clicked();
        }

    }
}

void player::set_token(const QString& token){
    this->jwt_token = token;
}


void player::ytoken_clicked(){


    bool ok;
    QString text = QInputDialog::getText(nullptr, "Set YANDEX TOKEN", "Enter your yandex token:", QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        int code = this->auth->addUserData({jwt_token.toStdString(), text.toStdString(), "nil"});
        //qDebug() << code;
        std::vector<std::string> tracks_names;
        audio->getList(this->jwt_token.toStdString(), tracks_names, true);
        
        for(const auto& track : tracks_names){
            if(!tracks.contains(track)){
                yandex_tracks.insert(track);
                ui->listWidget_Tracks->addItem(QString::fromStdString(track));
            }
        }


    } else {
        //qDebug() << "Input canceled";
    }
}
void player::chatid_clicked(){
    bool ok;
    QString text = QInputDialog::getText(nullptr, "Set TG CHATID", "Enter your chat id:", QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        int code = this->auth->addUserData({jwt_token.toStdString(), "nil", text.toStdString()});
        std::vector<std::string> tracks_names;
        audio->getList(this->jwt_token.toStdString(), tracks_names, false);
        for(const auto& track : tracks_names){
            if(!tracks.contains(track)){
                tg_tracks.insert(track);
                ui->listWidget_Tracks->addItem(QString::fromStdString(track));
            }
        }
        //qDebug() << code;
    } else {
        //qDebug() << "Input canceled";
    }
}

void player::refresh_clicked(){

    
        QtConcurrent::run([this](){
        //yandex tracks
            audio->getList(jwt_token.toStdString(), tmp_tracks, true);

            for(const auto& track : tmp_tracks){
                
                if(tracks.contains(track)){
                    if(yandex_tracks.contains(track)){
                        yandex_tracks.erase(track);
                    }
                    continue;
                }
                yandex_tracks.insert(track);
                //tracks.insert(track);
                ui->listWidget_Tracks->addItem(QString::fromStdString(track));
            }
            tmp_tracks.clear();
        

    
            //std::vector<std::string> tmp_tracks2;
            audio->getList(jwt_token.toStdString(), tmp_tracks, false);

            for(const auto& track : tmp_tracks){
                
                //qDebug() << track.data();
                if(tracks.contains(track)){
                    if(tg_tracks.contains(track)){
                        tg_tracks.erase(track);
                    }
                    continue;
                }
                tg_tracks.insert(track);
                //tracks.insert(track);
                ui->listWidget_Tracks->addItem(QString::fromStdString(track));
            }

            tmp_tracks.clear();
            
            while (!playing_sequence.empty()){
                playing_sequence.pop();
            }
            if(shuffle){
                generateRandomSequence(playing_sequence, ui->listWidget_Tracks->count() - 1);
            }
        });
}

void player::refresh_clicked1(){
    
    QString text = ui->lineEdit_search->text();


    for(int i = 0; i < ui->listWidget_Tracks->count(); ++i){
        auto itm = ui->listWidget_Tracks->item(i);
        if(!itm->text().toLower().contains(text.toLower())){
            itm->setHidden(true);
        }else {
            itm->setHidden(false);
        }
        
    }
    
}
