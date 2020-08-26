#include "graph.h"

/* **Top Level Function **
 * func : remove datas before given time
 * use : to maintain memory */

void RemoveData(QVector<QPointF>& data, QVector<QPointF>& data2, qreal start, qreal end){
    for(int i = 0; i<data.size(); i++){
        if(data[i].x() >= start){
            if(data[i].x() > end) return;
            else {
                data.remove(i);
                data2.remove(i);
                i--;
            }
        }
    }
}

void RemoveData(QVector<QPointF>& data, qreal start, qreal end){
    for(int i = 0; i<data.size(); i++){
        if(data[i].x() >= start){
            if(data[i].x() > end) return;
            else {
                data.remove(i);
                i--;
            }
        }
    }
}

void RemoveData(QVector<QPointF>& data, QVector<QPointF>& data2, QVector<QPointF>& data3, qreal start, qreal end){
    for(int i = 0; i<data.size(); i++){
        if(data[i].x() >= start){
            if(data[i].x() > end) return;
            else {
                data.remove(i);
                data2.remove(i);
                data3.remove(i);
                i--;
            }
        }
    }
}

void AddPointToVectorByQuery(QVector<QPointF>& data, QVector<QPointF>& data2, qreal start, qreal end, GRAPH_TYPE type, INSERT_TYPE iType){
    QSqlQuery query;
    if(type == VT) query.prepare("SELECT time , fine_V , coarse_V FROM " + firstTabTable + " WHERE time BETWEEN ? AND ?");
    else query.prepare("SELECT time , fine_W , coarse_W FROM " + firstTabTable + " WHERE time BETWEEN ? AND ?");
    query.bindValue(0, start);
    query.bindValue(1, end);
    query.exec();
    if(iType == FRONT){
        int i = 0;
        while(query.next()){
            data.insert(i, {query.value(0).toDouble(), query.value(1).toDouble()});
            data2.insert(i, {query.value(0).toDouble(), query.value(2).toDouble()});
            i++;
        }
    }
    else{
        while(query.next()){
            data.append({query.value(0).toDouble(), query.value(1).toDouble()});
            data2.append({query.value(0).toDouble(), query.value(2).toDouble()});
        }
    }
}

void AddPointToVectorByQuery(QVector<QPointF>& data, qreal start, qreal end, TABLE_TYPE tType, INSERT_TYPE iType){
    QSqlQuery query;
    query.prepare("SELECT time , data FROM " + GetTablePath(tType) + " WHERE time BETWEEN ? AND ?");
    query.bindValue(0, start);
    query.bindValue(1, end);
    query.exec();
    if(iType == FRONT){
        int i = 0;
        while(query.next()){
            data.insert(i, {query.value(0).toDouble(), query.value(1).toDouble()});
            i++;
        }
    }
    else{
        while(query.next()){
            data.append({query.value(0).toDouble(), query.value(1).toDouble()});
        }
    }
}

void AddPointToVectorByQuery(QVector<QPointF>& data, QVector<QPointF>& data2, QVector<QPointF>& data3, qreal start, qreal end, INSERT_TYPE iType){
    QSqlQuery query;
    query.prepare("SELECT time , Dr, Dg, Db FROM " + GetTablePath(CONT) + " WHERE time BETWEEN ? AND ?");
    query.bindValue(0, start);
    query.bindValue(1, end);
    query.exec();
    if(iType == FRONT){
        int i = 0;
        while(query.next()){
            data.insert(i, {query.value(0).toDouble(), query.value(1).toDouble()});
            data2.insert(i, {query.value(0).toDouble(), query.value(2).toDouble()});
            data3.insert(i, {query.value(0).toDouble(), query.value(3).toDouble()});
            i++;
        }
    }
    else{
        while(query.next()){
            data.append({query.value(0).toDouble(), query.value(1).toDouble()});
            data2.append({query.value(0).toDouble(), query.value(2).toDouble()});
            data3.append({query.value(0).toDouble(), query.value(3).toDouble()});
        }
    }
}

QString GetTablePath(TABLE_TYPE type){
    if(type == DEGREE) return degreeGraphTable;
    else if (type == RS) return rsGraphTable;
    else if (type == OX) return oxGraphTable;
    else if (type == CONT) return contGraphTable;
    else return NULL;
}

