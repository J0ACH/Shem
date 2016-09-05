#include "ListWidget.h"

#define DISPLAY_FRAMES 1

namespace Jui
{

  ListWidget::ListWidget(QWidget *parent) : QWidget(parent)
  {
    parent->installEventFilter(this);

    borderOffset = 5;
  }

  void ListWidget::onAppendWidget()
  {
    QWidget *widget = new QWidget(this);
    widget->setGeometry(borderOffset, this->positionY(library.size()), 30, 50);
    library.append(widget);
  }
  void ListWidget::onAppendWidget(QWidget *item)
  {
    item->setParent(this);
    item->setGeometry(borderOffset, this->positionY(library.size()), 30, item->height());
    item->show();
    library.append(item);
  }

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
    foreach(QWidget *oneW, library)
    {
      oneW->setFixedWidth(this->width() - 2 * borderOffset);
    }
  }

  void ListWidget::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);

    QRectF rect = QRectF(0, 0, this->width() - 1, this->height() - 1);

    //    painter.drawText(rect, text, option);

    painter.setPen(Qt::red);
    painter.drawRect(QRectF(0, 0, this->width() - 1, this->height() - 1));

    foreach(QWidget *oneW, library)
    {
      painter.drawRect(oneW->geometry());
    }
    this->update();
  }

  int ListWidget::positionY(int index)
  {
    int itemPositionY = borderOffset;
    int targetIndex = index;
    if (library.size() < index) { targetIndex = library.size(); }

    for (int i = 0; i < index; i++)
    {
      itemPositionY += library[i]->height();
      itemPositionY += borderOffset;
    }
    return itemPositionY;
  }

  ListWidget::~ListWidget()	{ }

}