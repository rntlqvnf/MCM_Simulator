#include "chartwindow.h"

ChartWindow::ChartWindow(qreal dangerValue, qreal cautionValue, qreal dangerSlope, qreal cautionSlope){
    /* *****************************
     * *******Class Members*********
     * 1. Sound Class
     * 2. Data Calculation/Receiving Class
     * 3. Timer
     * 4. Log File
     * 5. graph
     * *****************************
     */
    soundManageClass = new SoundManage();
    vtGraph = new VTGraph(this, dangerValue, cautionValue, dangerSlope, cautionSlope);
    wtGraph = new WTGraph(this);
    vtGraph->canvas()->installEventFilter(this);
    wtGraph->canvas()->installEventFilter(this);

    //log file setting
    QDir dir;
    if(!dir.exists(logDir))
        dir.mkpath(logDir);

    fineThresholdFile = new QFile(logDir + "Fine_Threshold.txt");
    fineThresholdFile->open(QFile::WriteOnly|QFile::Text);
    fineThresholdLogStream = new QTextStream(fineThresholdFile);
    *fineThresholdLogStream << "Time(s)            " << "Voltage(V)            " << "Weight(mg)            " << endl << endl;
    coarseThresholdFile = new QFile(logDir + "Coarse_Threshold.txt");
    coarseThresholdFile->open(QFile::WriteOnly|QFile::Text);
    coarseThresholdLogStream = new QTextStream(coarseThresholdFile);
    *coarseThresholdLogStream << "Time(s)           " << "Voltage(V)            " << "Weight(mg)            " << endl << endl;
    *fineThresholdLogStream << fixed;
    fineThresholdLogStream->setRealNumberPrecision(2);
    *coarseThresholdLogStream << fixed;
    coarseThresholdLogStream->setRealNumberPrecision(2);


    /* *****************************
     * *********GUI Settings********
     * *****************************
     */

    //widget call
    centerWidget = new QDialog();
    runButton = new QPushButton("Run");
    freezeButton = new QPushButton("Stop");
    runFreButton = new QButtonGroup();
    updateSettingLabel = new QLabel("Time\nInterval (ms)");
    updatePeriod = new QComboBox();
    fineVol = new QLineEdit();
    fineVolLabel = new QLabel("Voltage(V)");
    fineWear = new QLineEdit();
    fineWearLabel = new QLabel("Wear Particles(mg)");
    coarseVol = new QLineEdit();
    coarseVolLabel = new QLabel("Voltage(V)");
    coarseWear = new QLineEdit();
    coarseWearLabel = new QLabel("Wear Particles(mg)");
    fineGroup = new QGroupBox(tr("Fine"));
    coarseGroup = new QGroupBox(tr("Coarse"));
    chartSlider = new QSlider(Qt::Horizontal);

    //layout var call
    mainLayout = new QHBoxLayout();
    settingLayout = new QVBoxLayout();
    chartLayout = new QHBoxLayout();
    dataLayout = new QHBoxLayout();
    trafficLayout = new QVBoxLayout();
    QVBoxLayout* fineLayout = new QVBoxLayout();
    QVBoxLayout* coarseLayout = new QVBoxLayout();
    QVBoxLayout* graphSliderLayout = new QVBoxLayout();
    QHBoxLayout* sliderLayout = new QHBoxLayout();

    //widget setting
    updatePeriod->addItems(QStringList() << "50" << "100" << "200"
                          << "300" << "400" << "500" << "1000");
    runFreButton->addButton(runButton, 1);
    runFreButton->addButton(freezeButton, 0);
    fineVol->setReadOnly(true);
    fineWear->setReadOnly(true);
    coarseVol->setReadOnly(true);
    coarseWear->setReadOnly(true);
    fineVol->setMaximumHeight(30);
    fineWear->setMaximumHeight(30);
    coarseVol->setMaximumHeight(30);
    coarseWear->setMaximumHeight(30);
    updateSettingLabel->setAlignment(Qt::AlignCenter);
    fineVol->setAlignment(Qt::AlignCenter);
    fineWear->setAlignment(Qt::AlignCenter);
    coarseVol->setAlignment(Qt::AlignCenter);
    coarseWear->setAlignment(Qt::AlignCenter);
    fineGroup->setStyleSheet("QGroupBox {border: 2px solid}"
                             "QGroupBox {padding-top: 10px}"
                             "QGroupBox {margin-top: 6px}"
                             "QGroupBox::title {subcontrol-origin: margin}"
                             "QGroupBox::title {padding : 0px 3px 3px 3px}"
                             "QGroupBox::title {left : 10px}"
                             "QGroupBox::title {bottom : 4px}");
    coarseGroup->setStyleSheet("QGroupBox {border: 2px solid}"
                             "QGroupBox {padding-top: 10px}"
                             "QGroupBox {margin-top: 6px}"
                             "QGroupBox::title {subcontrol-origin: margin}"
                             "QGroupBox::title {padding : 0px 3px 3px 3px}"
                             "QGroupBox::title {left : 10px}"
                             "QGroupBox::title {bottom : 4px}");
    fineThresholdLogStream->setRealNumberPrecision(2);
    coarseThresholdLogStream->setRealNumberPrecision(2);
    ResetSlider(0,0,0);
    SettingActions();

    //put icon
    /*
    QPixmap run(":/icon/icon/run.png");
    QPixmap pause(":/icon/icon/pause.png");
    runButton->setIcon(QIcon(run));
    freezeButton->setIcon(QIcon(pause));
    runButton->setIconSize(QSize(20,20));
    freezeButton->setIconSize(QSize(20,20));
    */

    //Setting MaxWidth
    const int settingLayoutMaxWidth = 130;
    runButton->setMinimumWidth(settingLayoutMaxWidth - 10);
    freezeButton->setMinimumWidth(settingLayoutMaxWidth - 10);
    updateSettingLabel->setMaximumWidth(settingLayoutMaxWidth);
    updatePeriod->setMinimumWidth(settingLayoutMaxWidth);
    fineVol->setMaximumWidth(settingLayoutMaxWidth);
    fineWear->setMaximumWidth(settingLayoutMaxWidth);
    coarseVol->setMaximumWidth(settingLayoutMaxWidth);
    coarseWear->setMaximumWidth(settingLayoutMaxWidth);
    fineGroup->setMaximumWidth(140);
    coarseGroup->setMaximumWidth(140);

    //font setting
    QFont font("Arial");
    font.setBold(true);
    font.setPointSize(13);
    runButton->setFont(font);
    freezeButton->setFont(font);
    updateSettingLabel->setFont(font);
    font.setPointSize(10);
    font.setBold(false);
    updatePeriod->setFont(font);
    //setting part
    font.setPointSize(10);
    font.setBold(false);
    fineVolLabel->setFont(font);
    fineWearLabel->setFont(font);
    coarseVolLabel->setFont(font);
    coarseWearLabel->setFont(font);
    font.setPointSize(13);
    fineVol->setFont(font);
    fineWear->setFont(font);
    coarseVol->setFont(font);
    coarseWear->setFont(font);
    font.setBold(true);
    vtGraph->setFont(font);
    wtGraph->setFont(font);
    font.setPointSize(10);
    font.setBold(false);
    menuBar()->setFont(font);
    font.setPointSize(14);
    font.setBold(true);
    fineGroup->setFont(font);
    coarseGroup->setFont(font);

    //layout setting
    chartLayout->addWidget(vtGraph);
    chartLayout->addSpacing(15);
    chartLayout->addWidget(wtGraph);
    chartSlider->hide(); //default is not showing
    sliderLayout->addSpacing(100);
    sliderLayout->addWidget(chartSlider);
    sliderLayout->addSpacing(50);
    graphSliderLayout->addSpacing(20);
    graphSliderLayout->addLayout(chartLayout);
    graphSliderLayout->addLayout(sliderLayout);
    fineLayout->addWidget(fineVolLabel);
    fineLayout->addWidget(fineVol);
    fineLayout->addWidget(fineWearLabel);
    fineLayout->addWidget(fineWear);
    fineGroup->setLayout(fineLayout);
    coarseLayout->addWidget(coarseVolLabel);
    coarseLayout->addWidget(coarseVol);
    coarseLayout->addWidget(coarseWearLabel);
    coarseLayout->addWidget(coarseWear);
    coarseGroup->setLayout(coarseLayout);
    settingLayout->addSpacing(20);
    settingLayout->addWidget(runButton, 1, Qt::AlignCenter);
    settingLayout->addWidget(freezeButton, 1, Qt::AlignCenter);
    settingLayout->addSpacing(20);
    settingLayout->addWidget(updateSettingLabel, 0, Qt::AlignCenter);
    settingLayout->addWidget(updatePeriod, 0 , Qt::AlignCenter);
    settingLayout->addSpacing(30);
    settingLayout->addWidget(fineGroup, 0, Qt::AlignCenter);
    settingLayout->addSpacing(10);
    settingLayout->addWidget(coarseGroup, 0 ,Qt::AlignCenter);
    settingLayout->addSpacing(20);

    //traffic Layouts setting
    QPixmap info(":/trafficLight/traffic/info.png");
    QPixmap green(":/trafficLight/traffic/0.png");
    info = info.scaled({130, 130}, Qt::KeepAspectRatio);
    green = green.scaled({100, 100}, Qt::KeepAspectRatio);
    trafficWidgets[0] = new QLabel();
    trafficWidgets[0]->setPixmap(info);
    trafficWidgets[0]->setAlignment(Qt::AlignCenter);
    trafficWidgets[1] = new QLabel("Fine");
    font.setPointSize(13);
    font.setBold(true);
    trafficWidgets[1]->setFont(font);
    trafficWidgets[1]->setAlignment(Qt::AlignCenter);

    trafficWidgets[2] = new QLabel();
    trafficWidgets[2]->setPixmap(green);
    trafficWidgets[2]->setAlignment(Qt::AlignCenter);

    trafficWidgets[3] = new QLabel("Coarse");
    font.setPointSize(13);
    font.setBold(true);
    trafficWidgets[3]->setFont(font);
    trafficWidgets[3]->setAlignment(Qt::AlignCenter);

    trafficWidgets[4] = new QLabel();
    trafficWidgets[4]->setPixmap(green);
    trafficWidgets[4]->setAlignment(Qt::AlignCenter);

    trafficLayout->addSpacing(30);
    trafficLayout->addWidget(trafficWidgets[0]);
    trafficLayout->addSpacing(10);
    trafficLayout->addWidget(trafficWidgets[1]);
    trafficLayout->addWidget(trafficWidgets[2]);
    trafficLayout->addSpacing(10);
    trafficLayout->addWidget(trafficWidgets[3]);
    trafficLayout->addWidget(trafficWidgets[4]);
    trafficLayout->addSpacing(30);

    //Layout Interaction Setting
    mainLayout->addLayout(settingLayout);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(graphSliderLayout);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(trafficLayout);
    centerWidget->setLayout(mainLayout);
    this->setCentralWidget(centerWidget);

    //for GUI Update, when calculation ended
    connect(this, &ChartWindow::ChartUpdate, vtGraph, &VTGraph::ResetDataFromQuery);
    connect(this, &ChartWindow::ChartUpdate, wtGraph, &WTGraph::ResetDataFromQuery);

    //slider setting
    connect(this, &ChartWindow::EndTimeExpansion, chartSlider, &QSlider::setValue); //by data receive, end time expansion
    connect(chartSlider, &QSlider::valueChanged, this, &ChartWindow::ReplotGraph);
}

