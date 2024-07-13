#ifndef NEW2_H
#define NEW2_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QTime>

namespace Ui {
class new2;
}


class new2 : public QWidget
{
    Q_OBJECT

public:
    new2(QWidget *parent = nullptr);
    ~new2();

private slots:
    void updateTime();
    void checkWorkRest();
    void sendSOS();
    void startWork();
    void startRest();

private:
    //Ui::new2 *ui;
    QLabel *statusLabel;
    QLabel *currentTimeLabel;
    QLabel *workRemainLabel;
    QLabel *currentRestLabel;
    QLabel *totalWorkTimeLabel;
    QLabel *totalRestTimeLabel;
    QLabel *lastRestTimeLabel;
    QPushButton *sosButton;        // SOS
    QPushButton *workButton;
    QPushButton *restButton;
    QTimer *timer;                 // timer
    QTime workStartTime;
    QTime restStartTime;
    QTime totalWorkTime;
    QTime totalRestTime;
    QTime lastRest;                // last rest time
    bool isWorking;                // is working/not

    void resetTimers();
    void switchToWork();
    void switchToRest();
};

#endif // NEW2_H
