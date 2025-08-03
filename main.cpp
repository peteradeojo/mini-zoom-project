#include "mainwindow.h"
#include <QApplication>
#include "src/socket_helper.h"

int main(int argc, char *argv[]) {
#ifdef _WIN32
    socket_helper::WinsockInit wsa;
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
