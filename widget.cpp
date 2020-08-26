#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //font setting
    QFont font("Arial");
    font.setPointSize(10);
    font.setBold(true);

    //GUI setting
    //call widget variables
    connectButton = new QPushButton("Set");
    dangerInput = new QLineEdit();
    cautionInput = new QLineEdit();
    for(int i = 0; i<2; i++) slopeInput[i] = new QLineEdit();
    portBox = new QComboBox();
    dangerLabel = new QLabel();
    cautionLabel = new QLabel();
    for(int i = 0; i<2; i++) slopeLabel[i] = new QLabel();
    portLabel = new QLabel("Port Name");
    QLabel* vLabel = new QLabel("(V)");
    QLabel* vLabel2 = new QLabel("(V)");


    QHBoxLayout* slopeLayout[2];
    //call layout variables
    connectWidgetMainLayout = new QVBoxLayout();
    dangerLayout = new QHBoxLayout();
    cautionLayout = new QHBoxLayout();
    portLayout = new QHBoxLayout();
    for(int i = 0; i<2; i++) slopeLayout[i] = new QHBoxLayout();

    //set widget propertie
    dangerLabel->setText("<font color=\"black\">Threshold of<\font> <font color = \"red\"> Danger <\font>");
    cautionLabel->setText("<font color=\"black\">Threshold of<\font> <font color = \"#FF7F00\"> Caution <\font>");
    slopeLabel[0]->setText("<font color=\"black\">Slope <\font> <font color = \"red\"> (Danger) <\font>");
    slopeLabel[1]->setText("<font color=\"black\">Slope <\font> <font color = \"#FF7F00\"> (Caution) <\font>");
    portBox->addItems(QStringList() << "COM1" << "COM2" << "COM3" << "COM4" << "COM5" << "COM6" << "COM7" << "COM8" << "COM9");
    dangerLabel->setFont(font);
    cautionLabel->setFont(font);
    for(int i = 0; i<2; i++) slopeLabel[i]->setFont(font);
    font.setBold(false);
    vLabel->setFont(font);
    vLabel2->setFont(font);
    portBox->setFont(font);
    font.setBold(true);
    portLabel->setFont(font);
    font.setBold(false);
    connectButton->setFont(font);
    dangerInput->setMaximumWidth(40);
    cautionInput->setMaximumWidth(40);
    for(int i = 0; i<2; i++) slopeInput[i]->setMaximumWidth(40);
    portBox->setMaximumWidth(65);

    dangerInput->setAlignment(Qt::AlignCenter);
    cautionInput->setAlignment(Qt::AlignCenter);
    for(int i =0; i<2; i++)slopeInput[i]->setAlignment(Qt::AlignCenter);

    //layout push
    dangerLayout->addWidget(dangerLabel);
    dangerLayout->addSpacing(20);
    dangerLayout->addWidget(dangerInput);
    dangerLayout->addWidget(vLabel);

    cautionLayout->addWidget(cautionLabel);
    cautionLayout->addSpacing(17);
    cautionLayout->addWidget(cautionInput);
    cautionLayout->addWidget(vLabel2);

    for(int i = 0; i<2; i++){
        slopeLayout[i]->addWidget(slopeLabel[i]);
        slopeLayout[i]->addSpacing(23);
        slopeLayout[i]->addWidget(slopeInput[i]);
        slopeLayout[i]->addSpacing(27);
    }

    portLayout->addWidget(portLabel);
    portLayout->addWidget(portBox);
    connectWidgetMainLayout->setSpacing(10);
    connectWidgetMainLayout->addLayout(dangerLayout);
    connectWidgetMainLayout->addLayout(cautionLayout);
    for(int i = 0; i<2; i++) connectWidgetMainLayout->addLayout(slopeLayout[i]);
    connectWidgetMainLayout->addLayout(portLayout);
    connectWidgetMainLayout->addWidget(connectButton);
    this->setLayout(connectWidgetMainLayout);

    this->setWindowTitle("MCM's Set up");

    //connections
    connect(connectButton, SIGNAL(clicked()), this, SLOT(ConnectSerial()));
}

Widget::~Widget()
{
}

void Widget::ConnectSerial(){
    //serial setting
    port = new QSerialPort();
    port->setPortName(portBox->currentText());
    port->setBaudRate(QSerialPort::Baud19200);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);

    //open
    if(!port->open(QIODevice::ReadOnly)){
        QMessageBox errorBox;
        errorBox.setWindowTitle("Error!");
        errorBox.setInformativeText("Serial Port error");
        errorBox.setStandardButtons(QMessageBox::Close);
        errorBox.exec();
    }

    else{
        qreal dangerValue = dangerInput->text().toFloat();
        qreal cautionValue = cautionInput->text().toFloat();
        qreal dangerSlope = slopeInput[0]->text().toFloat();
        qreal cautionSlope = slopeInput[1]->text().toFloat();
        if(dangerValue < cautionValue){
            QMessageBox errorBox;
            errorBox.critical(0, "Error!", "Serial Port Error!");
            errorBox.show();
            return;
        }
        tabDialog = new TabDialog(dangerValue, cautionValue, dangerSlope, cautionSlope, port);
        this->hide();
        tabDialog->show();
        tabDialog->exec();
    }
}