ChartWindow::~ChartWindow(){
    GenerateTxtFile();
    fineThresholdFile->close();
    coarseThresholdFile->close();
}

QComboBox* ChartWindow::UpdatePeriod(){
    return updatePeriod;
}

QButtonGroup* ChartWindow::RunFreButton(){
    return runFreButton;
}

void ChartWindow::SettingActions(){
    QMenu* fileMenu = menuBar()->addMenu("&File");
    QMenu* graphMenu = menuBar()->addMenu("&Graph");
    QMenu* helpMenu = menuBar()->addMenu("&Manual");

    //fileMenu Setting
    openTXT = new QAction("Load");
    fileMenu->addAction(openTXT);
    connect(openTXT, SIGNAL(triggered()), this, SLOT(FileOpen()));
    QAction* makeTxt = new QAction("Save");
    fileMenu->addAction(makeTxt);
    connect(makeTxt, &QAction::triggered, this, &ChartWindow::GenerateTxtFile);


    //graphMenu setting
    //vtGraph
    QMenu* vtGraphMenu = new QMenu("Voltage-Time Graph");
    graphMenu->addMenu(vtGraphMenu);
    //graph visibility;
    vtSetting = new QAction("Properties Setting");
    vtGraphMenu->addAction(vtSetting);
    connect(vtSetting, SIGNAL(triggered()), this, SLOT(OpenSettingWidget()));

    QMenu* vtGraphVisibility = new QMenu("Set Visibility");
    vtGraphMenu->addMenu(vtGraphVisibility);
    vtDangerVisible = new QAction("Threshold of Danger");
    vtDangerVisible->setCheckable(true);
    vtDangerVisible->setChecked(true);
    vtGraphVisibility->addAction(vtDangerVisible);
    connect(vtDangerVisible, SIGNAL(triggered(bool)), vtGraph, SLOT(HideShowDanger(bool)));
    vtCautionVisible = new QAction("Threshold of Caution");
    vtCautionVisible->setCheckable(true);
    vtCautionVisible->setChecked(true);
    vtGraphVisibility->addAction(vtCautionVisible);
    connect(vtCautionVisible, SIGNAL(triggered(bool)), vtGraph, SLOT(HideShowCaution(bool)));
    vtFineVisible = new QAction("Fine Data");
    vtFineVisible->setCheckable(true);
    vtFineVisible->setChecked(true);
    vtGraphVisibility->addAction(vtFineVisible);
    connect(vtFineVisible, SIGNAL(triggered(bool)), vtGraph, SLOT(HideShowFine(bool)));
    vtCoarseVisible = new QAction("Coarse Data");
    vtCoarseVisible->setCheckable(true);
    vtCoarseVisible->setChecked(true);
    vtGraphVisibility->addAction(vtCoarseVisible);
    connect(vtCoarseVisible, SIGNAL(triggered(bool)), vtGraph, SLOT(HideShowCoarse(bool)));
    vtLegend = new QAction("Legend");
    vtLegend->setCheckable(true);
    vtLegend->setChecked(true);
    vtGraphVisibility->addAction(vtLegend);
    connect(vtLegend, SIGNAL(triggered(bool)), vtGraph, SLOT(HideShowLegend(bool)));
    QMenu* soundMenu = new QMenu("Sound On/Off");
    vtGraphMenu->addMenu(soundMenu);
    dangerSound = new QAction("Threshold (Danger)");
    cautionSound = new QAction("Threshold (Caution)");
    soundMenu->addAction(dangerSound);
    soundMenu->addAction(cautionSound);
    dangerSound->setCheckable(true);
    dangerSound->setChecked(true);
    cautionSound->setCheckable(true);
    cautionSound->setChecked(true);
    connect(dangerSound, SIGNAL(triggered(bool)), soundManageClass, SLOT(ChangeHighState(bool)));
    connect(cautionSound, SIGNAL(triggered(bool)), soundManageClass, SLOT(ChangeLowState(bool)));

    //wt Graph
    QMenu* wtGraphMenu = new QMenu("Weight-Time Graph");
    graphMenu->addMenu(wtGraphMenu);
    //graph visibility
    QMenu* wtGraphVisibility = new QMenu("Set Visibility");
    wtGraphMenu->addMenu(wtGraphVisibility);
    wtFineVisible = new QAction("Fine Data");
    wtFineVisible->setCheckable(true);
    wtFineVisible->setChecked(true);
    wtGraphVisibility->addAction(wtFineVisible);
    connect(wtFineVisible, SIGNAL(triggered(bool)), wtGraph, SLOT(HideShowFine(bool)));
    wtCoarseVisible = new QAction("Coarse Data");
    wtCoarseVisible->setCheckable(true);
    wtCoarseVisible->setChecked(true);
    wtGraphVisibility->addAction(wtCoarseVisible);
    connect(wtCoarseVisible, SIGNAL(triggered(bool)), wtGraph, SLOT(HideShowCoarse(bool)));
    wtLegend = new QAction("Legend");
    wtLegend->setCheckable(true);
    wtLegend->setChecked(true);
    wtGraphVisibility->addAction(wtLegend);
    connect(wtLegend, SIGNAL(triggered(bool)), wtGraph, SLOT(HideShowLegend(bool)));


    QMenu* PastGraph = new QMenu("Show Past Data");
    graphMenu->addSeparator();
    graphMenu->addMenu(PastGraph);

    pastGraphGroup = new QActionGroup(this);
    vtGraphPastData = new QAction("Voltage-Time Graph");
    wtGraphPastData = new QAction("Weight-Time Graph");
    QAction* sliderMode = new QAction("Slider Mode");
    sliderMode->setCheckable(true);
    sliderMode->setChecked(false);

    pastGraphGroup->addAction(vtGraphPastData);
    pastGraphGroup->addAction(wtGraphPastData);
    PastGraph->addAction(vtGraphPastData);
    PastGraph->addAction(wtGraphPastData);
    //PastGraph->addAction(sliderMode); //slider mode
    connect(pastGraphGroup, SIGNAL(triggered(QAction*)), this, SLOT(PastGraphSelection(QAction*)));
    connect(sliderMode, &QAction::triggered, this, &ChartWindow::SliderModeOnOff);


    QAction* ClearAll = new QAction("Clear Graph");
    graphMenu->addSeparator();
    graphMenu->addAction(ClearAll);
    connect(ClearAll, SIGNAL(triggered()), this, SLOT(ResetGraph()));

    QActionGroup* manualGroup = new QActionGroup(this);
    QAction* manualActions[4];

    //icon implementation
    /*
    openTXT->setIcon(QIcon(":/icon/icon/open.png"));
    makeTxt->setIcon(QIcon(":/icon/icon/save.png"));
    vtGraphMenu->setIcon(QIcon(":/icon/icon/V.png"));
    vtSetting->setIcon(QIcon(":/icon/icon/setting.png"));
    vtGraphVisibility->setIcon(QIcon(":/icon/icon/visible.png"));
    soundMenu->setIcon(QIcon(":/icon/icon/sound.png"));
    wtGraphMenu->setIcon(QIcon(":/icon/icon/W.png"));
    wtGraphVisibility->setIcon(QIcon(":/icon/icon/visible.png"));
    PastGraph->setIcon(QIcon(":/icon/icon/past.png"));
    vtGraphPastData->setIcon(QIcon(":/icon/icon/V.png"));
    wtGraphPastData->setIcon(QIcon(":/icon/icon/W.png"));
    ClearAll->setIcon(QIcon(":/icon/icon/clear.png"));
    */

    //text setting
    manualTexts[0] = "User guide";
    manualTexts[1] = "Main window";
    manualTexts[2] = "Location of data file";
    manualTexts[3] = "Function of buttons";

    //file name setting
    manualFileTexts[0] = "User_guide.pdf";
    manualFileTexts[1] = "Main_window.pdf";
    manualFileTexts[2] = "Location_of_data_file.pdf";
    manualFileTexts[3] = "Function_of_buttons.pdf";

    for(int i = 0 ; i < 4; i++) {
        manualActions[i] = new QAction(manualTexts[i]);
        manualGroup->addAction(manualActions[i]);
        helpMenu->addAction(manualActions[i]);
    }
    connect(manualGroup, &QActionGroup::triggered, this, &ChartWindow::OpenHelp);
}

