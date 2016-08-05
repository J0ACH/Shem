#ifndef LIBRARY_H
#define LIBRARY_H

#include "QuantObjects.h"

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QScrollArea>


using namespace Jui;
using namespace SupercolliderBridge;

namespace QuantIDE
{
  class Library : public QWidget
  {
    Q_OBJECT

  public:
    Library(QWidget *parent, QObject *core);
    ~Library();

    void addObject(QuantObject* obj);
    void addObject(DataNEW data);

    void removeObject(DataNEW data);

    QuantUser* getUser(QString name);
    QuantUser* getUser(DataUser data);

    void display(QWidget *parent = 0);

    void updateObjectPosition();

  protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

  private:
    QObject *mCore;
    QMap<QString, QuantObject*> lib;

    bool containObject(DataNEW data);
  };
}

#endif // LIBRARY_H

