#include "playbackwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTime>

PlaybackWidget::PlaybackWidget(QWidget *parent) : QWidget(parent)
{
    mediaPlayer = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    slider = new QSlider(Qt::Horizontal, this);
    timeLabel = new QLabel("00:00 / 00:00", this);

    // Layout setup
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(videoWidget);

    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->addWidget(slider);
    controlLayout->addWidget(timeLabel);

    mainLayout->addLayout(controlLayout);
    setLayout(mainLayout);

    // Connect signals
    mediaPlayer->setVideoOutput(videoWidget);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &PlaybackWidget::updatePosition);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &PlaybackWidget::updateDuration);
    connect(slider, &QSlider::sliderMoved, this, &PlaybackWidget::seek);
}

PlaybackWidget::~PlaybackWidget()
{
    mediaPlayer->stop();
}

void PlaybackWidget::setFilePath(const QString &path)
{
    mediaPlayer->setSource(QUrl::fromLocalFile(path));
}

void PlaybackWidget::play()
{
    mediaPlayer->play();
}

void PlaybackWidget::pause()
{
    mediaPlayer->pause();
}

void PlaybackWidget::seek(qint64 position)
{
    mediaPlayer->setPosition(position);
}

void PlaybackWidget::updatePosition(qint64 position)
{
    if (slider->isSliderDown())
        return;
    slider->setValue(static_cast<int>(position));
    QTime currentTime = QTime::fromMSecsSinceStartOfDay(static_cast<int>(position));
    QTime totalTime = QTime::fromMSecsSinceStartOfDay(static_cast<int>(mediaPlayer->duration()));
    timeLabel->setText(currentTime.toString("mm:ss") + " / " + totalTime.toString("mm:ss"));
}

void PlaybackWidget::updateDuration(qint64 duration)
{
    slider->setRange(0, static_cast<int>(duration));
}