void ChartWindow::ReplotGraph(int start){
    qreal startAsReal = static_cast<qreal>(start) / SLIDER_TIME_RATE;
    emit ChartUpdate(startAsReal, vtGraph->GetInterval());
}

void ChartWindow::GUIUpdate(qreal fineData, qreal coarseData, qreal weightFine, qreal weightCoarse, qreal time){
    fineVol->setText(QString::number(fineData));
    coarseVol->setText(QString::number(coarseData));
    fineWear->setText(QString::number(weightFine));
    coarseWear->setText(QString::number(weightCoarse));

    QPixmap green(":/trafficLight/traffic/0.png");
    QPixmap yellow(":/trafficLight/traffic/1.png");
    QPixmap red(":/trafficLight/traffic/2.png");
    green = green.scaled({100, 100}, Qt::KeepAspectRatio);
    yellow = yellow.scaled({100, 100}, Qt::KeepAspectRatio);
    red = red.scaled({100, 100}, Qt::KeepAspectRatio);

    //Database writing
    QSqlQuery query;
    query.prepare("INSERT INTO " + firstTabTable + " (time, fine_V, coarse_V, fine_W, coarse_W)"
                  "VALUES (?, ?, ?, ?, ?)");
    query.bindValue(0, time);
    query.bindValue(1, fineData);
    query.bindValue(2, coarseData);
    query.bindValue(3, weightFine);
    query.bindValue(4, weightCoarse);
    query.exec();

    //slider setting
    int prevMaximum = chartSlider->maximum();
    chartSlider->setMaximum(static_cast<int>(time * SLIDER_TIME_RATE));
    if(time > vtGraph->GetInterval())
        chartSlider->setValue(chartSlider->value() + (chartSlider->maximum() - prevMaximum));

    //chart update
    emit ChartUpdate(static_cast<qreal>(chartSlider->value()) / SLIDER_TIME_RATE, vtGraph->GetInterval());

    //traffic update and sound on/off
    qreal dangerValue = vtGraph->getDanger();
    qreal cautionValue = vtGraph->getCaution();
    if(fineData >= dangerValue){
        trafficWidgets[2]->setPixmap(red);
        *fineThresholdLogStream << time << "            " << fineData << "           " << weightFine << endl << endl;

        soundManageClass->SoundPlay(SOUND_TYPE::HIGH);
     }
    else if (fineData>=cautionValue && fineData < dangerValue){
       trafficWidgets[2]->setPixmap(yellow);
       soundManageClass->SoundPlay(SOUND_TYPE::LOW);
    }
    else{
       trafficWidgets[2]->setPixmap(green);
    }

    if(coarseData >= dangerValue){
       trafficWidgets[4]->setPixmap(red);
       *coarseThresholdLogStream << time << "            " << coarseData << "            " << weightCoarse << endl << endl;
       soundManageClass->SoundPlay(SOUND_TYPE::HIGH);
    }
    else if (coarseData>=cautionValue && coarseData < dangerValue){
       trafficWidgets[4]->setPixmap(yellow);
       soundManageClass->SoundPlay(SOUND_TYPE::LOW);
    }
    else{
       trafficWidgets[4]->setPixmap(green);
    }

    vtGraph->CheckSlope(fineData, coarseData, time);
}

