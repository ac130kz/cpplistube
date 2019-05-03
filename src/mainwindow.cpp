#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , manager(new QNetworkAccessManager(this))
    , reply(nullptr)
    , api_key("")
    , playlist_id("")

{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
}

void MainWindow::on_actionClear_triggered()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}

void MainWindow::on_actionQuit_triggered()
{
    if (QMessageBox::question(this, "Quit", "Do you want to quit?",
            QMessageBox::Yes | QMessageBox::No)
        == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QFile file(QFileDialog::getSaveFileName(this, "Save as"));
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error",
            "Cannot save the results: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    // TODO: write global json to file
    out << "Test!";
    file.close();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(
        this, "About",
        tr("<p>This is a simple YouTube video playlist tool.</p>"));
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

///////////////////////////////////////////////////////////////////////////////

// TODO: this is a simple hadle for a spinning animation
void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEditPlaylist->text().isEmpty() || ui->lineEditApi->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Empty playlist id or API key");
        return;
    }
    playlist_id = ui->lineEditPlaylist->text();
    api_key = ui->lineEditApi->text();

    QUrl url("https://www.googleapis.com/youtube/v3/playlistItems?playlistId=" + playlist_id + "&part=snippet,contentDetails&key=" + api_key + "&maxResults=50");
    QNetworkRequest request(url);

    // TODO: already handled automatically?
    // if (setttings->useHttps) {
    //    QSslConfiguration config = request.sslConfiguration();
    //    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    //    config.setProtocol(QSsl::SecureProtocols);
    //    request.setSslConfiguration(config);
    // }
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");
    request.setRawHeader("Connection", "keep-alive");

    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this,
        &MainWindow::handleReply);

    // Animation
    // connect(widget, SIGNAL(released()), this, SLOT(NumPressed());

    // in NumPressed
    // QPushButton* button = (QPushButton*) sender();

    // https://stackoverflow.com/a/26958738/6229350
    // ajaxload.info/
    // QLabel *lbl = new QLabel;
    // QMovie *movie = new QMovie("G:/loader.gif");
    // lbl->setMovie(movie);
    // lbl->show();
    // movie->start();
    // You can get gif-animation from here or use another gif.
    // I think that it is the easiest way because you can create this animation in
    // app with timer, color changing and so on, but it requires a lot of work and
    // time. But QMovie is powerful and easy to use class.

    // remember to destroy movie. From doc: "The label does NOT take ownership of
    // the movie."
}

// TODO: while 'pageToken'
// TODO: double click follow link https://www.youtube.com/watch?v={videoId}&list={playlistId}&index={position}
// TODO: thumbnail
void MainWindow::handleReply()
{
    if (!reply) {
        return;
    } else if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::warning(this, "Error", reply->errorString());
    } else {
        auto data = reply->readAll();
        auto jsonobj = QJsonDocument::fromJson(data).object();

        // DEBUG
        //QString dataReply(data);
        //QMessageBox::warning(this, "Data", dataReply);
        qDebug()
            << "nextPageToken: "
            << jsonobj["nextPageToken"].toString();
        qDebug() << "kind: " << jsonobj["kind"].toString();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                        .toString();

        QJsonObject jsonobjpageinfo = jsonobj["pageInfo"].toObject();
        auto totalResults = "Successfully loaded " + QString::number(jsonobjpageinfo["totalResults"].toInt()) + " results";
        this->statusBar()->showMessage(tr(totalResults.toLatin1().data()), 3000);

        QJsonArray array = jsonobj["items"].toArray();
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(array.size());
        ui->tableWidget->setColumnCount(8);
        for (int i = 0; i < array.size(); ++i) {
            auto item = array[i].toObject();
            auto snippet = item["snippet"].toObject();
            auto contentDetails = item["contentDetails"].toObject();

            // QTableWidgetItem* thumbnail = new QTableWidgetItem;
            // thumbnail->setData(Qt::DecorationRole, QPixmap::fromImage(*img));

            qDebug() << snippet["position"].toInt() << " " << snippet["title"].toString();
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(snippet["position"].toInt())));
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(snippet["thumbnail"].toString()));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(snippet["title"].toString()));
            ui->tableWidget->setItem(i, 3, new QTableWidgetItem(snippet["channelTitle"].toString()));
            ui->tableWidget->setItem(i, 4, new QTableWidgetItem(snippet["channelId"].toString()));
            ui->tableWidget->setItem(i, 5, new QTableWidgetItem(contentDetails["videoId"].toString()));
            ui->tableWidget->setItem(i, 6, new QTableWidgetItem(snippet["description"].toString()));
            ui->tableWidget->setItem(i, 7, new QTableWidgetItem(contentDetails["videoPublishedAt"].toString()));
        }
    }
    reply->deleteLater();
    reply = nullptr;
}
