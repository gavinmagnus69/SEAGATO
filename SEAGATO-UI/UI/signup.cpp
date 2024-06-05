#include "signup.h"
#include "login.h"
#include "ui_signup.h"
#include <cstdint>
#include <qchar.h>
#include <qdebug.h>
#include <ui_Login.h>
#include <qmessagebox.h>
#include "../sso/sso.h"

SignUp::SignUp(QWidget *parent) : 
    QWidget(parent), ui(new Ui::SignUp),
    sso(new ssoGRPC(grpc::CreateChannel("localhost:44044", grpc::InsecureChannelCredentials()))){
    
    int w = 735;
    int h = 596;
    this->setFixedSize(w, h);
    ui->setupUi(this);
    connect(this->ui->pushButton_backToLogin, &QPushButton::clicked, this, &SignUp::login_clicked);
    connect(this->ui->pushButton_signUp, &QPushButton::clicked, this, &SignUp::signup_clicked);


}

SignUp::~SignUp() {
    delete ui;
}

void SignUp::login_clicked(){
    Login* login = new Login();
    login->show();
    this->close();
}

void SignUp::signup_clicked(){
    QString login = ui->lineEdit_EMAIL->text();
    QString password1 = ui->lineEdit_PASSWORD->text();
    QString password2 = ui->lineEdit_CONFIRM_PASSWORD->text();

    if(login.length() == 0 || password1.length() == 0 || password2.length() == 0){
        QMessageBox::warning(this, "Empty field", "Please, fill empty fields");
        return;
    }
    if(password1 !=  password2){
        QMessageBox::warning(this, "Incorrect field", "Please, passwords should match");
        ui->lineEdit_CONFIRM_PASSWORD->clear();
        return;
    }

    std::variant<std::string, int64_t> ans = sso->Register(login.toStdString(), password1.toStdString());
    if(std::holds_alternative<int64_t>(ans)){
        std::cout << "successful registration\n";
        QMessageBox::information(this, "Success", "Successful regestration");
        Login* login = new Login();
        login->show();
        this->close();
    }
    else{
        qDebug() << std::get<std::string>(ans).data();
        QMessageBox::warning(this, "Error", "User already exists");
        return;
    }




}
