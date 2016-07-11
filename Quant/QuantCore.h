#ifndef QUANTCORE_H
#define QUANTCORE_H

#include <QWidget>
#include <QDebug>

#include "ScBridge.h"
#include "Customize.h"
#include "UDPServer.h"

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

    public slots :
    void onLibraryChange(QMap <QString, QString>);

  signals:
    void actLibraryChanged();


  protected:


  private:
    ScBridge *mBridge;
    UDPServer *mNetwork;
    Customize *mCustomize;

    QMap <QString, QString> library;

  }; 
}
#endif // QUANTCORE_H

