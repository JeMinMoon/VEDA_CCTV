#ifndef PLAYBACKWIDGET_H
#define PLAYBACKWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSlider>
#include <QLabel>

class PlaybackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaybackWidget(QWidget *parent = nullptr);
    ~PlaybackWidget();

    void setFilePath(const QString &path);
    void play();
    void pause();
    void seek(qint64 position);

private slots:
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);

private:
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QSlider *slider;
    QLabel *timeLabel;
};

#endif // PLAYBACKWIDGET_H
