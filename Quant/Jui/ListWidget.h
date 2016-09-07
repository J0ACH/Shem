#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QEvent>
#include <QScrollArea>

#include <QKeyEvent> 




namespace Jui
{
  class ListWidget : public QWidget
  {
    Q_OBJECT

  public:
    ListWidget(QWidget *parent = 0);
    ~ListWidget();

    void insertWidget(QWidget *item, int index);

    QWidget *getWidget(int i);

    public slots:
    QWidget *onAppendWidget();
    void onAppendWidget(QWidget *item);
    
  protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent * event);

  private:
    QList<QWidget*> library;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    int borderOffset;
    int positionY(int index);
  };
}
#endif // LISTWIDGET_H

