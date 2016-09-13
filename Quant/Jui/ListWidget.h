#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QEvent>
#include <QKeyEvent> 
#include <QScrollArea>

namespace Jui
{
  class ListWidget : public QWidget
  {
    Q_OBJECT

  public:
    ListWidget(QWidget *parent = 0);
    ~ListWidget();

    void insertWidget(QWidget *item, int index);

    int size();
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

    void sortLibrary();
  };
}
#endif // LISTWIDGET_H

