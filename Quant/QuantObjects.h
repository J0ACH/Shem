#ifndef QUANTOBJECTS_H
#define QUANTOBJECTS_H

#include <QWidget>
#include <QDebug>
#include <QPainter>

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

    void setMap(QString key, QString value);

    QString getMap_string(QString key);

    void printMap();
    
    public slots :
    void onObjectChanged();

  signals:
    void actMapChanged(QMap <QString, QVariant>);

  protected:
    void paintEvent(QPaintEvent *event);

  private:
    QMap <QString, QVariant> map;
    QWidget *mCanvan;

  };

  // QUANT PROXYSPACE ////////////////////////////////////////////////////////////////

  class QuantProxy : public QuantObject
  {
  public:
    QuantProxy(QWidget *parent, QObject *core);
    ~QuantProxy();
  };


  // QUANT NODEPROXY ////////////////////////////////////////////////////////////////

  class QuantNode : public QuantObject
  {
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
  public:
    QuantControl(QWidget *parent, QObject *core);
    ~QuantControl();
  };


  // QUANT BUS ////////////////////////////////////////////////////////////////

  class QuantBus : public QuantObject
  {
  public:
    QuantBus(QWidget *parent, QObject *core);
    ~QuantBus();
  };
}
#endif // QUANTOBJECTS_H

