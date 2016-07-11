#include "QuantCore.h"

namespace QuantIDE
{
  QuantCore::QuantCore(QObject *parent, ScBridge *bridge, UDPServer *network, Customize *customize) :
    QObject(parent),
    mBridge(bridge),
    mNetwork(network),
    mCustomize(customize)
  {
    qDebug("Core init...");
    this->setLibrary("user", "testName");
  }

  void QuantCore::setLibrary(QString key, QString value)
  {
    library.insert(key, value);
    emit actLibraryChanged();
  }

  QString QuantCore::getLibrary(QString key)
  {
    return library.value(key);
  }

  void QuantCore::onLibraryChange(QMap<QString, QString> newLib)
  {
    foreach(QString oneValue, newLib.values())
    {
      qDebug() << "QuantCore::onLibraryChange get library:" << oneValue;
    }
  }

  QString QuantCore::printLibrary()
  {
    QString printTxt; // = tr("%1::printLibrary\n").arg(objectName);

    foreach(QString oneValue, library.values())
    {
      printTxt += tr("\t-> %1\n").arg(oneValue);
    }

    return printTxt;
  }

  QuantCore::~QuantCore() { }
}


