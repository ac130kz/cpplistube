#pragma once

#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QSsl>
#include <QSslConfiguration>
#include <QSslError>
#include <QTextStream>
#include <QUrl>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();
    void on_actionSave_as_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();
    void on_pushButton_clicked();
    void on_actionClear_triggered();
    void on_actionHelp_triggered();
    void handleReply(QNetworkReply* reply);

private:
    Ui::MainWindow* ui;
    QNetworkAccessManager* manager;
    QString api_key;
    QString playlist_id;
};
