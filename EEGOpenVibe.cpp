#include <QPointer>
#include <QPainter>
#include <QTimer>
#include <QMutex>
#include <QWaitCondition>

#include "EEGOpenVibe.h"

#include <iostream>
 
#include <vrpn_Button.h>
#include <vrpn_Analog.h>
#if 1


#endif
class EEGOpenVibePrivate
  {
  public:
  EEGOpenVibePrivate() : m_instanceNumber(0), m_WaitCondition(NULL) {}
  ~EEGOpenVibePrivate() {}
  int m_instanceNumber;
  QWaitCondition *m_WaitCondition;
  static QPointer<EEGOpenVibe> m_instance1;
  static QPointer<EEGOpenVibe> m_instance2;

  static void VRPN_CALLBACK EEGOpenVibePrivate::vrpn_button_callback(void* user_data, vrpn_BUTTONCB button)
    {
    if (button.button == 1)
      {
      *(bool*)user_data = false;
      }
    if (button.button == 0)
      {
      if (false == m_instance2.isNull())
        {
        m_instance2->vrpnButton(button.button, button.state);
        }
      }
    /*
    std::cout << "Button ID : " << button.button << " / Button State : " << button.state << std::endl;
    */
    }
 
  static void VRPN_CALLBACK EEGOpenVibePrivate::vrpn_analog_callback(void* user_data, vrpn_ANALOGCB analog)
    {
    /*
    for (int i = 0; i < analog.num_channel; i++)
      {
      std::cout << "Analog Channel : " << i << " / Analog Value : " << analog.channel[i] << std::endl;
      }
    */
    }
  };

QPointer<EEGOpenVibe> EEGOpenVibePrivate::m_instance1;
QPointer<EEGOpenVibe> EEGOpenVibePrivate::m_instance2;

EEGOpenVibe *EEGOpenVibe::getInstance(int instance, QWaitCondition *waitCondition)
  {
  if (1 == instance)
    {
    EEGOpenVibe *result = EEGOpenVibePrivate::m_instance1;
    if (true == EEGOpenVibePrivate::m_instance1.isNull())
      {
      // Create first instance
      QMutex mutex;
      QWaitCondition started;
      result = new EEGOpenVibe(1, &started);
      EEGOpenVibePrivate::m_instance1 = result;
      result->start(QThread::InheritPriority);
      connect(result, SIGNAL(finished()), result, SLOT(deleteLater()), Qt::UniqueConnection);
      // Wait for thread's start.
      mutex.lock();  
      started.wait(&mutex);
      mutex.unlock();
      }
    return result;
    }
  else
    {
    if (EEGOpenVibePrivate::m_instance2 == NULL)
      {
      // Create second instance
      EEGOpenVibePrivate::m_instance2 = new EEGOpenVibe(2, waitCondition);
      }
    return EEGOpenVibePrivate::m_instance2;
    }
  /*
  EEGOpenVibe *result = NULL;
  switch (instance)
    {
    case 1:
      {
      if (true == EEGOpenVibePrivate::m_instance1.isNull())
        {
        EEGOpenVibePrivate::m_instance1 = new EEGOpenVibe(instance);
        }
      result = EEGOpenVibePrivate::m_instance1;
      }
      break;
    case 2:
      {
      if (true == EEGOpenVibePrivate::m_instance2.isNull())
        {
        EEGOpenVibePrivate::m_instance2 = new EEGOpenVibe(instance);
        }
      result = EEGOpenVibePrivate::m_instance2;
      }
      break;
    default:
      {
      // intentional
      }
    }
  return result;
  */
  }

EEGOpenVibe::EEGOpenVibe(int instance, QWaitCondition *waitCondition)
  {
  m_Private = new EEGOpenVibePrivate();
  m_Private->m_instanceNumber = instance;
  m_Private->m_WaitCondition = waitCondition;

  if (m_Private->m_instanceNumber == 1)
    {
    return;
    }

  EEGOpenVibe *first = EEGOpenVibe::getInstance();
  connect(this, SIGNAL(signalStimulationSentBack(int, int) ),
          first,  SLOT(  slotStimulationSentBack(int, int) ),
          Qt::QueuedConnection);


}

