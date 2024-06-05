//
// Created by roman on 5/30/24.
//

#ifndef QTUI_LOGIN_H
#define QTUI_LOGIN_H

#include <QWidget>
#include <memory>
#include <qobjectdefs.h>

#include "../Interfaces/IServerAuth.h"
#include "../Interfaces/Isso.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget {
Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);

    ~Login() override;

private slots:
    void login_clicked();
    void signup_clicked();


private:
    std::shared_ptr<Isso> sso;
    std::shared_ptr<IServerAuth> auth;

    Ui::Login *ui;

private:
    int AuthUser(const std::string&);
};


#endif //QTUI_LOGIN_H