Graph::Graph(QWidget* parent) : QwtPlot(parent) {
    end = start+interval;
    this->setAxisAutoScale(QwtPlot::xBottom,false);
    this->setAxisAutoScale(QwtPlot::yLeft,true);

    firstGraph = new QwtPlotCurve();
    secondGraph = new QwtPlotCurve();

    firstGraph->setSamples(firstData);
    secondGraph->setSamples(secondData);

    QPen pen;
    pen.setColor("black");
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    firstGraph->setPen(pen);
    pen.setColor("blue");
    secondGraph->setPen(pen);

    QFont font("Arial");
    font.setPointSize(11);
    font.setBold(false);
    this->setAxisFont(QwtPlot::xBottom, font);
    this->setAxisFont(QwtPlot::yLeft, font);
    this->axisWidget(QwtPlot::yLeft)->setSpacing(10);
    this->axisWidget(QwtPlot::xBottom)->setSpacing(10);

    firstGraph->setStyle(QwtPlotCurve::CurveStyle::Lines);
    secondGraph->setStyle(QwtPlotCurve::CurveStyle::Lines);
    firstGraph->setTitle("Fine Particles");
    secondGraph->setTitle("Coarse Particles");

    firstGraph->attach(this);
    secondGraph->attach(this);

    this->setMinimumSize(400, 400);
    this->setMaximumSize(400, 400);
}

Graph::~Graph() {}

qreal Graph::GetStartTime(){
    return start;
}

qreal Graph::GetInterval(){
    return interval;
}

void Graph::ClearAll(){
    firstData.clear();
    secondData.clear();
    start = 0;
    end = start + interval;
    this->setAxisScale(QwtPlot::xBottom, 0, end);
}

void Graph::ReplotPastGraph(int lower, int upper){
    ResetDataFromQuery(static_cast<qreal>(lower)/SLIDER_TIME_RATE, static_cast<qreal>(upper - lower)/SLIDER_TIME_RATE);
}

void Graph::HideShowLegend(bool checked){
    if(checked){
        this->insertLegend(new QwtLegend(), QwtPlot::BottomLegend, 0.5);
        QwtLegend* legend = dynamic_cast<QwtLegend*>(this->legend());
        legend->setMaxColumns(2);
        legend->contentsWidget()->layout()->setSpacing(10);
    }
    else{
        this->insertLegend(NULL);
    }
    this->updateLegend();
}

void Graph::HideShowFine(bool checked){
    if(checked){
        firstGraph->attach(this);
    }
    else{
        firstGraph->detach();
    }
    this->updateLegend();
    this->replot();
}

void Graph::HideShowCoarse(bool checked){
    if(checked){
        secondGraph->attach(this);
    }
    else{
        secondGraph->detach();
    }
    this->updateLegend();
    this->replot();
}

VTGraph::VTGraph(QWidget* parent, qreal _dangerValue, qreal _cautionValue, qreal _dangerSlope, qreal _cautionSlope) :
Graph(parent), dangerValue(_dangerValue), cautionValue(_cautionValue), dangerSlope(_dangerSlope), cautionSlope(_cautionSlope) {
    accTime = 0;
    prevEndTime = -1;

    fineSlopeFile = new QFile(logDir +"Slope(Fine).txt");
    fineSlopeFile->open(QFile::WriteOnly|QFile::Text);
    fineFileStream = new QTextStream(fineSlopeFile);
    *fineFileStream << "Time(s)" << "           " << "Slope" << endl << endl;

    coarseSlopeFile = new QFile(logDir + "Slope(Coarse).txt");
    coarseSlopeFile->open(QFile::WriteOnly|QFile::Text);
    coarseFileStream = new QTextStream(coarseSlopeFile);
    *coarseFileStream << "Time(s)" << "           " << "Slope" << endl << endl;

    *fineFileStream << fixed;
    fineFileStream->setRealNumberPrecision(2);
    *coarseFileStream << fixed;
    coarseFileStream->setRealNumberPrecision(2);

    QFont font("Arial");
    font.setPointSize(15);
    font.setBold(true);
    QwtText title("Voltage-Time Graph");
    title.setFont(font);
    this->setTitle(title);

    dangerLine = new QwtPlotCurve("Threshold of Danger");
    cautionLine = new QwtPlotCurve("Threshold of Caution");

    //pen setting
    QPen pen;
    pen.setColor(Qt::red);
    pen.setStyle(Qt::DashDotLine);
    pen.setWidth(3);
    dangerLine->setPen(pen);
    pen.setColor("#FF7F00");
    pen.setStyle(Qt::DotLine);
    cautionLine->setPen(pen);

    //axis setting
    //font setting
    font.setPointSize(11);
    font.setBold(true);
    title.setText("Time  (s)");
    title.setFont(font);
    this->setAxisTitle(QwtPlot::xBottom, title);
    this->setAxisScale(QwtPlot::xBottom, 0, end);
    title.setText("Voltage (V)");
    this->setAxisTitle(QwtPlot::yLeft, title);
    this->setAxisScale(QwtPlot::yLeft, 0, 5);

    //legend
    this->HideShowLegend(true);

    //intial value setting
    dangerLineVector.push_back({0, dangerValue});
    dangerLineVector.push_back({end, dangerValue});
    cautionLineVector.push_back({0, cautionValue});
    cautionLineVector.push_back({end, cautionValue});

    dangerLine->setSamples(dangerLineVector);
    cautionLine->setSamples(cautionLineVector);

    dangerLine->attach(this);
    cautionLine->attach(this);

    this->replot();
}

