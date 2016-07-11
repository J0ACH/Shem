#include "QuantObjects.h"

namespace QuantIDE
{
  QuantObject::QuantObject(QObject *parent, QuantCore *core) :
    QObject(parent),
    mCore(core)
  {
    qDebug("Proxy init...");
    qDebug() << "QuantProxy get[username] " << mCore->getLibrary("user");

    connect(mCore, SIGNAL(actLibraryChanged()), this, SLOT(onLibraryChanged()));

  }

  void QuantObject::setLibrary(QString key, QString value)
  {
    mCore->setLibrary(key, value);
  }

  QString QuantObject::getLibrary(QString key)
  {
    QString value = mCore->getLibrary(key);
    qDebug() << "QuantObject::getLibrary [" << key << "] :" << value;
    return value;
  }

  void QuantObject::onLibraryChanged()
  {
    qDebug() << "QuantObject::onLibraryChanged()";
  }

  void QuantObject::printLibrary(QString objectName)
  {
    QString printTxt = tr("%1::printLibrary\n").arg(objectName);
    printTxt += mCore->printLibrary();

    qDebug() << printTxt;
  }

  QuantObject::~QuantObject()
  {
  }
}


