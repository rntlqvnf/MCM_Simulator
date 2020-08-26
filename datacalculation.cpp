#include "datacalculation.h"

DataCalculation::DataCalculation(){
}

DataCalculation::~DataCalculation(){
}

void DataCalculation::Calculate(qreal fineData, qreal coarseData, qreal time){
    qreal weightFine = 41.62*fineData - 43.57;
    qreal weightCoarse = 118.65*coarseData - 186.87;

    if(weightFine < 0 || fineData < 1.07) weightFine = 0;
    if(weightCoarse < 0 || coarseData < 1.07) weightCoarse = 0;

    emit GUISignal(fineData, coarseData, weightFine, weightCoarse, time);
}