void ChartWindow::OpenSettingWidget(){
    SettingWidget* widget = new SettingWidget(vtGraph->getDanger(), vtGraph->getCaution(), vtGraph->getDangerSlope(), vtGraph->getCautionSlope());
    connect(widget, &SettingWidget::SettingEnterClicked, vtGraph, &VTGraph::ChangerProperties);
}

void ChartWindow::SliderModeOnOff(bool state){
    if(state){
        chartSlider->show();
    }
    else{
        chartSlider->hide();
    }
}

void ChartWindow::PastGraphSelection(QAction* action){
    if(action == vtGraphPastData){
        PastGraph(VT);
    }
    else{
        PastGraph(WT);
    }
}

void ChartWindow::PastGraph(GRAPH_TYPE type){
    QDialog* graphFragWidget = new QDialog();
    RangeSlider* slider = new RangeSlider();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    Graph* pastGraph;

    //polymorphism
    qreal endTime = ServerTimer::GetInstance().Time();
    if(type == VT) {
        graphFragWidget->setWindowTitle("V-T Graph");
        pastGraph = new VTGraph(this ,vtGraph->getDanger(), vtGraph->getCaution(), 0, 0);
    }
    else {
        graphFragWidget->setWindowTitle("W-T Graph");
        pastGraph = new WTGraph(this);
    }
    pastGraph->installEventFilter(this);

    slider->setMinimum(0);
    slider->setMaximum(static_cast<int>(endTime * SLIDER_TIME_RATE));
    slider->setLowerValue(0);
    if(endTime > GRAPH_X_LIMITATION) {
        pastGraph->ResetDataFromQuery(0, GRAPH_X_LIMITATION);
        slider->setUpperValue(static_cast<int>(GRAPH_X_LIMITATION * SLIDER_TIME_RATE));
    }
    else {
        pastGraph->ResetDataFromQuery(0, endTime);
        slider->setUpperValue(static_cast<int>(endTime * SLIDER_TIME_RATE));
    }

    connect(this, &ChartWindow::PastChartUpdate, pastGraph, &Graph::ResetDataFromQuery);
    connect(slider, &RangeSlider::valueChanged, pastGraph, &Graph::ReplotPastGraph);
    connect(this, &ChartWindow::UpperSliderValueChange, slider, &RangeSlider::setUpperValue);
    connect(this, &ChartWindow::LowerSliderValueChange, slider, &RangeSlider::setLowerValue);

    mainLayout->addWidget(pastGraph);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(slider);
    graphFragWidget->setLayout(mainLayout);
    graphFragWidget->setFixedSize(450,450);
    graphFragWidget->show();
}

