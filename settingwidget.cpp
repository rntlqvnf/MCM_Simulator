#include "settingwidget.h"

SettingWidget::SettingWidget(qreal _dangerV, qreal _cautionV, qreal _dangerS, qreal _cautionS)
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
    dangerLabel = new QLabel();
    cautionLabel = new QLabel();
    for(int i = 0; i<2; i++) slopeLabel[i] = new QLabel();
    QLabel* vLabel = new QLabel("(V)");
    QLabel* vLabel2 = new QLabel("(V)");


    QHBoxLayout* slopeLayout[2];
    //call layout variables
    connectWidgetMainLayout = new QVBoxLayout();
    dangerLayout = new QHBoxLayout();
    cautionLayout = new QHBoxLayout();
    for(int i = 0; i<2; i++) slopeLayout[i] = new QHBoxLayout();

    //set widget propertie
    dangerLabel->setText("<font color=\"black\">Threshold of<\font> <font color = \"red\"> Danger <\font>");
    cautionLabel->setText("<font color=\"black\">Threshold of<\font> <font color = \"#FF7F00\"> Caution <\font>");
    slopeLabel[0]->setText("<font color=\"black\">Slope <\font> <font color = \"red\"> (Danger) <\font>");
    slopeLabel[1]->setText("<font color=\"black\">Slope <\font> <font color = \"#FF7F00\"> (Caution) <\font>");
    dangerLabel->setFont(font);
    cautionLabel->setFont(font);
    for(int i = 0; i<2; i++) slopeLabel[i]->setFont(font);
    font.setBold(false);
    vLabel->setFont(font);
    vLabel2->setFont(font);
    font.setBold(true);
    font.setBold(false);
    connectButton->setFont(font);
    dangerInput->setMaximumWidth(40);
    cautionInput->setMaximumWidth(40);
    for(int i = 0; i<2; i++) slopeInput[i]->setMaximumWidth(40);

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

    connectWidgetMainLayout->setSpacing(10);
    connectWidgetMainLayout->addLayout(dangerLayout);
    connectWidgetMainLayout->addLayout(cautionLayout);
    for(int i = 0; i<2; i++) connectWidgetMainLayout->addLayout(slopeLayout[i]);
    connectWidgetMainLayout->addWidget(connectButton);
    this->setLayout(connectWidgetMainLayout);

    this->setWindowTitle("Properties Setting");

    dangerInput->setText(QString::number(_dangerV));
    cautionInput->setText(QString::number(_cautionV));
    slopeInput[0]->setText(QString::number(_dangerS));
    slopeInput[1]->setText(QString::number(_cautionS));

    connect(connectButton, &QPushButton::clicked, this, &SettingWidget::SettingEnterClick);

    this->show();
}


void SettingWidget::SettingEnterClick(){
    qreal dangerValue = dangerInput->text().toDouble();
    qreal cautionValue = cautionInput->text().toDouble();
    qreal dangerSlope = slopeInput[0]->text().toDouble();
    qreal cautionSlope = slopeInput[1]->text().toDouble();
    emit SettingEnterClicked(dangerValue, cautionValue, dangerSlope, cautionSlope);
    this->close();
}

