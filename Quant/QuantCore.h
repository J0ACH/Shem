#ifndef QUANTCORE_H
#define QUANTCORE_H

#include <QWidget>
#include <QDebug>

#include "ScBridge.h"
#include "Customize.h"
#include "UDPServer.h"

#include "QuantObjects.h"

using namespace SupercolliderBridge;

namespace QuantIDE
{

  class QuantCore : public QObject
  {
    Q_OBJECT

  public:
    QuantCore(QObject *parent, ScBridge *bridge, UDPServer *network, Customize *customize);
    ~QuantCore();

    void setLibrary(QString key, QString value);
    QString getLibrary(QString key);
    QString printLibrary();

    void addNode(QString name);

    public slots :
    void onMapChanged(QMap <QString, QVariant>);
    

  signals:
    void actObjectChanged();


  protected:


  private:
    ScBridge *mBridge;
    UDPServer *mNetwork;
    Customize *mCustomize;

    QMap <QString, QString> library;
    QuantProxy *proxy;
    QMap <QString, QuantNode> nodeLibrary;
    QMap <QString, QuantControl> controloLibrary;
    QMap <QString, QuantBus> busLibrary;
    
    //QMap <QString, QuantNode> library;

  }; 
}
#endif // QUANTCORE_H

