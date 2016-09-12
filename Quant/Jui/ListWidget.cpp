#include "ListWidget.h"

#define DISPLAY_FRAMES 1

namespace Jui
{

  ListWidget::ListWidget(QWidget *parent) : QWidget(parent)
  {
    //parent->installEventFilter(this);

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
    item->show();
    item->installEventFilter(this);
    library.insert(index, item);

    this->sortLibrary();
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

  void ListWidget::sortLibrary()
  {
    for (int i = 0; i < library.size(); i++)
    {
      library[i]->setObjectName(QString("item_%1").arg(QString::number(i)));
      library[i]->setGeometry(library[i]->geometry().left(), this->positionY(i), library[i]->width(), library[i]->height());
      if (i < library.size() - 1)
      {
        library[i]->setTabOrder(library.at(i), library.at(i + 1));
      }
    }
  }

  bool ListWidget::eventFilter(QObject *watched, QEvent *event)
  {
    QWidget *firstChildren;

    switch (event->type())
    {
    case QEvent::Resize:
      qDebug() << "ListWidget::eventFilter parent resized";
      this->setFixedWidth(this->parentWidget()->width() - 20);
      foreach(QWidget *oneW, library)
      {
        oneW->resize(QSize(this->width() - 2 * borderOffset, oneW->height()));
      }
      this->sortLibrary();
      break;

    case QEvent::KeyPress:

      QKeyEvent *eventKey = static_cast<QKeyEvent*>(event);
      quint32 modifers = eventKey->nativeModifiers();

      switch (eventKey->key())
      {
      case Qt::Key::Key_Down:
        if (!library.at(library.size() - 1)->hasFocus()) { this->focusNextChild(); }
        else { library.at(0)->setFocus(); }
        break;
      case Qt::Key::Key_Up:
        if (!library.at(0)->hasFocus()) { this->focusPreviousChild(); }
        else { library.at(library.size() - 1)->setFocus(); }
        break;
      case Qt::Key::Key_Return:
        qDebug() << "ListWidget::eventFilter ENTER on " << watched->objectName();
        qDebug() << "ListWidget::eventFilter children " << watched->children();
        if (watched->children().size() > 0)
        {
          firstChildren = static_cast<QWidget*>(watched->children().at(0));
          firstChildren->setFocus();
        }
        break;
      case Qt::Key::Key_Escape:
        qDebug() << "ListWidget::eventFilter ESCon " << watched->objectName();
        break;
      }
      this->update();
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
        oneW->geometry().left() + borderOffset,
        oneW->geometry().top() + scrollWidget->y() + borderOffset,
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