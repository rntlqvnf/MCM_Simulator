#ifndef REDEEMWINDOW_H
#define REDEEMWINDOW_H

#include "graph.h"
#include "datacalculation.h"
#include "datareceive.h"
#include <QWidget>
#include <QGridLayout>
#include <QSqlQuery>

class RedeemWindow : public QWidget {
    Q_OBJECT
public:
    RedeemWindow();
private:
    OnlyFirstGraph* degreeGraph;
    OnlyFirstGraph* rsGraph;
    OnlyFirstGraph* oxGraph;
    ContGraph* contGraph;

public slots:
    void GUIUpdate(qreal, qreal, qreal, qreal, qreal, qreal, qreal);

signals:
    void ChartUpdate(qreal, qreal);
};

#endif // REDEEMWINDOW_H
