#include "include/mainwindow.h"
#include <QApplication>

// The main function of the application
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();
    return app.exec();
}