void ChartWindow::GenerateTxtFile(){
    //Get all Count
    QSqlQuery query;
    query.exec("SELECT Count(*) FROM " + firstTabTable);
    query.next();
    int count = query.value(0).toInt();

    if(count == 0) return;

    //making path
    QString dirPath  = logDir + QDate::currentDate().toString("yyyy-MM-dd") + "/";
    QDir dir;
    if(!dir.exists(dirPath))
        dir.mkpath(dirPath);

    //making File
    QString txtPath = dirPath + QTime::currentTime().toString("hh_mm") + ".txt";
    QFile logTxt(txtPath);
    logTxt.open(QFile::WriteOnly|QFile::Text);
    QTextStream logStream(&logTxt);

    //make dialog
    QProgressDialog* dialog = new QProgressDialog("Generating File...", "Cancel", 0, count);
    dialog->setValue(-1);
    dialog->setCancelButton(NULL);
    dialog->setAutoClose(false);
    dialog->setAutoReset(false);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);

    QFont font("Arial");
    font.setBold(false);
    font.setPointSize(9);
    dialog->setFont(font);

    connect(this, &ChartWindow::UpdateProgress, dialog, &QProgressDialog::setValue);

    dialog->show();

    query.prepare("SELECT time, fine_V, coarse_V, fine_W, coarse_W FROM " + firstTabTable);
    query.exec();

    while(query.next()){
        logStream << query.value(0).toDouble() << " " << query.value(1).toDouble() << " "
                  << query.value(2).toDouble() << " " << query.value(3).toDouble() << " "
                  << query.value(4).toDouble() << endl;
        emit UpdateProgress(dialog->value() + 1);
        QApplication::processEvents();
    }
    emit UpdateProgress(count);

    logTxt.close();
}

