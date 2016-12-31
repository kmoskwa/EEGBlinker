#ifndef EEGBLINKER_H
#define EEGBLINKER_H

#include <QThread>
#include "ui_EEGBlinker.h"

class EEGBlinkerPrivate;

class EEGBlinker : public QMainWindow
{
    Q_OBJECT

public:
    EEGBlinker(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~EEGBlinker();

private slots:
    void slotStartButtonClicked(void);
    void slotStopButtonClicked(void);
    void slotDividerChanged(double div);
    void slotStimulationReceived(int button, int isOn);

private:
    Ui::EEGBlinkerClass ui;
    EEGBlinkerPrivate *m_Private;
};

#endif // EEGBLINKER_H
