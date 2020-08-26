#ifndef SOUNDMANAGE_H
#define SOUNDMANAGE_H

#include "default.h"
#include <QObject>
#include <QThread>
#include <QSoundEffect>
#include <QTimer>

class SoundManage : public QObject{
    Q_OBJECT
public:
    SoundManage();
    ~SoundManage();
private:
    QSoundEffect* beepSoundLow;
    QSoundEffect* beepSoundHigh;
    QThread* soundThread;
    bool lowState;
    bool highState;
public slots:
    void SoundPlay(SOUND_TYPE type);
    void ChangeLowState(bool);
    void ChangeHighState(bool);
};


#endif // SOUNDMANAGE_H
