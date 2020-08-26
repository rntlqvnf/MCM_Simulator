#ifndef TABDIALOG_H
#define TABDIALOG_H

#include "chartwindow.h"
#include "redeemwindow.h"
#include "servertimer.h"
#include <QFont>
#include <QDialog>
#include <QTabWidget>
#include <QVBoxLayout>

class TabDialog : public QDialog
{
public:
    TabDialog(qreal, qreal, qreal, qreal,QSerialPort*);
    ~TabDialog();
private:
    QTabWidget* tabWidget;
    QVBoxLayout* mainLayout;
    ChartWindow* chartWindow;
    RedeemWindow* redeemWindow;

private:
    //Thread setting for data recieve / calculate
    QSerialPort* port1;
    QSerialPort* port2;

    QThread* recClassThread;
    QThread* calClassThread;
    QThread* rec2ClassThread;

    DataCalculation* calClass;
    DataReceive* recClass;
    DataReceive_2* rec2Class;

    void SettingDataRelatedClasses();
    void DatabaseOpen();
};

#endif // TABDIALOG_H
