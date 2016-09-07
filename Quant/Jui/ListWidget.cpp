#include "ListWidget.h"

#define DISPLAY_FRAMES 1

namespace Jui
{

  ListWidget::ListWidget(QWidget *parent) : QWidget(parent)
  {
    parent->installEventFilter(this);

    this->setObjectName("ListWidget");

    borderOffset = 5;

    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setObjectName("ListWidget_scrollArea");
    scrollWidget = new QWidget(scrollArea);
    scrollWidget->setObjectName("ListWidget_scrollWidget");
    scrollWidget->setStyleSheet("background: rgba(0,0,0,0)");
    scrollArea->setWidget(scrollWidget);
  }

  void ListWidget::insertWidget(QWidget *item, int index)
  {
    item->setParent(scrollWidget);
    item->setFocusPolicy(Qt::StrongFocus);
    item->setGeometry(borderOffset, this->positionY(library.size()), 30, item->height());
    item->setObjectName(QString("item_%1").arg(QString::number(index)));
    item->show();
    library.insert(index, item);

    if (library.size() > 1)
    {
      item->setTabOrder(library.at(index - 1), item);
      item->setTabOrder(item, library.at(index));

      if (index < library.size())
      {
        //item->setTabOrder(item, library.at(index + 1));
      }
      else
      {
        item->setTabOrder(item, library.at(0));
      }
      //library.at(library.size()-1)->setTabOrder((library.at(index - 1), library.at(index));
    }

    item->installEventFilter(this);

  }

  QWidget* ListWidget::onAppendWidget()
  {
    QWidget *item = new QWidget(scrollWidget);
    item->setFixedHeight(50);
    this->insertWidget(item, library.size());
    return item;
  }
  void ListWidget::onAppendWidget(QWidget *item)  { this->insertWidget(item, library.size()); }

  QWidget* ListWidget::getWidget(int index)  { return library.at(index); }



  bool ListWidget::eventFilter(QObject *watched, QEvent *event)
  {
    switch (event->type())
    {
    case QEvent::Resize:
      qDebug() << "ListWidget::eventFilter parent resized";
      this->setFixedWidth(this->parentWidget()->width() - 20);
      foreach(QWidget *oneW, library)
      {
        oneW->resize(QSize(this->width() - 2 * borderOffset, oneW->height()));
      }

      break;

    case QEvent::KeyPress:

      QKeyEvent *eventKey = static_cast<QKeyEvent*>(event);  // static_cast<QKeyEvent>(event);
      quint32 modifers = eventKey->nativeModifiers();

      switch (eventKey->key())
      {
      case Qt::Key::Key_Down:
        this->focusNextChild();
        
        break;
      case Qt::Key::Key_Up:
        this->focusPreviousChild();
        
        break;
      }

      this->update();

      break;
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
      if (oneW->hasFocus()) { painter.setPen(QPen(QColor(220, 30, 30), 3)); }
      else { painter.setPen(QPen(QColor(120, 30, 30), 1)); }
      painter.drawRect(
        oneW->geometry().left(),
        oneW->geometry().top() + scrollWidget->y(),
        oneW->width(),
        oneW->height()
        );

      painter.setPen(QColor(120, 30, 30));

      painter.drawText(
        oneW->width() - 30,
        oneW->geometry().top() + scrollWidget->y() + 20,
        oneW->objectName()
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