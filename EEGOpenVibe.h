#ifndef EEGOpenVibe_H
#define EEGOpenVibe_H
#include <QObject>
#include <QThread>
#include <QWaitCondition>

class EEGOpenVibePrivate;

class EEGOpenVibe : public QThread
{
    Q_OBJECT

public:
    static EEGOpenVibe *getInstance(int instance = 1, QWaitCondition *waitCondition = NULL);
    void run(void);
private slots:
    void slotStimulationSentBack(int button, int isOn);
protected slots:
friend EEGOpenVibePrivate;
    void vrpnButton(int button, int isOn);
signals :
    void signalStimulationSent(int button, int isOn);
    void signalStimulationSentBack(int button, int isOn);
private:
    EEGOpenVibe(int instance, QWaitCondition *waitCondition);
    ~EEGOpenVibe();
    EEGOpenVibePrivate *m_Private;
};

#endif // EEGOpenVibe_H
