#ifndef QUANTCORE_H
#define QUANTCORE_H

#include <QWidget>
#include <QDebug>

#include "Customize.h"
#include "ScBridge.h"
#include "UDPServer.h"

#include "Canvan.h"
#include "QuantObjects.h"

using namespace SupercolliderBridge;

namespace QuantIDE
{

  class QuantCore : public QObject
  {
    Q_OBJECT

  public:
    QuantCore(CanvanNEW *canvan, Customize *customize);
    ~QuantCore();

    void addProxySpace();
    void addNode(QString name);

    public slots :
    void onMapChanged(QMap <QString, QVariant>);


  signals:
    void actObjectChanged();


  protected:


  private:
    CanvanNEW *mCanvan;
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

