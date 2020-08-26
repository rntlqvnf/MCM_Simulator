#ifndef DATARECEIVE_H
#define DATARECEIVE_H

#include "default.h"
#include "servertimer.h"
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QRegExp>
#include <QVector>

class DataReceive : public QObject {
    Q_OBJECT
protected:
    QSerialPort* port;
public:
    DataReceive(QSerialPort*);
public slots:
    void GetDataAndEmit();
signals:
    void CalculationSignal(qreal, qreal, qreal);
};

class DataReceive_2 : public DataReceive {
    Q_OBJECT
public:
    DataReceive_2(QSerialPort*);
public slots:
    void GetDataAndEmit();
signals:
    void SecondTabUpdateSignal(qreal, qreal, qreal, qreal, qreal, qreal, qreal);
};

#endif // DATARECEIVE_H
