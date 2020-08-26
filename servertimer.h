#ifndef SERVERTIMER_H
#define SERVERTIMER_H

#include <QObject>
#include <QTimer>
#include <QDebug>

class ServerTimer : public QObject
{
    Q_OBJECT
public:
    static ServerTimer& GetInstance();
    qreal Time();
    void ResetTime();
    void SetTime(qreal);
    QTimer& Timer();

private:
    explicit ServerTimer(QObject *parent = nullptr);
    qreal graphTime;
    QTimer graphTimer;

public slots:
    void RunFreChanged(int);
    void UpdatePeriodChanged(QString);
    void UpdateGraphTime();
};

#endif // SERVERTIMER_H
