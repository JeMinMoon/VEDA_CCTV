#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMediaPlayer>
#include <QVideoWidget>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTime>
#include <QDebug>
#include "livewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // QGridLayout* gridLayout = new QGridLayout();
    // gridLayout->setSpacing(1);
    // for (int i = 0; i < 2; i++) {
    //     for (int j = 0; j < 2; j++) {
    //         LiveWidget* display = new LiveWidget();
    //         // displays[display] = qMakePair(i, j);
    //         display->setObjectName(QString("display_%1").arg(i*2 + j));
    //         // connect(display, &LiveWidget::focusChanged, this, [=](LiveWidget* widget, bool hasFocus) {
    //         //     if (hasFocus) {
    //         //         focusedDisplay = widget;
    //         //         qDebug() << "focussed display changed" << focusedDisplay->objectName();
    //         //     }
    //         // });
    //         display->setContentsMargins(2,2,2,2);
    //         gridLayout->addWidget(display);
    //         display->playLive(rtspUrls[i*2 + j]);
    //     }
    // }
    // ui->centralwidget->setLayout(gridLayout);



    // Initialize Grid Layout
    QGridLayout* gridLayout = new QGridLayout();
    ui->centralwidget->setLayout(gridLayout);

    // Initialize 4 Video Streams
    for(int i = 0; i < 4; ++i){
        QLabel *rtspLabel = new QLabel(this);
        rtspLabel->setText("RTSP URL : ");
        // Initialize Stream Data
        streams[i].liveWidget = new LiveWidget(this);
        // streams[i].playbackWidget = new PlaybackWidget(this);
        streams[i].playLiveButton = new QPushButton("Play", this);
        streams[i].stopLiveButton = new QPushButton("Stop", this);
        streams[i].lineEdit = new QLineEdit(this);
        streams[i].lineEdit->setText(rtspUrls[i]);
        // streams[i].slider = new QSlider(Qt::Horizontal, this);
        // streams[i].timeLabel = new QLabel("00:00 / 00:00", this);
        // streams[i].isSliderBeingMoved = false;

        // // Set QLabel properties
        // streams[i].timeLabel->setAlignment(Qt::AlignCenter); // 텍스트 중앙 정렬
        // streams[i].timeLabel->setMinimumWidth(100); // 최소 너비 설정

        // // Set Video Output
        // streams[i].player->setVideoOutput(streams[i].videoWidget);
        // streams[i].player->setSource(QUrl(rtspUrls[i]));
        // streams[i].liveWidget->playLive(rtspUrls[i]);

        // // Disable slider until media is loaded
        // streams[i].slider->setRange(0, 0);
        // streams[i].slider->setEnabled(false);

        // Connect Play and Stop Buttons
        connect(streams[i].playLiveButton, &QPushButton::clicked, [this, i]() { playStream(i); });
        connect(streams[i].stopLiveButton, &QPushButton::clicked, [this, i]() { stopStream(i); });

        // // Connect media player's durationChanged signal to set slider range
        // connect(streams[i].player, &QMediaPlayer::durationChanged, [this, i](qint64 duration) {
        //     streams[i].slider->setRange(0, static_cast<int>(duration));
        //     streams[i].slider->setEnabled(streams[i].player->isSeekable());
        //     if (!streams[i].player->isSeekable()) {
        //         streams[i].slider->setRange(0, 0);
        //         streams[i].slider->setToolTip("Seeking not supported for this stream.");
        //         qDebug() << "Stream" << i << "is not seekable.";
        //     }
        // });

        // // Connect media player's positionChanged signal to update slider and time label
        // connect(streams[i].player, &QMediaPlayer::positionChanged, [this, i](qint64 position) {
        //     if (!streams[i].isSliderBeingMoved) {
        //         streams[i].slider->setValue(static_cast<int>(position));
        //     }

        //     // Update time label using fromMSecsSinceStartOfDay for accurate time formatting
        //     qint64 duration = streams[i].player->duration();
        //     QTime currentTime = QTime::fromMSecsSinceStartOfDay(static_cast<int>(position));
        //     QTime totalTime = QTime::fromMSecsSinceStartOfDay(static_cast<int>(duration));
        //     QString format = "mm:ss";
        //     QString timeText = currentTime.toString(format) + " / " + totalTime.toString(format);
        //     streams[i].timeLabel->setText(timeText);

        //     // Debug logs
        //     qDebug() << "Stream" << i << "Position:" << position << "Duration:" << duration;
        //     qDebug() << "TimeLabel Text:" << timeText;
        // });

        // // Connect media player's mediaStatusChanged to check if seekable
        // connect(streams[i].player, &QMediaPlayer::mediaStatusChanged, [this, i](QMediaPlayer::MediaStatus status) {
        //     if (status == QMediaPlayer::LoadedMedia || status == QMediaPlayer::BufferedMedia) {
        //         bool isSeekable = streams[i].player->isSeekable();
        //         streams[i].slider->setEnabled(isSeekable);
        //         if (!isSeekable) {
        //             streams[i].slider->setRange(0, 0);
        //             streams[i].slider->setToolTip("Seeking not supported for this stream.");
        //             qDebug() << "Stream" << i << "is not seekable.";
        //         }
        //     }
        // });

        // // Connect media player's error signal
        // connect(streams[i].player, &QMediaPlayer::errorOccurred, [this, i](QMediaPlayer::Error error) {
        //     qDebug() << "Stream" << i << "Error Occurred:" << streams[i].player->errorString();
        // });

        // // Connect slider interactions
        // connect(streams[i].slider, &QSlider::sliderPressed, [this, i]() {
        //     streams[i].isSliderBeingMoved = true;
        // });

        // connect(streams[i].slider, &QSlider::sliderReleased, [this, i]() {
        //     streams[i].isSliderBeingMoved = false;
        //     streams[i].player->setPosition(streams[i].slider->value());
        //     qDebug() << "Seeking to position:" << streams[i].slider->value() << "ms in stream:" << rtspUrls[i];
        // });

        // Arrange Layouts
        // Buttons Layout
        QHBoxLayout* buttonsLayout = new QHBoxLayout();
        buttonsLayout->addWidget(rtspLabel);
        buttonsLayout->addWidget(streams[i].lineEdit);
        buttonsLayout->addWidget(streams[i].playLiveButton);
        buttonsLayout->addWidget(streams[i].stopLiveButton);
        // buttonsLayout->addWidget(streams[i].timeLabel);
        // buttonsLayout->addWidget(streams[i].slider);

        // Vertical Layout for Video, Buttons, Slider, and Time Label
        QVBoxLayout* verticalLayout = new QVBoxLayout();
        verticalLayout->addWidget(streams[i].liveWidget);
        verticalLayout->addLayout(buttonsLayout);
        verticalLayout->addLayout(buttonsLayout);

        QWidget* container = new QWidget(this);
        container->setLayout(verticalLayout);

        // Add to Grid Layout (2x2)
        gridLayout->addWidget(container, i / 2, i % 2);
    }

    // Set Initial Window Size
    this->resize(1200, 800);
}

void MainWindow::playStream(int index)
{
    if(index < 0 || index >= 4){
        qDebug() << "Invalid stream index:" << index;
        return;
    }
    streams[index].lineEdit->setEnabled(false);
    streams[index].liveWidget->playLive(streams[index].lineEdit->text());
    qDebug() << "Playing stream:" << streams[index].lineEdit->text();
}

void MainWindow::stopStream(int index)
{
    if(index < 0 || index >= 4){
        qDebug() << "Invalid stream index:" << index;
        return;
    }
    streams[index].lineEdit->setEnabled(true);
    streams[index].liveWidget->stopLive(); // Use pause to maintain current position
    qDebug() << "Stopping stream:" << streams[index].lineEdit->text();
}