EEGOpenVibe::~EEGOpenVibe()
  {
  }


void EEGOpenVibe::run(void)
  {
#if 1
  if(m_Private->m_instanceNumber == 1)
    {
    EEGOpenVibe *instance2 = EEGOpenVibe::getInstance(2, m_Private->m_WaitCondition);
    instance2->run();
    exec();
    delete EEGOpenVibePrivate::m_instance2;
    }
  else
    {
    m_Private->m_WaitCondition->wakeAll();
    /* flag used to stop the program execution */
    bool running = true;
 
    /* VRPN Button object */
    vrpn_Button_Remote* VRPNButton;
 
    /* Binding of the VRPN Button to a callback */
    VRPNButton = new vrpn_Button_Remote( "openvibe-vrpn@localhost" );
    VRPNButton->register_change_handler( &running, EEGOpenVibePrivate::vrpn_button_callback );
 
    /* VRPN Analog object */
    vrpn_Analog_Remote* VRPNAnalog;
 
    /* Binding of the VRPN Analog to a callback */
    VRPNAnalog = new vrpn_Analog_Remote( "openvibe-vrpn@localhost" );
    VRPNAnalog->register_change_handler( NULL, EEGOpenVibePrivate::vrpn_analog_callback );
 
    /* The main loop of the program, each VRPN object must be called in order to process data */
    while (running)
      {
      VRPNButton->mainloop();
      }
    }
#endif
  }

void EEGOpenVibe::slotStimulationSentBack(int button, int isOn)
  {
  emit signalStimulationSent(button, isOn);
  }

void EEGOpenVibe::vrpnButton(int button, int isOn)
  {
  emit signalStimulationSentBack(button, isOn); 
  }

#if 0
int main(int argc, char** argv)
{
    /* flag used to stop the program execution */
    bool running = true;
 
    /* VRPN Button object */
    vrpn_Button_Remote* VRPNButton;
 
    /* Binding of the VRPN Button to a callback */
    VRPNButton = new vrpn_Button_Remote( "openvibe_vrpn_button@localhost" );
    VRPNButton->register_change_handler( &running, vrpn_button_callback );
 
    /* VRPN Analog object */
    vrpn_Analog_Remote* VRPNAnalog;
 
    /* Binding of the VRPN Analog to a callback */
    VRPNAnalog = new vrpn_Analog_Remote( "openvibe_vrpn_analog@localhost" );
    VRPNAnalog->register_change_handler( NULL, vrpn_analog_callback );
 
    /* The main loop of the program, each VRPN object must be called in order to process data */
    while (running)
    {
        VRPNButton->mainloop();
    }
 
    return 0;
}
#endif
//#include "GenericVRPNServer.h"
#include <iostream>
#include <cmath>
 
#define DEFAULT_PORT 50555
/*
int main(int argc, char** argv)
{
    GenericVRPNServer* vrpnServer = GenericVRPNServer::getInstance(DEFAULT_PORT);
 
    vrpnServer->addButton("button_test", 1);
    vrpnServer->addAnalog("analog_test", 2);
 
    double time = 0;
    double period = 0;
 
    while (true)
    {
        if (period >= 2 * M_PI)
        {
            vrpnServer->changeButtonState("button_test", 0, 1 - vrpnServer->getButtonState("button_test", 0));
            period = 0;
        }
 
        vrpnServer->changeAnalogState("analog_test", sin(time), cos(time));
 
        time = time + 0.01;
        period = period + 0.01;
 
        vrpnServer->loop();
 
        // sleep for 10 miliseconds (on Unix)
        usleep(10000);
    }
 
    return 0;
}
*/