void ChartWindow::FileOpen(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Log Txt File"), logDir);
    if (!fileName.isEmpty()){
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
            return;
        }
        QTextStream stream(&file);

        //Clear Datas
        ResetGraph();

        //make dialog
        const auto size = file.size();
        QProgressDialog* dialog = new QProgressDialog("Loading File...", "Cancel", 0, size);
        dialog->setValue(-1);
        dialog->setCancelButton(NULL);
        dialog->setAutoClose(false);
        dialog->setAutoReset(false);
        dialog->setAttribute(Qt::WA_DeleteOnClose, true);

        QFont font("Arial");
        font.setBold(false);
        font.setPointSize(9);
        dialog->setFont(font);

        connect(this, &ChartWindow::UpdateProgress, dialog, &QProgressDialog::setValue);

        dialog->show();

        //read value
        QSqlQuery query;
        qreal time = 0;
        while(!stream.atEnd()){
            QString line = stream.readLine();
            QRegExp rx("\\d+\\.?\\d*\\s\\d+\\.?\\d*\\s\\d+\\.?\\d*\\s\\d+\\.?\\d*\\s\\d+\\.?\\d*");
            if(!line.contains(rx)) {
                dialog->close();
                file.close();
                QMessageBox errorBox;
                errorBox.critical(0, "Error", "Invalid Text File");
                errorBox.exec();
                return;
            }
            QStringList dataSplit = line.split(" ");
            //format : time fine coarse w_fine w_coarse
            time = dataSplit[0].toDouble();
            qreal fineData = dataSplit[1].toDouble();
            qreal coarseData = dataSplit[2].toDouble();
            qreal weightFine = dataSplit[3].toDouble();
            qreal weightCoarse = dataSplit[4].toDouble();

            query.prepare("INSERT INTO " + firstTabTable + "(time, fine_V, coarse_V, fine_W, coarse_W)"
                          "VALUES (?, ?, ?, ?, ?)");
            query.bindValue(0, time);
            query.bindValue(1, fineData);
            query.bindValue(2, coarseData);
            query.bindValue(3, weightFine);
            query.bindValue(4, weightCoarse);
            query.exec();

            auto remains = file.bytesAvailable();
            auto progress = ((size - remains) * 100) / size;
            emit UpdateProgress(progress);
            QApplication::processEvents();
        }
        emit UpdateProgress(size);
        ServerTimer::GetInstance().SetTime(time);

        qreal Interval = vtGraph->GetInterval();
        qreal startTime = time - Interval;
        if(startTime < 0) startTime = 0;
        emit ChartUpdate(startTime, Interval);

        ResetSlider(0, static_cast<int>((startTime+Interval) * SLIDER_TIME_RATE), static_cast<int> (startTime * SLIDER_TIME_RATE));

        file.close();
    }
}

