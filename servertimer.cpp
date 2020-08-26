#include "servertimer.h"

ServerTimer::ServerTimer(QObject *parent) : QObject(parent) {
    connect(&graphTimer, &QTimer::timeout, this, &ServerTimer::UpdateGraphTime);
    graphTime = 0;
}

ServerTimer& ServerTimer::GetInstance(){
    static ServerTimer instance;
    return instance;
}

qreal ServerTimer::Time(){
    return graphTime;
}

QTimer& ServerTimer::Timer(){
    return graphTimer;
}

void ServerTimer::ResetTime(){
    graphTime = 0;
}

void ServerTimer::SetTime(qreal _time){
    graphTime = _time;
}

void ServerTimer::RunFreChanged(int run){
    if(run){
        graphTimer.start();
    }
    else{
        graphTimer.stop();
    }
}

void ServerTimer::UpdatePeriodChanged(QString txt){
    if(graphTimer.isActive()) graphTimer.start(txt.toInt());
    else graphTimer.setInterval(txt.toInt());
}

void ServerTimer::UpdateGraphTime(){
    graphTime += static_cast<qreal>(graphTimer.interval()) / 1000;
}
