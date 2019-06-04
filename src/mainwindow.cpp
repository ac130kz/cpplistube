#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , manager(new QNetworkAccessManager(this))
    , playlist_id("")

{
    // using ini file
    QSettings settings(QString("config.ini"), QSettings::IniFormat);
    api_key = settings.value("API_KEY").toString();

    // ui setup
    ui->setupUi(this);
    ui->lineEditApi->setText(api_key);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    // handling network and table slots
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::handleReply);
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this,
        [&](int row, int col) {
            // thumbnail column
            if (col == 1) {
                QDesktopServices::openUrl(QUrl("https://www.youtube.com/watch?v=" + ui->tableWidget->item(row, 5)->text()
                    + "&list=" + playlist_id
                    + "&index=" + ui->tableWidget->item(row, 0)->text()));
                //qDebug() << row << " " << col << " " << ui->tableWidget->item(row, col)->text();
            }
        });
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

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox::about(
        this, "Help",
        tr("<p>Paste your Youtube v3 API key and playlist id in the provided<br>inputs, you can also specify your API key via config.ini. Enjoy!</p>"));
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

// TODO: a simple hadle for a spinning animation
void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEditPlaylist->text().isEmpty() || ui->lineEditApi->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Empty playlist id or API key");
        return;
    }
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    playlist_id = ui->lineEditPlaylist->text();
    api_key = ui->lineEditApi->text();

    QUrl url("https://www.googleapis.com/youtube/v3/playlistItems?playlistId=" + playlist_id + "&part=snippet,contentDetails&key=" + api_key + "&maxResults=50");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");
    request.setRawHeader("Connection", "keep-alive");
    manager->get(request);

    this->statusBar()->showMessage(tr("Loading..."));

    // Animation
    // connect(widget, SIGNAL(released()), this, SLOT(NumPressed());

    // in NumPressed
    // QPushButton* button = (QPushButton*) sender();

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

// TODO: thumbnail
void MainWindow::handleReply(QNetworkReply* reply)
{
    if (reply->error()) {
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

        QJsonArray array = jsonobj["items"].toArray();
        auto prevRows = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(prevRows + array.size());
        ui->tableWidget->setColumnCount(8);
        for (int i = 0; i < array.size(); ++i) {
            const auto item = array[i].toObject();
            const auto snippet = item["snippet"].toObject();
            const auto contentDetails = item["contentDetails"].toObject();

            // QImage *img = new QImage();
            // QTableWidgetItem* thumbnail = new QTableWidgetItem;
            // thumbnail->setData(Qt::DecorationRole, QPixmap::fromImage(*img));

            //qDebug() << snippet["position"].toInt() << " " << snippet["title"].toString();
            ui->tableWidget->setItem(prevRows, 0, new QTableWidgetItem(QString::number(snippet["position"].toInt())));
            ui->tableWidget->setItem(prevRows, 1, new QTableWidgetItem(snippet["thumbnail"].toString()));
            ui->tableWidget->setItem(prevRows, 2, new QTableWidgetItem(snippet["title"].toString()));
            ui->tableWidget->setItem(prevRows, 3, new QTableWidgetItem(snippet["channelTitle"].toString()));
            ui->tableWidget->setItem(prevRows, 4, new QTableWidgetItem(snippet["channelId"].toString()));
            ui->tableWidget->setItem(prevRows, 5, new QTableWidgetItem(contentDetails["videoId"].toString()));
            ui->tableWidget->setItem(prevRows, 6, new QTableWidgetItem(snippet["description"].toString()));
            ui->tableWidget->setItem(prevRows++, 7, new QTableWidgetItem(contentDetails["videoPublishedAt"].toString()));
        }

        // continue loading using the nextPageToken
        const auto nextPageToken = jsonobj["nextPageToken"].toString();
        if (!nextPageToken.isEmpty()) {
            QUrl url("https://www.googleapis.com/youtube/v3/playlistItems?playlistId=" + playlist_id + "&part=snippet,contentDetails&key=" + api_key + "&maxResults=50&pageToken=" + nextPageToken);
            QNetworkRequest request(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader,
                "application/x-www-form-urlencoded");
            request.setRawHeader("Connection", "keep-alive");
            manager->get(request);
        } else {
            QJsonObject jsonobjpageinfo = jsonobj["pageInfo"].toObject();
            auto totalResults = "Successfully loaded " + QString::number(jsonobjpageinfo["totalResults"].toInt()) + " results";
            this->statusBar()->showMessage(tr(totalResults.toLatin1().data()));
        }
    }
    reply->deleteLater();
}
