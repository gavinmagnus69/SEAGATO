//
// Created by roman on 5/20/24.
//

#ifndef QTUI_PLAYER_H
#define QTUI_PLAYER_H

#include <QMainWindow>
#include <QtMultimedia>
#include <QtCore>
#include <QtWidgets>
#include <qbuffer.h>
#include <qchar.h>
#include <qicon.h>
#include <qlistwidget.h>
#include <qobject.h>
#include <qthread.h>
#include <queue>
#include <string>
#include <thread>
#include <ui_Login.h>
#include <unordered_set>
#include <vector>
#include "Streamer.h"
#include "Worker.h"
#include "login.h"
#include "../Interfaces/IAudioServer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class player; }
QT_END_NAMESPACE

class player : public QMainWindow {
Q_OBJECT

public:
    explicit player(const QString& str ,QWidget *parent = nullptr);

    ~player() override;

    void set_token(const QString&);
private:
    bool playing;
    bool is_muted;
    bool shuffle;
    bool repeat;
    QMediaPlayer* MPlayer;
    qint64 Mduration;
    QString jwt_token = "";

    void updateduration(qint64 duration);
private:
    Ui::player *ui;
    Login* log;
    std::shared_ptr<IServerAuth> auth;
    std::shared_ptr<IAudioServer> audio;
    std::unordered_set<std::string> tracks; //local
    std::vector<std::string> tmp_tracks;
    std::unordered_set<std::string> yandex_tracks;
    std::unordered_set<std::string> tg_tracks;
    std::queue<int> playing_sequence;
    int prev = -1;
    int cur = -1;

    QIcon icon;
    QIcon icon2;
    QIcon icon3;
    QIcon icon4;

    QThread* workerThread;
    Worker* worker;
    Streamer* streamer;
    Worker* downloader;
    QThread* downloaderThread;
    QString selected;
    QString toDownload;
    QByteArray* arr;
    QBuffer* buf;
    QString tmp;
    QMovie* movik;

private slots:
    void play_clicked();
    void refresh_clicked();
    void refresh_clicked1();

    void back_clicked();
    void forward_clicked();

    void repeat_clicked();
    void shuffle_clicked();
    void batch(const QString&);

    // void play_next();
    // void play_prev();

    void muted_clicked();
    void ytoken_clicked();
    void chatid_clicked();
    void on_actionOpen_Audio_File_triggered(bool);
    void duration_valueChanged();
    void volume_valueChanged(int value);
    bool is_playing() const;
    void duration_changed(qint64);
    void positin_changed(qint64);
    void selection_clicked(QListWidgetItem*);

    void onWorkFinished();
    void onDownloadFinished();
    void download_next();

    void remove_clicked();
};


#endif //QTUI_PLAYER_H
