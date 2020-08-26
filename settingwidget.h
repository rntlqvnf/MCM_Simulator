#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H


#include <QWidget>
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


class SettingWidget : public QWidget {
    Q_OBJECT
public:
    SettingWidget(qreal, qreal, qreal, qreal);
private:
    QPushButton* connectButton;
    QVBoxLayout* connectWidgetMainLayout;
    QHBoxLayout* dangerLayout;
    QHBoxLayout* cautionLayout;
    QLineEdit* dangerInput;
    QLineEdit* cautionInput;
    QLineEdit* slopeInput[2];
    QLabel* dangerLabel;
    QLabel* cautionLabel;
    QLabel* slopeLabel[2];
    QGroupBox* thresholdGroup;
    QGroupBox* slopeGroup;
public slots:
    void SettingEnterClick();
signals:
    void SettingEnterClicked(qreal, qreal, qreal, qreal);
};

#endif // SETTINGWIDGET_H
