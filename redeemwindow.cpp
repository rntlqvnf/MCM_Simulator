#include "redeemwindow.h"

RedeemWindow::RedeemWindow() {
    //widget var call
    QGridLayout* mainLayout = new QGridLayout();

    //var call
    degreeGraph = new OnlyFirstGraph(DEGREE, "Degree-Time Graph", "degree graph","Time(s)", "Degree(℃)", 40);
    rsGraph = new OnlyFirstGraph(RS, "RS-Time Graph", "RS graph", "Time(s)", "RS(%)", 100);
    oxGraph = new OnlyFirstGraph(OX, "OX-Time Graph", "oxidation graph", "Time(s)", "Oxidation()", 100);
    contGraph = new ContGraph(CONT, "Contamination-Time Graph", "Dr graph", "Dg graph", "Db graph", "Time(s)", "Contamination(PPM)", 100);
    mainLayout->addWidget(degreeGraph, 0, 0);
    mainLayout->addWidget(contGraph, 0, 1);
    mainLayout->addWidget(rsGraph, 1, 0);
    mainLayout->addWidget(oxGraph, 1, 1);

    this->setLayout(mainLayout);

    connect(this, &RedeemWindow::ChartUpdate, degreeGraph, &OnlyFirstGraph::ResetDataFromQuery);
    connect(this, &RedeemWindow::ChartUpdate, rsGraph, &OnlyFirstGraph::ResetDataFromQuery);
    connect(this, &RedeemWindow::ChartUpdate, oxGraph, &OnlyFirstGraph::ResetDataFromQuery);
    connect(this, &RedeemWindow::ChartUpdate, contGraph, &ContGraph::ResetDataFromQuery);
}

void RedeemWindow::GUIUpdate(qreal degree, qreal rs, qreal ox, qreal Dr, qreal Dg, qreal Db, qreal time) {
    QSqlQuery query;
    query.prepare("INSERT INTO " + degreeGraphTable + " (time, data) VALUES (?, ?)");
    query.bindValue(0, time);
    query.bindValue(1, degree);
    query.exec();

    query.prepare("INSERT INTO " + rsGraphTable + " (time, data) VALUES (?, ?)");
    query.bindValue(0, time);
    query.bindValue(1, rs);
    query.exec();

    query.prepare("INSERT INTO " + oxGraphTable + " (time, data) VALUES (?, ?)");
    query.bindValue(0, time);
    query.bindValue(1, ox);
    query.exec();

    query.prepare("INSERT INTO " + contGraphTable + " (time, Dr, Dg, Db) VALUES (?, ?, ?, ?)");
    query.bindValue(0, time);
    query.bindValue(1, Dr);
    query.bindValue(2, Dg);
    query.bindValue(3, Db);
    query.exec();

    //replot graph
    qreal start;
    if(degreeGraph->GetInterval() > time) start = 0;
    else start = time - degreeGraph->GetInterval();
    emit ChartUpdate(start, degreeGraph->GetInterval());
}