void ChartWindow::ClearAllData(){
    vtGraph->ClearAll();
    wtGraph->ClearAll();
    QSqlQuery query;
    query.exec("DELETE FROM " + firstTabTable);
}

void ChartWindow::ResetGraph(){
    ClearAllData();
    ServerTimer::GetInstance().ResetTime();
    vtGraph->ResetDataFromQuery(0, 0+vtGraph->GetInterval());
    wtGraph->ResetDataFromQuery(0, 0+vtGraph->GetInterval());
    ResetSlider(0,0,0);
}

void ChartWindow::OpenHelp(QAction* action){
    QString text = action->text();
    int i = 0;
    while(true){
        if(text == manualTexts[i]){
            QDesktopServices::openUrl(QUrl("file:///" + QApplication::applicationDirPath() + "/utility/" + manualFileTexts[i]));
            return;
        }
        else i++;
    }
}

void ChartWindow::SerialError(QSerialPort::SerialPortError errorCode){
    if(errorCode != 0){
        switchButton->setValue(false);
        QString errorText = "Serial Error !\n   code : " + QString::number(errorCode);
        QMessageBox errorBox;
        errorBox.critical(0, "Error!", errorText);
        errorBox.setFixedSize(200, 200);
        errorBox.exec();
    }
}

void ChartWindow::PortClosed(){
    //if(!port->isOpen()){
        switchButton->setValue(false);
        QMessageBox errorBox;
        errorBox.critical(0, "Error!", "Port Closed. Please check port connection.");
        errorBox.setFixedSize(200, 200);
        connect(&errorBox, &QMessageBox::buttonClicked, this, &ChartWindow::PortClosed);
        errorBox.exec();
    //}
}

