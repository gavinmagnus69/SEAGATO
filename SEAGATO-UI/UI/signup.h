//
// Created by roman on 5/30/24.
//

#ifndef QTUI_SIGNUP_H
#define QTUI_SIGNUP_H

#include <QWidget>
#include <memory>
#include <qobjectdefs.h>
#include <qt/QtCore/qobjectdefs.h>
#include "../Interfaces/Isso.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SignUp; }
QT_END_NAMESPACE

class SignUp : public QWidget {
Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);

    ~SignUp() override;

private:
    std::shared_ptr<Isso> sso;
    Ui::SignUp *ui;
private slots:
    void login_clicked();
    void signup_clicked();

};


#endif //QTUI_LOGIN_H
