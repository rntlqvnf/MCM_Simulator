#ifndef DATAFORMAT_H
#define DATAFORMAT_H


#include <QWidget>

class DataFormat
{
public:
    DataFormat(qreal, qreal, qreal, qreal, qreal);
    DataFormat();
    qreal time, fineData, coarseData, weightFine, weightCoarse;
};

#endif // DATAFORMAT_H
