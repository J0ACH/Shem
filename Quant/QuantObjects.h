#ifndef QUANTOBJECTS_H
#define QUANTOBJECTS_H

#include <QWidget>
#include <QDebug>
#include <QPainter>

#include "Button.h"
#include "ControlBox.h"

using namespace Jui;

namespace QuantIDE
{

  class QuantObject : public QWidget
  {
    Q_OBJECT

  public:
    QuantObject(QWidget *parent, QObject *core);
    ~QuantObject();

    QString getMap_string(QString key);

    void printMap();

    public slots :
    void onMyMapSet(QString key, QString value);
    void onNetworkMapSet(QString key, QString value);

  signals:
    void actEvaluate(QString code);
    void actMyMapSet(QMap <QString, QVariant>);
    void actNetworkMapSet(QMap <QString, QVariant>);

  protected: // protected je viditelna jen detmi, ne z venku
    void paintEvent(QPaintEvent *event);
    void setMap(QString key, QString value);

  private:
    QWidget *mCanvan;
    QMap <QString, QVariant> map;



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

  private:
    Button *testButton;
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

