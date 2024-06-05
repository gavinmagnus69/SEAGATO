#include <QApplication>
#include <QPushButton>
#include "../UI/login.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    QPushButton button("Hello world!", nullptr);
//    button.resize(200, 100);
//    button.show();
    Login login;
    login.show();
    return QApplication::exec();
}
