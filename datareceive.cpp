#include "datareceive.h"

DataReceive::DataReceive(QSerialPort* _port) : port(_port){
}

void DataReceive::GetDataAndEmit(){
    bool testMode = true;

    QString rawdata;
    QStringList dataGroup;
    qreal fineData = 0, coarseData = 0;

    if(testMode){
        fineData = qrand() % ((3 + 1) - 0) + 0;
        coarseData = qrand() % ((3 + 1) - 0) + 0;
        emit CalculationSignal(fineData, coarseData, ServerTimer::GetInstance().Time());
        return;
    }

    if(port->isReadable()){
        rawdata = port->readAll();
        QRegExp rx("([0-9]\\.[0-9]{2})\\s([0-9]\\.[0-9]{2})");
        int pos = 0;

        if(!rawdata.contains(rx)) return;

        while((pos = rx.indexIn(rawdata, pos)) != -1){
            fineData = rx.cap(1).toDouble();
            coarseData = rx.cap(2).toDouble();
            pos += rx.matchedLength();
        }
        emit CalculationSignal(fineData, coarseData, ServerTimer::GetInstance().Time());
    }
    else{
        return;
    }
}

/*******************
 *
 * Data Receiving tab 2
 *
 * ******************/

DataReceive_2::DataReceive_2(QSerialPort* _port) : DataReceive(_port){
}

void DataReceive_2::GetDataAndEmit(){
    bool testMode = false;

    QString rawdata;
    QStringList dataGroup;
    qreal degree = 0, rs = 0, ox = 0, Dr = 0, Dg = 0, Db = 0;

    if(testMode){
        degree = qrand() % ((3 + 1) - 0) + 0;
        rs = qrand() % ((3 + 1) - 0) + 0;
        ox = qrand() % ((3 + 1) - 0) + 0;
        Dr = qrand() % ((3 + 1) - 0) + 0;
        Dg = qrand() % ((3 + 1) - 0) + 0;
        Db = qrand() % ((3 + 1) - 0) + 0;
        emit SecondTabUpdateSignal(degree, rs, ox, Dr, Dg, Db, ServerTimer::GetInstance().Time());
        return;
    }

    if(port->isReadable()){
        rawdata = port->readAll();
        qDebug() << rawdata;
        QRegExp rx("[0-9]*\\:[0-9]*\\,\\s*([0-9]*)\\,\\s*([0-9]*)\\,\\s*([0-9]*\\.[0-9]*)\\,\\s*([0-9]*\\.[0-9]*)\\,\\s*([0-9]*\\.[0-9]*)\\,\\s*([0-9]*\\.[0-9]*)");
        int pos = 0;

        if(!rawdata.contains(rx)) return;

        while((pos = rx.indexIn(rawdata, pos)) != -1){
            degree = rx.cap(2).toDouble();
            rs = rx.cap(1).toDouble();
            ox = rx.cap(6).toDouble();
            Dr = rx.cap(3).toDouble();
            Dg = rx.cap(4).toDouble();
            Db = rx.cap(5).toDouble();
            pos += rx.matchedLength();
        }

        emit SecondTabUpdateSignal(degree, rs, ox, Dr, Dg, Db, ServerTimer::GetInstance().Time());
    }
    else{
        return;
    }
}
