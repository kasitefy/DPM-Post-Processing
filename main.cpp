#include "mainwindow.h"
#include <QApplication>
#include <QIcon>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("DPM Post-Processing");
    w.setWindowIcon(QIcon("qtconfig.ico"));
    w.show();
    return a.exec();
}
