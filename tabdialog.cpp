#include "tabdialog.h"

TabDialog::TabDialog(qreal dangerValue, qreal cautionValue, qreal dangerSlope, qreal cautionSlope, QSerialPort* port) : port1(port), port2(port){
    tabWidget = new QTabWidget();

    SettingDataRelatedClasses();
    DatabaseOpen();

    //GUI classes
    tabWidget->addTab(chartWindow = new ChartWindow(dangerValue, cautionValue, dangerSlope, cautionSlope), tr("1"));
    tabWidget->addTab(redeemWindow = new RedeemWindow(), tr("2"));

    //set Properties
    QFont font("Arial");
    font.setPointSize(11);
    font.setBold(false);
    tabWidget->setFont(font);
    tabWidget->setStyleSheet("QTabBar::tab { height: 20px; width: 50px; }");

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(tabWidget);

    this->setLayout(mainLayout);

    /* *****************************
     * *********Connections*********
     * * Flow
     * * 1. Play/Stop button -> Timer start/stop (in recClass)
     * * 2. Timer timeout -> receiveData(recClass)
     * * 3. After receiving Data -> calcuateData(calClass)
     * * 4. After calculating Data -> GUI update; Graph, Traffic Pictures, Sound
     * *****************************
     */

    //1
    connect(chartWindow->UpdatePeriod(), SIGNAL(currentIndexChanged(QString)), &ServerTimer::GetInstance(), SLOT(UpdatePeriodChanged(QString)));
    connect(chartWindow->RunFreButton(), SIGNAL(buttonPressed(int)), &ServerTimer::GetInstance(), SLOT(RunFreChanged(int)));
    ServerTimer::GetInstance().Timer().setInterval(chartWindow->UpdatePeriod()->currentText().toInt());
    //2
    connect(&ServerTimer::GetInstance().Timer(), &QTimer::timeout, recClass, &DataReceive::GetDataAndEmit);
    connect(&ServerTimer::GetInstance().Timer(), &QTimer::timeout, rec2Class, &DataReceive_2::GetDataAndEmit);
    //3
    connect(recClass, SIGNAL(CalculationSignal(qreal, qreal, qreal)), calClass, SLOT(Calculate(qreal, qreal, qreal)));
    //4
    connect(calClass, &DataCalculation::GUISignal, chartWindow, &ChartWindow::GUIUpdate);
    //for errors
    connect(port1, &QSerialPort::errorOccurred, chartWindow, &ChartWindow::SerialError);
    connect(port1, &QIODevice::readChannelFinished, chartWindow, &ChartWindow::PortClosed);

    /***************
     * **************
     * for second tab
     * *****************
     */
    connect(rec2Class, &DataReceive_2::SecondTabUpdateSignal, redeemWindow, &RedeemWindow::GUIUpdate);
}

TabDialog::~TabDialog(){
    port1->close();
}

void TabDialog::SettingDataRelatedClasses(){
    /* *****************************
     * ********Thread Setting*******
     * 1. data receiving
     * 2. data calculation
     * 3. GUI settings (main Thread)
     * 4. Sound Play(especially on Sound Files)
     * 5. graph Drawing
     * *****************************
     */
    calClass = new DataCalculation();
    recClass = new DataReceive(port1);
    rec2Class = new DataReceive_2(port2);

    calClassThread = new QThread();
    recClassThread = new QThread();
    rec2ClassThread = new QThread();

    calClass->moveToThread(calClassThread);
    recClass->moveToThread(recClassThread);
    rec2Class->moveToThread(rec2ClassThread);

    calClassThread->start();
    recClassThread->start();
    rec2ClassThread->start();
}

void TabDialog::DatabaseOpen(){
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    db.open();

    QSqlQuery query;
    query.exec("CREATE TABLE " + firstTabTable + " (time REAL, fine_V REAL, coarse_V REAL, fine_W REAL, coarse_W REAL)");
    query.exec("CREATE TABLE " + degreeGraphTable + " (time REAL, data REAL)");
    query.exec("CREATE TABLE " + rsGraphTable + " (time REAL, data REAL)");
    query.exec("CREATE TABLE " + oxGraphTable + " (time REAL, data REAL)");
    query.exec("CREATE TABLE " + contGraphTable + " (time REAL, Dr REAL, Dg REAL, Db REAL)");
}
