#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
#if defined(Q_OS_WIN)
    // https://github.com/qbittorrent/qBittorrent/issues/4209
    qputenv("QT_BEARER_POLL_TIMEOUT", QByteArray::number(-1));
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
