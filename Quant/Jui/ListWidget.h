#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QEvent>

namespace Jui
{
  class ListWidget : public QWidget
  {
    Q_OBJECT

  public:
    ListWidget(QWidget *parent = 0);
    ~ListWidget();

    void getWidget(int i);

    public slots:
    void onAppendWidget();
    void onAppendWidget(QWidget *item);

  protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent * event);

  private:
    QList<QWidget*> library;

    int borderOffset;
    int positionY(int index);
  };
}
#endif // LISTWIDGET_H

