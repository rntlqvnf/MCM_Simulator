#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include "default.h"
#include "datacalculation.h"
#include "datareceive.h"
#include "soundmanage.h"
#include "switchbutton.h"
#include "dataformat.h"
#include "settingwidget.h"
#include "RangeSlider.h"
#include "graph.h"
#include "servertimer.h"
#include <QThread>
#include <QWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QComboBox>
#include <QtSerialPort/QSerialPort>
#include <QTextEdit>
#include <QFont>
#include <QPen>
#include <QPixmap>
#include <QLineEdit>
#include <QSound>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QMenuBar>
#include <QTabBar>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include <QLineEdit>
#include <QPointF>
#include <QMessageBox>
#include <QStringList>
#include <QActionGroup>
#include <QFileDialog>
#include <QDesktopServices>
#include <QApplication>
#include <QtSql>
#include <QSqlQuery>
#include <QSlider>
#include <QTime>
#include <QDate>
#include <QDir>
#include <QWheelEvent>
#include <QProgressDialog>

class ChartWindow : public QMainWindow{
    Q_OBJECT
public:
    ChartWindow(qreal, qreal, qreal, qreal);
    ~ChartWindow();

    /*
     * **Widget Members**
     * **Including Layout**
     */
private:
    QDialog* centerWidget;
    QPushButton* runButton;
    QPushButton* freezeButton;
    QButtonGroup* runFreButton;
    QLabel* updateSettingLabel;
    QComboBox* updatePeriod;
    QLabel* fineVolLabel;
    QLineEdit* fineVol;
    QLabel* fineWearLabel;
    QLineEdit* fineWear;
    QLabel* coarseVolLabel;
    QLineEdit* coarseVol;
    QLabel* coarseWearLabel;
    QLineEdit* coarseWear;
    QLabel* trafficWidgets[5]; //For Traffic Layout - all QLabel so declare in Array
    QGroupBox* fineGroup;
    QGroupBox* coarseGroup;
    QSlider* chartSlider;
    SwitchButton* switchButton;

    //Layouts
    QHBoxLayout* mainLayout;
    QVBoxLayout* settingLayout;
    QVBoxLayout* graphicsLayout;
    QHBoxLayout* chartLayout;
    QHBoxLayout* dataLayout;
    QVBoxLayout* trafficLayout;

    /*
     * **Members Related to GUI**
     * 1. Graph
     * 2. Sound
     */
private:
    VTGraph* vtGraph;
    WTGraph* wtGraph;
    SoundManage* soundManageClass;

    //for log

private:
    QFile* fineThresholdFile;
    QTextStream* fineThresholdLogStream;
    QFile* coarseThresholdFile;
    QTextStream* coarseThresholdLogStream;

    /*
     * Setting Actions Function
     */
private:

    void SettingActions();
    QAction* openTXT;
    QActionGroup* pastGraphGroup;
    QAction* vtSetting;
    QAction* vtGraphPastData;
    QAction* wtGraphPastData;
    QAction* vtDangerVisible;
    QAction* vtCautionVisible;
    QAction* vtFineVisible;
    QAction* vtCoarseVisible;
    QAction* wtFineVisible;
    QAction* wtCoarseVisible;
    QAction* vtLegend;
    QAction* wtLegend;
    QAction* dangerSound;
    QAction* cautionSound;
    QString manualTexts[4];
    QString manualFileTexts[4];
private:
    QLineEdit* dangerInput;
    QLineEdit* cautionInput;

public:
    void ClearAllData();
    bool eventFilter(QObject *target, QEvent *event) override;
    QComboBox* UpdatePeriod();
    QButtonGroup* RunFreButton();

private:
    void ResetSlider(int, int, int);
public slots:
    void OpenSettingWidget();
    void GUIUpdate(qreal, qreal, qreal, qreal, qreal);
    void FileOpen();
    void OpenHelp(QAction*);
    void ResetGraph();
    void SerialError(QSerialPort::SerialPortError);
    void ReplotGraph(int);
    void PortClosed();
    void PastGraph(GRAPH_TYPE);
    void PastGraphSelection(QAction*);
    void SliderModeOnOff(bool);
    void GenerateTxtFile();
signals:
    void ChangeDangerValue(qreal);
    void ChangeCautionValue(qreal);
    void EndTimeExpansion(int); //by data recieve, end time expansion
    void ChartUpdate(qreal, qreal);
    void PastChartUpdate(qreal, qreal);
    void UpdateProgress(int);
    void UpperSliderValueChange(int);
    void LowerSliderValueChange(int);
};

#endif // CHARTWINDOW_H
