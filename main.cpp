#include "mainwindow.h"
#include <QApplication>
#include "socket_lib.h"

int main(int argc, char *argv[]) {
#ifdef _WIN32
    WSAData wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "Failed to initialize winsock. Error " << WSAGetLastError() << "\n";
        return 1;
    }
#elif __linux__
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int result = a.exec();

#ifdef _WIN32
    WSACleanup(); // Cleanup winsock
#endif

    return result;
}
