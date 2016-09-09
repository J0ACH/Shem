#include "QuantPanels.h"

namespace QuantIDE
{
  // QuantPanel /////////////////////////////////////////////////////////////////////////////////

  QuantPanel::QuantPanel(QWidget *parent) : Panel(parent)
  {
    this->setMinimumWidth(400);
    this->setMinimumHeight(150);
    scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollOffset = 30;
  }

  void QuantPanel::setScrollWidget(QWidget *widget)  { scrollArea->setWidget(widget); }
  QWidget* QuantPanel::getScrollWidget()  { return  scrollArea->widget(); }

  void QuantPanel::setScrollOffset(int offset) { scrollOffset = offset; }

  void QuantPanel::resizeEvent(QResizeEvent *event)
  {
    scrollArea->setGeometry(10, scrollOffset, width() - 20, height() - scrollOffset - 20);
    scrollArea->widget()->setFixedWidth(scrollArea->width() - 10);

    Panel::resizeEvent(event);
  }

  QuantPanel::~QuantPanel() { }

  // NodePanel /////////////////////////////////////////////////////////////////////////////////

  NodePanelNEW::NodePanelNEW(QWidget *parent) : QuantPanel(parent)
  {
    buttonAddNode = new Button(this);
    buttonAddNode->setText("AddNode");

    connect(buttonAddNode, SIGNAL(actPressed()), this, SLOT(onAddNodePressed()));

    this->setScrollOffset(60);
  }

  void NodePanelNEW::onAddNodePressed()
  {
    // qDebug("NodePanelNEW::onButtonAddNodePressed() -> node added");
    emit actAddNode("node", true);
  }

  void NodePanelNEW::resizeEvent(QResizeEvent *event)
  {
    buttonAddNode->setGeometry(5, 25, width() - 10, 20);

    QuantPanel::resizeEvent(event);
  }

  NodePanelNEW::~NodePanelNEW() { }

  // SnippetPanel /////////////////////////////////////////////////////////////////////////////////

  SnippetPanel::SnippetPanel(QWidget *parent) : Panel(parent)
  {
    listWidget = new ListWidget(this);

    buttonSave = new Button(this);
    buttonSave->setText("Save");
    //connect(buttonSave, SIGNAL(pressAct()), this, SLOT(onSavePressed()));

    SnippedPanel_Item *item1 = new SnippedPanel_Item();
    item1->setFixedHeight(70);
    listWidget->onAppendWidget(item1);

    SnippedPanel_Item *item2 = new SnippedPanel_Item();
    item2->setFixedHeight(70);
    listWidget->onAppendWidget(item2);

    listWidget->onAppendWidget();
    //listWidget->onAppendWidget();
    //listWidget->onAppendWidget();
    //listWidget->onAppendWidget();
  }

  void SnippetPanel::resizeEvent(QResizeEvent *event)
  {
    listWidget->setGeometry(10, 30, this->width() - 20, this->height() - 70);
    buttonSave->setGeometry(this->width() - 90, this->height() - 30, 80, 20);

    Panel::resizeEvent(event);
  }

  SnippetPanel::~SnippetPanel() { }

  SnippedPanel_Item::SnippedPanel_Item(QWidget *parent) : QWidget(parent)
  {
    snippetBox = new ControlBox(this);
    snippetBox->setObjectName("snippedBox");
    snippetBox->setLabel("snippet");
    snippetBox->setLabelSize(50);
    snippetBox->setGeometry(5, 5, 200, 20);

    /*
    codeBox = new ControlBox(this);
    codeBox->setObjectName("codeBox");
    codeBox->setLabel("code");
    codeBox->setLabelSize(50);
    codeBox->setGeometry(5, 35, 200, 20);

    codeBox->setTabOrder(snippetBox, codeBox);
    */

    snippetBox->installEventFilter(this);
    //codeBox->installEventFilter(this);
    
    codeEditor = new CodeEditor(this);
    codeEditor->setGeometry(5, 35, 200, 20);
    

  }

  bool SnippedPanel_Item::eventFilter(QObject *watched, QEvent *event)
  {
    QWidget *firstChildren;

    switch (event->type())
    {
    case QEvent::KeyPress:

      QKeyEvent *eventKey = static_cast<QKeyEvent*>(event);  // static_cast<QKeyEvent>(event);
      quint32 modifers = eventKey->nativeModifiers();

      switch (eventKey->key())
      {

      case Qt::Key::Key_Up:
        //this->findChild<ControlBox*>("snippedBox")->hasFocus();
        //if (this->findChild<ControlBox*>("snippedBox")->hasFocus())
        if (snippetBox->hasFocus())
        {
          /*
          ControlBox *target  = this->findChild<ControlBox*>("codeBox");
          target->setFocus();
          target->update();
          */
         // codeBox->setFocus(Qt::FocusReason::MouseFocusReason);
          qDebug() << "SnippedBox ma focus" << this->findChild<ControlBox*>("codeBox")->hasFocus();
        }
        /*
        if (static_cast<ControlBox*>(this->children().at(0))->hasFocus())
        {
        static_cast<ControlBox*>(this->children().at(this->children().size()-1))->setFocus();
        qDebug() << "Key_Up SnippedPanel_Item::eventFilter focus set to childAt" << codeBox->origin();

        }
        if (static_cast<ControlBox*>(this->children().at(0))->hasFocus())
        {
        this->childAt(codeBox->origin())->setFocus();
        qDebug() << "Key_Up SnippedPanel_Item::eventFilter focus set to childAt" << codeBox->origin();
        }
        else {
        this->childAt(snippetBox->origin())->setFocus();
        qDebug() << "Key_Up SnippedPanel_Item::eventFilter focus set to childAt" << snippetBox->origin();
        }
        */


        break;

      case Qt::Key::Key_Down:

        if (codeEditor->hasFocus())
        {
          snippetBox->setFocus(Qt::FocusReason::MouseFocusReason);
          qDebug() << "SnippedBox ma focus" << this->findChild<ControlBox*>("codeBox")->hasFocus();
        }

        /*

        if (static_cast<ControlBox*>(this->children().at(this->children().size() - 1))->hasFocus())
        {
          this->childAt(snippetBox->origin())->setFocus();
          qDebug() << "Key_Down SnippedPanel_Item::eventFilter focus set to childAt" << codeBox->origin();
        }
        */
        break;
        /*
        if (static_cast<ControlBox*>(this->children().at(0))->hasFocus())
        {
        this->childAt(codeBox->origin())->setFocus();
        qDebug() << "Key_Down SnippedPanel_Item::eventFilter focus set to childAt" << codeBox->origin();
        }
        else {
        this->childAt(snippetBox->origin())->setFocus();
        qDebug() << "Key_Down SnippedPanel_Item::eventFilter focus set to childAt" << snippetBox->origin();
        }
        break;
        */
      }

      qDebug() << "SnippedPanel_Item::eventFilter UPDATE";
      this->update();
    }

    return QWidget::eventFilter(watched, event);
  }

  void SnippedPanel_Item::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    QRectF rect = QRectF(0, 0, this->width() - 1, this->height() - 1);

    //if (this->hasFocus()) { painter.setPen(QPen(QColor(30, 220, 30), 3)); }
    //else { painter.setPen(QPen(QColor(30, 120, 30), 1)); }

    //painter.drawRect(rect);
    painter.fillRect(rect, QColor(20, 130, 20, 70));

    QWidget::paintEvent(event);
  }

  SnippedPanel_Item::~SnippedPanel_Item() {};
}



