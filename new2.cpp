#include "new2.h"
#include "ui_new2.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QMessageBox>
#include <QIcon>  // add QIcon

new2::new2(QWidget *parent)
    : QWidget(parent), isWorking(false), totalWorkTime(0, 0, 0), totalRestTime(0, 0, 0) {
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *topLayout = new QHBoxLayout;
    statusLabel = new QLabel("work", this);  // initial to work status
    currentTimeLabel = new QLabel(QDateTime::currentDateTime().toString(), this);
    topLayout->addWidget(statusLabel);
    topLayout->addStretch();
    topLayout->addWidget(currentTimeLabel);

    // font setting
    QString topLabelStyle = "font-size: 15px; font-weight: bold;";
    statusLabel->setStyleSheet(topLabelStyle);
    currentTimeLabel->setStyleSheet(topLabelStyle);
    QString labelStyle = "font-size: 15px; font-weight: bold; background-color: green;";

    // Layout of first line:remaining work time, current break time, next break time
    QHBoxLayout *firstRowLayout = new QHBoxLayout;
    workRemainLabel = new QLabel("work remain: 08:00:00", this);
    currentRestLabel = new QLabel("current rest: 01:00:00", this);
    QLabel *nextRestLabel = new QLabel("next rest: 00:00", this);

    workRemainLabel->setStyleSheet(labelStyle);
    currentRestLabel->setStyleSheet(labelStyle);
    nextRestLabel->setStyleSheet(labelStyle);

    firstRowLayout->addWidget(workRemainLabel);
    firstRowLayout->addWidget(currentRestLabel);
    firstRowLayout->addWidget(nextRestLabel);

    // Layout of the second row:total hours worked, total time off, and last time off
    QHBoxLayout *secondRowLayout = new QHBoxLayout;
    totalWorkTimeLabel = new QLabel("total work time: 00:00:00", this);
    totalRestTimeLabel = new QLabel("total rest time: 00:00:00", this);
    lastRestTimeLabel = new QLabel("last rest time: 00:00", this);

    totalWorkTimeLabel->setStyleSheet(labelStyle);
    totalRestTimeLabel->setStyleSheet(labelStyle);
    lastRestTimeLabel->setStyleSheet(labelStyle);

    secondRowLayout->addWidget(totalWorkTimeLabel);
    secondRowLayout->addWidget(totalRestTimeLabel);
    secondRowLayout->addWidget(lastRestTimeLabel);

    // Work button, rest button and SOS button
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    workButton = new QPushButton("Work", this);
    restButton = new QPushButton("Rest", this);
    sosButton = new QPushButton("SOS", this);
    buttonLayout->addWidget(workButton);
    buttonLayout->addWidget(restButton);
    buttonLayout->addWidget(sosButton);
    sosButton->setIcon(QIcon(":/images/sos_icon.jpg"));  // icon set
    sosButton->setIconSize(QSize(50, 50));  // icon size

    // add to mainlayout
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(firstRowLayout);
    mainLayout->addLayout(secondRowLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // timer set
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &new2::updateTime);
    connect(timer, &QTimer::timeout, this, &new2::checkWorkRest);
    connect(workButton, &QPushButton::clicked, this, &new2::startWork);
    connect(restButton, &QPushButton::clicked, this, &new2::startRest);
    connect(sosButton, &QPushButton::clicked, this, &new2::sendSOS);

    timer->start(1000);  // update every second
    resetTimers();
}
new2::~new2() {
    // destructor (computing)
}

void new2::updateTime() {
    // update current time
    currentTimeLabel->setText(QDateTime::currentDateTime().toString("hh:mm"));

    if (isWorking) {
        // if work，update work time
        QTime currentTime = QTime::currentTime();
        int elapsedSecs = workStartTime.secsTo(currentTime);
        QTime remainTime = QTime(8, 0, 0).addSecs(-elapsedSecs);
        workRemainLabel->setText("work remain: " + remainTime.toString("hh:mm:ss"));
    } else {
        // if rest，update rest time
        QTime currentTime = QTime::currentTime();
        int elapsedSecs = restStartTime.secsTo(currentTime);
        currentRestLabel->setText("current rest: " + QTime(1, 0, 0).addSecs(-elapsedSecs).toString("hh:mm:ss"));
    }
}

void new2::checkWorkRest() {
    if (isWorking && workStartTime.secsTo(QTime::currentTime()) >= 8 * 3600) {
        // reach to 8 hrs working，warning
        QMessageBox::warning(this, "Warning", "You have worked for 8 hours!");
        switchToRest();
    }

    // total work time
    totalWorkTimeLabel->setText("total work time: " + totalWorkTime.toString("hh:mm:ss"));
    totalRestTimeLabel->setText("total rest time: " + totalRestTime.toString("hh:mm:ss"));
}

void new2::sendSOS() {
    // sos signal(by hand)
    QMessageBox::critical(this, "SOS", "SOS signal sent!");
}

void new2::startWork() {
    // start work
    if (!isWorking) {
        workStartTime = QTime::currentTime();
        switchToWork();
    }
}

void new2::startRest() {
    // start rest
    if (isWorking) {
        restStartTime = QTime::currentTime();
        switchToRest();
    }
}

void new2::switchToWork() {
    isWorking = true;
    totalRestTime = totalRestTime.addSecs(restStartTime.secsTo(QTime::currentTime()));
    statusLabel->setText("work");
    workButton->setEnabled(false);
    restButton->setEnabled(true);
}

void new2::switchToRest() {
    isWorking = false;
    totalWorkTime = totalWorkTime.addSecs(workStartTime.secsTo(QTime::currentTime()));
    lastRest = QTime::currentTime();
    statusLabel->setText("rest");
    workButton->setEnabled(true);
    restButton->setEnabled(false);
    lastRestTimeLabel->setText("last rest time: " + lastRest.toString("hh:mm"));
}

void new2::resetTimers() {
    // refresh timer
    workRemainLabel->setText("work remain: 08:00:00");
    currentRestLabel->setText("current rest: 01:00:00");
    totalWorkTimeLabel->setText("total work time: 00:00:00");
    totalRestTimeLabel->setText("total rest time: 00:00:00");
    lastRestTimeLabel->setText("last rest time: 00:00");
    isWorking = true;
}