VTGraph::~VTGraph(){
    fineSlopeFile->close();
    coarseSlopeFile->close();
}

qreal VTGraph::getDanger(){
    return dangerValue;
}

qreal VTGraph::getCaution(){
    return cautionValue;
}

qreal VTGraph::getDangerSlope(){
    return dangerSlope;
}

qreal VTGraph::getCautionSlope(){
    return cautionSlope;
}

void VTGraph::ClearAll(){
    Graph::ClearAll();
    accTime = 0;
    prevEndTime = -1;
    accFineVector.clear();
    accCoarseVector.clear();

    *fineFileStream << "Graph Cleared" << endl << endl;
    *coarseFileStream << "Graph Cleared" << endl << endl;
}

void VTGraph::ResetDataFromQuery(qreal _start, qreal _interval){
    //four cases
    qreal _end = _start + _interval;

    if(start < _start && end < _end){
        RemoveData(firstData, secondData, start, _start);
        AddPointToVectorByQuery(firstData, secondData, end, _end, VT, BACK);
    }
    else if (start < _start && _end < end){
        RemoveData(firstData, secondData, start, _start);
        RemoveData(firstData, secondData, _end, end);
    }
    else if (_start < start && _end < end){
        RemoveData(firstData, secondData, _end, end);
        AddPointToVectorByQuery(firstData, secondData, _start, start, VT, FRONT);
    }
    else{
        firstData.clear();
        secondData.clear();
        AddPointToVectorByQuery(firstData, secondData, _start, _end, VT, BACK);
    }

    start = _start;
    interval = _interval;
    end = _end;

    dangerLineVector.clear();
    cautionLineVector.clear();
    dangerLineVector.push_back({start, dangerValue});
    dangerLineVector.push_back({end, dangerValue});
    cautionLineVector.push_back({start, cautionValue});
    cautionLineVector.push_back({end, cautionValue});

    firstGraph->setSamples(firstData);
    secondGraph->setSamples(secondData);
    dangerLine->setSamples(dangerLineVector);
    cautionLine->setSamples(cautionLineVector);

    this->setAxisScale(QwtPlot::xBottom, start, end);
    this->replot();
}

void VTGraph::HideShowDanger(bool checked){
    if(checked){
        dangerLine->attach(this);
    }
    else{
        dangerLine->detach();
    }
    this->updateLegend();
    this->replot();
}

void VTGraph::HideShowCaution(bool checked){
    if(checked){
        cautionLine->attach(this);
    }
    else{
        cautionLine->detach();
    }
    this->updateLegend();
    this->replot();
}

void VTGraph::CheckSlope(qreal _firstData, qreal _secondData, qreal time){
    if(prevEndTime < 0) {
        prevEndTime = time;
    }
    qreal dx = time - prevEndTime;
    accTime += dx;
    accFineVector.push_back({time, _firstData});
    accCoarseVector.push_back({time, _secondData});
    prevEndTime = time;
    if(accTime >= 1){
        accTime = 0;
        CheckSlopeUt(CalSlope(accFineVector), time, DATA_TYPE::FINE);
        CheckSlopeUt(CalSlope(accCoarseVector), time, DATA_TYPE::COARSE);
    }
}

