#include "ListWidget.h"

#define DISPLAY_FRAMES 1

namespace Jui
{

  ListWidget::ListWidget(QWidget *parent) : QWidget(parent)
  {
    parent->installEventFilter(this);

    borderOffset = 5;

    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollWidget = new QWidget(scrollArea);
    scrollWidget->setStyleSheet("background: rgba(0,0,0,0)");
    scrollArea->setWidget(scrollWidget);
  }

  void ListWidget::insertWidget(QWidget *item, int index)
  {
    item->setParent(scrollWidget);
    item->setGeometry(borderOffset, this->positionY(library.size()), 30, item->height());
    item->show();
    library.insert(index, item);
  }

  QWidget* ListWidget::onAppendWidget()
  {
    QWidget *widget = new QWidget(scrollWidget);
    widget->setGeometry(borderOffset, this->positionY(library.size()), 30, 50);
    library.append(widget);
    return widget;
  }
  void ListWidget::onAppendWidget(QWidget *item)
  {
    this->insertWidget(item, library.size());
  }

  QWidget* ListWidget::getWidget(int index)  { return library.at(index); }

  bool ListWidget::eventFilter(QObject *watched, QEvent *event)
  {
    if (event->type() == QEvent::Resize)
    {
      //      qDebug() << "ListWidget::eventFilter parent resized";
      this->setFixedWidth(this->parentWidget()->width() - 20);
      foreach(QWidget *oneW, library)
      {
        oneW->resize(QSize(this->width() - 2 * borderOffset, oneW->height()));
      }
    }
    return QWidget::eventFilter(watched, event);
  }

  void ListWidget::resizeEvent(QResizeEvent *event)
  {
    scrollArea->setGeometry(5, 5, this->width() - 10, this->height() - 10);
    scrollWidget->setFixedSize(scrollArea->width() - 5, this->positionY(library.size()) - 2 * borderOffset);

    foreach(QWidget *oneW, library)
    {
      oneW->setFixedWidth(scrollWidget->width() - 2 * borderOffset);
    }
  }

  void ListWidget::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);

    QRectF rect = QRectF(0, 0, this->width() - 1, this->height() - 1);

    painter.setPen(Qt::green);
    painter.drawRect(rect);

    painter.setPen(Qt::red);

    int no = 0;
    foreach(QWidget *oneW, library)
    {
      painter.drawRect(
        oneW->geometry().left(),
        oneW->geometry().top() + scrollWidget->y(),
        oneW->width(),
        oneW->height()
        );


      painter.drawText(
        oneW->width() - 10,
        oneW->geometry().top() + scrollWidget->y() + 20,
        QString::number(no)
        );
      no++;
    }
    this->update();
  }

  int ListWidget::positionY(int index)
  {
    int itemPositionY = borderOffset;
    int targetIndex = index;
    if (library.size() < index) { targetIndex = library.size(); }

    for (int i = 0; i < targetIndex; i++)
    {
      itemPositionY += library[i]->height();
      itemPositionY += borderOffset;
    }
    return itemPositionY;
  }

  ListWidget::~ListWidget()	{ }

}