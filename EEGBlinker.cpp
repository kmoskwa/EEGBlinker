#include <QPointer>
#include <QPainter>
#include <QTimer>

#include "EEGBlinker.h"
#include "EEGOpenVibe.h"
#include "glwidget.h"


class EEGBlinkerPrivate
  {
  public:
  EEGBlinkerPrivate() : m_Phase(false) {}
  ~EEGBlinkerPrivate() {}
  bool m_Phase;
  QPointer <QTimer> m_Timer;
  QPointer<GLWidget> m_GLwidget;
  void setFreq(double freq)
    {
    if (freq < 0.001)
      {
      freq = 0.001;
      }
    }
  void startTimer(EEGBlinker *mainClass, double freq, double time)
    {
    if (false == m_Timer.isNull())
      {
      delete m_Timer;
      }

    m_Timer      = new QTimer();
    setFreq(freq);
    m_Timer->setInterval(int(time * 1000));
    m_Timer->setSingleShot(true);
    bool ret = false;
    ret = QObject::connect(m_Timer,      SIGNAL(timeout()), mainClass, SLOT(slotStopButtonClicked()));
    Q_ASSERT(ret);
    m_Timer->start();
    }

  void stopTimer()
    {
    delete m_Timer;
    }
  };

EEGBlinker::EEGBlinker(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
  {
  ui.setupUi(this);
  m_Private = new EEGBlinkerPrivate;
  ui.m_SBFreq->setValue(10.0f);
  ui.m_SBFreq->setMinimum(1.0f);
  ui.m_SBTime->setValue(5.0f);
  bool ret = false;
  ret = connect(ui.m_BStart, SIGNAL(clicked()), this, SLOT(slotStartButtonClicked()));
  Q_ASSERT(ret);
  ret = connect(ui.m_BStop,  SIGNAL(clicked()), this, SLOT(slotStopButtonClicked() ));
  Q_ASSERT(ret);
  ret = connect(ui.m_SBFreq, SIGNAL(valueChanged(double)), this, SLOT(slotDividerChanged(double)));
  Q_ASSERT(ret);

  QGridLayout *gridLayout = new QGridLayout(ui.m_Widget);
  gridLayout->setObjectName(QString::fromUtf8("gridLayout1"));
  gridLayout->setContentsMargins(0, 0, 0, 0);

  m_Private->m_GLwidget = new GLWidget(NULL);
  gridLayout->addWidget(m_Private->m_GLwidget);

  QPointer<EEGOpenVibe> m_OpenVibe = EEGOpenVibe::getInstance();

  connect(m_OpenVibe, SIGNAL(signalStimulationSent    (int, int) ),
          this,         SLOT(  slotStimulationReceived(int, int) ),
          Qt::QueuedConnection);

  }

EEGBlinker::~EEGBlinker()
  {
  }

void EEGBlinker::slotStimulationReceived(int button, int isOn)
  {
  if (0 == button)
    {
    if (1 == isOn)
      {
      slotStartButtonClicked();
      }
    else
      {
      slotStopButtonClicked();
      }
    }
  }

void EEGBlinker::slotStartButtonClicked(void)
  {
  m_Private->startTimer(this,ui.m_SBFreq->value(), ui.m_SBTime->value());
  m_Private->m_GLwidget->startBlinking();
  }

void EEGBlinker::slotStopButtonClicked(void)
  {
  m_Private->stopTimer();
  m_Private->m_GLwidget->stopBlinking();
  }

void EEGBlinker::slotDividerChanged(double div)
  {
  if (div < 1)
    {
    div = 1;
    }
  m_Private->m_GLwidget->setBlinkDivider(div);
  }