qreal VTGraph::CalSlope(QVector<QPointF>& dataVector){
    QVector<qreal> meanVector;
    qreal sum = 0;
    int  i = 0;
    for(i = 0; i < dataVector.size() - 1; i++){
        sum += (dataVector[i+1].y() - dataVector[i].y()) / (dataVector[i+1].x() - dataVector[i].x());
    }
    meanVector.push_back(sum / i);

    sum = 0;
    i = 0;
    qreal meanValue_1;
    qreal meanTime_1;
    qreal meanValue_2;
    qreal meanTime_2;
    while(i + 1 < dataVector.size()){
        if(i % 2 == 0){
            meanValue_1 = (dataVector[i].y() + dataVector[i+1].y()) / 2;
            meanTime_1 = (dataVector[i].x() + dataVector[i+1].x()) / 2;
        }
        else{
            meanValue_2 = (dataVector[i].y() + dataVector[i+1].y()) / 2;
            meanTime_2 = (dataVector[i].x() + dataVector[i+1].x()) / 2;
            sum += (meanValue_2 - meanValue_1) / (meanTime_2 - meanTime_1);
        }
        i++;
    }
    meanVector.push_back(sum / i);

    sum = 0;
    i = 0;
    while(i + 2 < dataVector.size()){
        if(i % 2 == 0){
            meanValue_1 = (dataVector[i].y() + dataVector[i+1].y() + dataVector[i+2].y()) / 3;
            meanTime_1 = (dataVector[i].x() + dataVector[i+1].x() + dataVector[i+2].x()) / 3;
        }
        else{
            meanValue_2 = (dataVector[i].y() + dataVector[i+1].y() + dataVector[i+2].y()) / 3;
            meanTime_2 = (dataVector[i].x() + dataVector[i+1].x() + dataVector[i+2].x()) / 3;
            sum += (meanValue_2 - meanValue_1) / (meanTime_2 - meanTime_1);
        }
        i++;
    }
    meanVector.push_back(sum / i);

    dataVector.clear();

    qreal meanSum = 0;
    for(int i = 0; i<meanVector.size(); i++){
        meanSum += meanVector[i];
    }
    return meanSum / meanVector.size();
}

void VTGraph::CheckSlopeUt(qreal slope, qreal time, DATA_TYPE type){
    if(slope >= dangerSlope){
        if(type == FINE) *fineFileStream << time << "            " << slope << endl;
        else *coarseFileStream << time << "            " << slope << endl;
        QMessageBox errorBox;
        errorBox.critical(0, "Warning", "This slope has exceeded the danger level.");
        errorBox.setFixedSize(200, 200);
        errorBox.show();
    }
    else if(slope >= cautionSlope){
        QMessageBox errorBox;
        errorBox.critical(0, "Warning", "This slope has exceeded the caution level.");
        errorBox.setFixedSize(200, 200);
        errorBox.show();
    }
}

void VTGraph::ChangerProperties(qreal _dangerV, qreal _cautionV, qreal _dangerS, qreal _cautionS){
    dangerValue = _dangerV;
    cautionValue = _cautionV;
    dangerSlope = _dangerS;
    cautionSlope = _cautionS;

    dangerLineVector.clear();
    cautionLineVector.clear();
    dangerLineVector.push_back({start, dangerValue});
    dangerLineVector.push_back({end, dangerValue});
    cautionLineVector.push_back({start, cautionValue});
    cautionLineVector.push_back({end, cautionValue});
    dangerLine->setSamples(dangerLineVector);
    cautionLine->setSamples(cautionLineVector);

    replot();
}

WTGraph::WTGraph(QWidget* parent) : Graph(parent){
    QFont font("Arial");
    font.setPointSize(15);
    font.setBold(true);
    QwtText title("Weight-Time Graph");
    title.setFont(font);
    this->setTitle(title);

    font.setPointSize(11);
    font.setBold(true);
    title.setText("Time  (s)");
    title.setFont(font);
    this->setAxisTitle(QwtPlot::xBottom, title);
    this->setAxisScale(QwtPlot::xBottom, 0, end);
    title.setText("Weight of Wear Particles (mg)");
    this->setAxisTitle(QwtPlot::yLeft, title);
    this->setAxisScale(QwtPlot::yLeft, 0, 500);

    this->HideShowLegend(true);
    this->updateLegend();
}

WTGraph::~WTGraph(){}

void WTGraph::ResetDataFromQuery(qreal _start, qreal _interval){
    qreal _end = _start + _interval;

    if(start < _start && end < _end){
        RemoveData(firstData, secondData, start, _start);
        AddPointToVectorByQuery(firstData, secondData, end, _end, WT, BACK);
    }
    else if (start < _start && _end < end){
        RemoveData(firstData, secondData, start, _start);
        RemoveData(firstData, secondData, _end, end);
    }
    else if (_start < start && _end < end){
        RemoveData(firstData, secondData, _end, end);
        AddPointToVectorByQuery(firstData, secondData, _start, start, WT, FRONT);
    }
    else{
        firstData.clear();
        secondData.clear();
        AddPointToVectorByQuery(firstData, secondData, _start, _end, WT, BACK);
    }

    start = _start;
    interval = _interval;
    end = _end;

    firstGraph->setSamples(firstData);
    secondGraph->setSamples(secondData);

    this->setAxisScale(QwtPlot::xBottom, start, end);
    this->replot();
}

