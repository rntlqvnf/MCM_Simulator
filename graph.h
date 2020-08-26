#ifndef GRAPH_H
#define GRAPH_H

#include "default.h"
#include <QPainter>
#include <QLayout>
#include <QLabel>
#include <qwt_plot.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_curve.h>
#include <qwt_text.h>
#include <qwt_legend.h>
#include <QPointF>
#include <QVector>
#include <QMessageBox>
#include <QFont>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QEvent>
#include <QWheelEvent>
#include <QTime>

void RemovePoints(QVector<QPointF>&, QVector<QPointF>&, qreal, qreal);
void RemovePoints(QVector<QPointF>&, qreal, qreal);
void RemovePoints(QVector<QPointF>&, QVector<QPointF>&, QVector<QPointF>&, qreal, qreal);
void AddPointToVectorByQuery(QVector<QPointF>&, QVector<QPointF>&,  QVector<QPointF>&, qreal, qreal, INSERT_TYPE);
void AddPointToVectorByQuery(QVector<QPointF>&, QVector<QPointF>&, qreal, qreal, GRAPH_TYPE, INSERT_TYPE);
void AddPointToVectorByQuery(QVector<QPointF>&, qreal, qreal, GRAPH_TYPE, INSERT_TYPE);
QString GetTablePath(TABLE_TYPE type);

class Graph : public QwtPlot {
    Q_OBJECT
public:
    Graph(QWidget*);
    virtual ~Graph();
    void ClearAll();
    qreal GetStartTime();
    qreal GetInterval();
protected:
    double start = 0;
    double interval = 5;
    double end;
    QVector<QPointF> firstData;
    QVector<QPointF> secondData;
    QwtPlotCurve* firstGraph;
    QwtPlotCurve* secondGraph;
public slots:
    void HideShowLegend(bool);
    void HideShowFine(bool);
    void HideShowCoarse(bool);
    void ReplotPastGraph(int, int);
    virtual void ResetDataFromQuery(qreal, qreal) = 0;
};

class VTGraph : public Graph{
    Q_OBJECT
public:
    VTGraph(QWidget*, qreal, qreal, qreal, qreal);
    ~VTGraph() override;
    qreal getDanger();
    qreal getCaution();
    qreal getDangerSlope();
    qreal getCautionSlope();
    void ClearAll();
    void CheckSlope(qreal, qreal, qreal);
private:
    qreal dangerValue;
    qreal cautionValue;
    qreal dangerSlope;
    qreal cautionSlope;
    QVector<QPointF> dangerLineVector;
    QVector<QPointF> cautionLineVector;
    QwtPlotCurve* dangerLine;
    QwtPlotCurve* cautionLine;
    QFile* fineSlopeFile;
    QFile* coarseSlopeFile;
    QTextStream* fineFileStream;
    QTextStream* coarseFileStream;

//for slope checking
private:
    qreal accTime;
    qreal prevEndTime;
    QVector<QPointF> accFineVector;
    QVector<QPointF> accCoarseVector;
    void CheckSlopeUt(qreal, qreal, DATA_TYPE);
    qreal CalSlope(QVector<QPointF>&);
public slots:
    void HideShowDanger(bool);
    void HideShowCaution(bool);
    void ChangerProperties(qreal, qreal, qreal, qreal);
    void ResetDataFromQuery(qreal, qreal) override;
signals:
    void SlopeCautionSound(SOUND_TYPE);
};

class WTGraph : public Graph{
    Q_OBJECT
public:
    WTGraph(QWidget*);
    ~WTGraph() override;
public slots:
    void HideShowLegend(bool);
    void ResetDataFromQuery(qreal, qreal) override;
};

class OnlyFirstGraph : public Graph {
    Q_OBJECT
public:
    OnlyFirstGraph(TABLE_TYPE, QString, QString, QString, QString, qreal);
    QString GetGraphType();
private:
    TABLE_TYPE graphType;
public slots:
    void ResetDataFromQuery(qreal, qreal) override;
};

class ContGraph : public OnlyFirstGraph {
    Q_OBJECT
private:
    QVector<QPointF> thirdData;
    QwtPlotCurve* thirdGraph;

public:
    ContGraph(TABLE_TYPE, QString, QString, QString, QString, QString, QString, qreal);

public slots:
    void ResetDataFromQuery(qreal, qreal) override;
};

#endif // GRAPH_H
