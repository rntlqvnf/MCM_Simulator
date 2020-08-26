#ifndef DATACALCULATION_H
#define DATACALCULATION_H

#include "default.h"
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QSqlQuery>
#include <QSqlDatabase>

class DataCalculation : public QObject{
    Q_OBJECT
public:
    DataCalculation();
    ~DataCalculation();

public slots:
    void Calculate(qreal, qreal, qreal);
signals:
    void GUISignal(qreal, qreal, qreal, qreal, qreal);
};

#endif // DATACALCULATION_H
