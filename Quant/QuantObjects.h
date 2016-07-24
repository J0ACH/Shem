#ifndef QUANTOBJECTS_H
#define QUANTOBJECTS_H

#include <QWidget>
#include <QDebug>
#include <QPainter>

#include "Data.h"
#include "Button.h"
#include "ControlBox.h"

using namespace SupercolliderBridge;
using namespace Jui;

namespace QuantIDE
{

  class QuantObject : public QWidget
  {
    Q_OBJECT

  public:
    QuantObject(QWidget *parent, QObject *core);
    ~QuantObject();

    public slots :
    void onNetworkDataRecived(DataNEW);

  signals:
    void actDataSend(DataNEW);
    void actEvaluate(QString code);

  protected: // protected je viditelna jen detmi, ne z venku
    void paintEvent(QPaintEvent *event);

  private:
    QWidget *mCanvan;
  };

  // QUANT PROXYSPACE ////////////////////////////////////////////////////////////////

  class QuantProxy : public QuantObject
  {
    Q_OBJECT
  public:
    QuantProxy(QWidget *parent, QObject *core);
    ~QuantProxy();

    public slots:
    void onBeep();
    void onControlTEST(QString);

    void onDataRecived(DataProxy);

  private:
    Button *testButton;
    ControlBox *nameBox;
    DataProxy data;
  };


  // QUANT NODEPROXY ////////////////////////////////////////////////////////////////

  class QuantNode : public QuantObject
  {
    Q_OBJECT
  public:
    QuantNode(QWidget *parent, QObject *core);
    ~QuantNode();

    void setName(QString);

  private:
    ControlBox *nameBox;
  };


  // QUANT CONTROLS ////////////////////////////////////////////////////////////////

  class QuantControl : public QuantObject
  {
    Q_OBJECT
  public:
    QuantControl(QWidget *parent, QObject *core);
    ~QuantControl();
  };


  // QUANT BUS ////////////////////////////////////////////////////////////////

  class QuantBus : public QuantObject
  {
    Q_OBJECT
  public:
    QuantBus(QWidget *parent, QObject *core);
    ~QuantBus();
  };
}
#endif // QUANTOBJECTS_H

