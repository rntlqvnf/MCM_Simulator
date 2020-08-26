#ifndef WIDGET_H
#define WIDGET_H

#include "tabdialog.h"
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QFont>
#include <QGroupBox>
#include <QStringList>
#include <QGridLayout>
#include <QToolTip>


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    QPushButton* connectButton;
    QVBoxLayout* connectWidgetMainLayout;
    QHBoxLayout* dangerLayout;
    QHBoxLayout* cautionLayout;
    QHBoxLayout* portLayout;
    QLineEdit* dangerInput;
    QLineEdit* cautionInput;
    QLineEdit* slopeInput[2];
    QComboBox* portBox;
    QComboBox* portRateBox;
    TabDialog* tabDialog;
    QLabel* dangerLabel;
    QLabel* cautionLabel;
    QLabel* slopeLabel[2];
    QLabel* portLabel;
    QSerialPort* port;
    QGroupBox* thresholdGroup;
    QGroupBox* slopeGroup;
    QGroupBox* portGroup;

public slots:
    void ConnectSerial();
};


#endif // WIDGET_H