void WTGraph::HideShowLegend(bool checked){
    if(checked){
        this->insertLegend(new QwtLegend(), QwtPlot::BottomLegend, 0.5);
        QwtLegend* legend = dynamic_cast<QwtLegend*>(this->legend());
        legend->setMaxColumns(2);
        legend->contentsWidget()->layout()->setSpacing(10);
        legend->contentsWidget()->layout()->addWidget(new QLabel);
        legend->contentsWidget()->layout()->addWidget(new QLabel);
    }
    else{
        this->insertLegend(NULL);
    }
    this->updateLegend();
}



/*********************************
 * for second tab
 * Only Use Fine graph between Two
 * *******************************
 */

OnlyFirstGraph::OnlyFirstGraph(TABLE_TYPE tType, QString _title, QString graphName, QString xTitle, QString yTitle, qreal yEnd) : Graph(NULL){
    graphType = tType;

    secondGraph->detach();

    QFont font("Arial");
    font.setPointSize(15);
    font.setBold(true);
    QwtText title = _title;
    title.setFont(font);
    this->setTitle(title);

    firstGraph->setTitle(graphName);

    font.setPointSize(11);
    font.setBold(true);
    title.setText(xTitle);
    title.setFont(font);
    this->setAxisTitle(QwtPlot::xBottom, title);
    this->setAxisScale(QwtPlot::xBottom, 0, end);

    title.setText(yTitle);
    this->setAxisTitle(QwtPlot::yLeft, title);
    this->setAxisScale(QwtPlot::yLeft, 0, yEnd);

    this->setFixedSize(400,250);
}

void OnlyFirstGraph::ResetDataFromQuery(qreal _start, qreal _interval){
    qreal _end = _start + _interval;

    if(start < _start && end < _end){
        RemoveData(firstData, start, _start);
        AddPointToVectorByQuery(firstData, end, _end, graphType, BACK);
    }
    else if (start < _start && _end < end){
        RemoveData(firstData, start, _start);
        RemoveData(firstData, _end, end);
    }
    else if (_start < start && _end < end){
        RemoveData(firstData, _end, end);
        AddPointToVectorByQuery(firstData, _start, start, graphType, FRONT);
    }
    else{
        firstData.clear();
        AddPointToVectorByQuery(firstData, _start, _end, graphType, BACK);
    }

    start = _start;
    interval = _interval;
    end = _end;
    firstGraph->setSamples(firstData);

    this->setAxisScale(QwtPlot::xBottom, start, end);
    this->replot();
}

QString OnlyFirstGraph::GetGraphType(){
    return GetTablePath(graphType);
}


/*********************************
 * for second tab
 * Three graphs
 * *******************************
 */

ContGraph::ContGraph(TABLE_TYPE tType, QString _title,
                     QString graphName_1, QString graphName_2, QString graphName_3,
                     QString xTitle, QString yTitle, qreal yEnd)
    : OnlyFirstGraph(tType, _title, graphName_1, xTitle, yTitle, yEnd){

    //adding Two graphs
    thirdGraph = new QwtPlotCurve();

    QPen pen;
    pen.setColor("red");
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    thirdGraph->setPen(pen);

    //setting graphs
    secondGraph->setTitle(graphName_2);
    thirdGraph->setTitle(graphName_3);

    thirdGraph->setSamples(thirdData);

    firstGraph->attach(this);
    secondGraph->attach(this);
    thirdGraph->attach(this);
}

void ContGraph::ResetDataFromQuery(qreal _start, qreal _interval){
    qreal _end = _start + _interval;

    if(start < _start && end < _end){
        RemoveData(firstData, secondData, thirdData, start, _start);
        AddPointToVectorByQuery(firstData, secondData, thirdData, end, _end, BACK);
    }
    else if (start < _start && _end < end){
        RemoveData(firstData, secondData, thirdData, start, _start);
        RemoveData(firstData, secondData, thirdData, _end, end);
    }
    else if (_start < start && _end < end){
        RemoveData(firstData, secondData, thirdData, _end, end);
        AddPointToVectorByQuery(firstData, secondData, thirdData, _start, start, FRONT);
    }
    else{
        firstData.clear();
        secondData.clear();
        thirdData.clear();
        AddPointToVectorByQuery(firstData, secondData, thirdData, _start, _end, BACK);
    }

    start = _start;
    interval = _interval;
    end = _end;

    firstGraph->setSamples(firstData);
    secondGraph->setSamples(secondData);
    thirdGraph->setSamples(thirdData);

    this->setAxisScale(QwtPlot::xBottom, start, end);
    this->replot();
}
