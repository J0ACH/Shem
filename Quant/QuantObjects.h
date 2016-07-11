#ifndef QUANTOBJECTS_H
#define QUANTOBJECTS_H

#include <QWidget>
#include <QDebug>

#include "QuantCore.h"

namespace QuantIDE
{
  class QuantObject : public QObject
  {
    Q_OBJECT

  public:
    QuantObject(QObject *parent, QuantCore *core);
    ~QuantObject();

    void setLibrary(QString key, QString value);
    QString getLibrary(QString key);

    void printLibrary(QString objectName);

    public slots :
    void onLibraryChanged();

  signals:


  private:
    QuantCore *mCore;
  };


}
#endif // QUANTOBJECTS_H

