#ifndef QUANTOBJECTS_H
#define QUANTOBJECTS_H

#include <QWidget>
#include <QDebug>

namespace QuantIDE
{
  enum ObjectType { BASE, PROXYSPACE, NODEPROXY, ENVCONTROL, BUS };

  class QuantObject : public QObject
  {
    Q_OBJECT

  public:
    QuantObject(QObject *parent);
    ~QuantObject();

    ObjectType objectType;

    void setMap(QString key, QString value);
    void setMap(QString key, ObjectType value);
    

    void printMap();


    public slots :
    void onObjectChanged();

  signals:
    void actMapChanged(QMap <QString, QVariant>);

  private:
    QMap <QString, QVariant> map;

  };

  // QUANT PROXYSPACE ////////////////////////////////////////////////////////////////

  class QuantProxy : public QuantObject
  {
  public:
    QuantProxy(QObject *parente);
    ~QuantProxy();
  };


  // QUANT NODEPROXY ////////////////////////////////////////////////////////////////

  class QuantNode : public QuantObject
  {
  public:
    QuantNode(QObject *parent);
    ~QuantNode();
  };


  // QUANT CONTROLS ////////////////////////////////////////////////////////////////

  class QuantControl : public QuantObject
  {
  public:
    QuantControl(QObject *parent);
    ~QuantControl();
  };


  // QUANT BUS ////////////////////////////////////////////////////////////////

  class QuantBus : public QuantObject
  {
  public:
    QuantBus(QObject *parent);
    ~QuantBus();
  };
}
#endif // QUANTOBJECTS_H

