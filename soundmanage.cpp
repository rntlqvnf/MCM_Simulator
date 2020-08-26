#include "soundmanage.h"

SoundManage::SoundManage(){
    lowState = highState = true;
    beepSoundLow = new QSoundEffect();
    beepSoundLow->setSource(QUrl("qrc:/audio/audio/beep_low.wav"));
    beepSoundHigh = new QSoundEffect();
    beepSoundHigh->setSource(QUrl("qrc:/audio/audio/beep_high.wav"));

    soundThread = new QThread();
    soundThread->start();
    beepSoundLow->moveToThread(soundThread);
    beepSoundHigh->moveToThread(soundThread);
}

SoundManage::~SoundManage(){
    soundThread->quit();
}

void SoundManage::SoundPlay(SOUND_TYPE type){
    QTimer* timer = new QTimer();
    timer->moveToThread(soundThread);
    if(type == LOW){
        if(lowState) timer->singleShot(0, beepSoundLow, SLOT(play()));
    }
    else {
        if(highState) timer->singleShot(0, beepSoundHigh, SLOT(play()));
    }
}

void SoundManage::ChangeLowState(bool state){
    lowState = state;
}

void SoundManage::ChangeHighState(bool state){
    highState = state;
}
