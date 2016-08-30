#ifndef LIBRARY_H
#define LIBRARY_H

#include "QuantObjects.h"

#include <QWidget>
#include <QPainter>
#include <QDebug>

using namespace Jui;

namespace QuantIDE
{
  class Library : public QWidget
  {
    Q_OBJECT
      Q_PROPERTY(QColor colorBackground READ getColorBackground WRITE setColorBackground)

  public:
    Library(QWidget *parent, QObject *core);
    ~Library();

    void addObject(QuantObject* obj);
    void removeObject(QString name);
    bool containObject(QString name);

    QList<QString> keys();

    QuantUser* getUser(QString name);
    QuantUser* getUser(DataUser data);

    QuantProxy* getProxy();

    QuantNode* getNode(QString name);
    QuantNode* getNode(DataUser data);

    QString getUniqueName(QString);

    void setColorBackground(QColor color);
    QColor getColorBackground();

  public slots:
    void onUpdateObjectPosition();

  protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

  private:
    QObject *mCore;
    QMap<QString, QuantObject*> lib;

    QColor colorBackground;
  };
}

#endif // LIBRARY_H

