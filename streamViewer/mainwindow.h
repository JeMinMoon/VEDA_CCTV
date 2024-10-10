#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Define the Stream structure
struct Stream {
    QMediaPlayer* player;
    QVideoWidget* videoWidget;
    QPushButton* playButton;
    QPushButton* stopButton;
    QSlider* slider;
    QLabel* timeLabel;
    bool isSliderBeingMoved;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void playStream(int index);
    void stopStream(int index);

private:
    Ui::MainWindow *ui;
    Stream streams[4];
    QString rtspUrls[4] = {
        "rtsp://210.99.70.120:1935/live/cctv001.stream",
        "rtsp://210.99.70.120:1935/live/cctv002.stream",
        "rtsp://210.99.70.120:1935/live/cctv003.stream",
        "rtsp://210.99.70.120:1935/live/cctv004.stream"
    };
};

#endif // MAINWINDOW_H
