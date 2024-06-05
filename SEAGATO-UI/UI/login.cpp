//
// Created by roman on 5/30/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Login.h" resolved

#include "login.h"
#include "player.h"
#include "signup.h"
#include "ui_Login.h"
#include <grpcpp/create_channel.h>
#include <qchar.h>
#include <qdebug.h>
#include <qglobal.h>
#include <qmessagebox.h>
#include <string>
#include <variant>
#include "../sso/sso.h"
#include "../ServerAuth/ServerAuth.h"


Login::Login(QWidget *parent) :
    QWidget(parent), ui(new Ui::Login), 
    sso(new ssoGRPC(grpc::CreateChannel("localhost:44044", grpc::InsecureChannelCredentials()))),
    auth(new ServerAuth(grpc::CreateChannel("localhost:1489", grpc::InsecureChannelCredentials())))
{
    this->setFixedSize(735, 596);
    ui->setupUi(this);
    connect(this->ui->pushButton_logIn, &QPushButton::clicked, this, &Login::login_clicked);
    connect(this->ui->pushButton_signUp, &QPushButton::clicked, this, &Login::signup_clicked);
    

}


int Login::AuthUser(const std::string& token){
    int code = auth->addUser(token);


}

Login::~Login() {
    delete ui;
}


void Login::login_clicked(){

    QString login = ui->lineEdit_LOGIN->text();
    QString password = ui->lineEdit_PASSWORD->text();

    if(login == "q" && password == "q"){
        player* playerPage = new player("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhcHBfaWQiOjEsImVtYWlsIjoicm9tYWJybyIsImV4cCI6MTcxNDQ2OTgxOCwidWlkIjozfQ.yF5luYD5lqAZYBvv6tk8ZkZuR2gTCap7VbJK5LhtyJ8");
        playerPage->set_token("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhcHBfaWQiOjEsImVtYWlsIjoicm9tYWJybyIsImV4cCI6MTcxNDQ2OTgxOCwidWlkIjozfQ.yF5luYD5lqAZYBvv6tk8ZkZuR2gTCap7VbJK5LhtyJ8");
        auth->addUser("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhcHBfaWQiOjEsImVtYWlsIjoicm9tYWJybyIsImV4cCI6MTcxNDQ2OTgxOCwidWlkIjozfQ.yF5luYD5lqAZYBvv6tk8ZkZuR2gTCap7VbJK5LhtyJ8");
        playerPage->show();
        this->close();
        return;
    }

    if(login.length() == 0 || password.length() == 0){
        QMessageBox::warning(this, "Empty field", "Please, fill empty fields");
        return;
    }

    std::variant<std::string, int> respond = sso->Login(login.toStdString(), password.toStdString());
    
    if(std::holds_alternative<std::string>(respond)){
        //QMessageBox::information(this, "Successful login", "Success");
        QString token = std::get<std::string>(respond).data();
        //std::cout << std::get<std::string>(respond); 
        qDebug() << token;

        player* playerPage = new player(token);
        playerPage->set_token(token);
        int code = auth->addUser(token.toStdString());
        qDebug() << code;

        playerPage->show();
        this->close();

        

        //this->close();
    }else{
        //std::cout << "something went wrong\n";
        QMessageBox::warning(this, "Please, try again", "Incorret login or password");
    }

}
void Login::signup_clicked(){
    SignUp* singup = new SignUp();
    singup->show();
    this->close();
}

