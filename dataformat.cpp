#include "dataformat.h"

DataFormat::DataFormat(qreal _time, qreal _fineD, qreal _coarseD, qreal W_F, qreal W_C) :
    time(_time), fineData(_fineD), coarseData(_coarseD), weightFine(W_F), weightCoarse(W_C)
{

}

DataFormat::DataFormat(){
    DataFormat(0,0,0,0,0);
}