void ChartWindow::ResetSlider(int min, int max, int value){
    chartSlider->setMinimum(min);
    chartSlider->setMaximum(max);
    chartSlider->setValue(value);
}

bool ChartWindow::eventFilter(QObject *target, QEvent *event){
    if(event->type() == QEvent::Wheel){
        if(target == vtGraph->canvas() || target == wtGraph->canvas()){
            QWheelEvent *w = static_cast<QWheelEvent *>(event);
            qreal newInterval, newStart, end = vtGraph->GetStartTime() + vtGraph->GetInterval();
            if(w->delta() > 0){
                newInterval = vtGraph->GetInterval() - WHEEL_ZOOM_RATE;
                newStart = vtGraph->GetStartTime() + WHEEL_ZOOM_RATE/2;
            }
            else{
                newInterval = vtGraph->GetInterval() + WHEEL_ZOOM_RATE;
                newStart = vtGraph->GetStartTime() - WHEEL_ZOOM_RATE/2;
            }
            if(newInterval < 0) newInterval = 0;
            else if(newInterval > GRAPH_X_LIMITATION) newInterval = GRAPH_X_LIMITATION;

            if(newStart < 0) newStart = 0;
            else if(newStart > end) newStart = end;
            emit ChartUpdate(newStart, newInterval);
            return true;
        }
        else {
            QWheelEvent *w = static_cast<QWheelEvent *>(event);
            Graph* graph = dynamic_cast<Graph*>(target);

            qreal newInterval, newStart, end = graph->GetStartTime() + graph->GetInterval();
            if(w->delta() > 0){
                newInterval = graph->GetInterval() - WHEEL_ZOOM_RATE;
                newStart = graph->GetStartTime() + WHEEL_ZOOM_RATE/2;
            }
            else{
                newInterval = graph->GetInterval() + WHEEL_ZOOM_RATE;
                newStart = graph->GetStartTime() - WHEEL_ZOOM_RATE/2;
            }
            if(newInterval < 0) newInterval = 0;
            else if(newInterval > GRAPH_X_LIMITATION) newInterval = GRAPH_X_LIMITATION;

            if(newStart < 0) newStart = 0;
            else if(newStart > end) newStart = end;

            emit PastChartUpdate(newStart, newInterval);
            emit UpperSliderValueChange(static_cast<int>((newStart + newInterval) * SLIDER_TIME_RATE));
            emit LowerSliderValueChange(static_cast<int>((newStart) * SLIDER_TIME_RATE));
            return true;
        }
    }
    return false;
}
