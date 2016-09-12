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
    listWidget->onAppendWidget();
    listWidget->onAppendWidget();
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

    connect(snippetBox, SIGNAL(actPreviousFocused(QWidget*)), this, SLOT(onPreviousFocused(QWidget*)));
    connect(snippetBox, SIGNAL(actNextFocused(QWidget*)), this, SLOT(onNextFocused(QWidget*)));
    connect(snippetBox, SIGNAL(actParentFocused(QWidget*)), this, SLOT(onParentFocused(QWidget*)));

    codeEditor = new CodeEditor(this); 

    connect(codeEditor, SIGNAL(actPreviousFocused(QWidget*)), this, SLOT(onPreviousFocused(QWidget*)));
    connect(codeEditor, SIGNAL(actNextFocused(QWidget*)), this, SLOT(onNextFocused(QWidget*)));
    connect(codeEditor, SIGNAL(actParentFocused(QWidget*)), this, SLOT(onParentFocused(QWidget*)));
    connect(codeEditor, SIGNAL(actHeightChanged()), this, SLOT(onCodeHeightChanged()));
  }

  void SnippedPanel_Item::onPreviousFocused(QWidget *target)
  {
    //qDebug() << "SnippedPanel_Item::onPreviousFocused target" << target->objectName();
    if (target->objectName() == "snippedBox") { codeEditor->setFocus(); }
    else { snippetBox->setFocus(); }
  }
  void SnippedPanel_Item::onNextFocused(QWidget *target)
  {
    //qDebug() << "SnippedPanel_Item::onNextFocused target" << target->objectName();
    if (target->objectName() == "snippedBox") { codeEditor->setFocus(); }
    else { snippetBox->setFocus(); }
  }
  void SnippedPanel_Item::onParentFocused(QWidget *target)  { this->setFocus(); }

  void SnippedPanel_Item::onCodeHeightChanged()
  {
    this->setFixedHeight(snippetBox->height() + codeEditor->height() + 15);
  }

  bool SnippedPanel_Item::eventFilter(QObject *watched, QEvent *event)
  {
    switch (event->type())
    {
    case QEvent::KeyPress:

      QKeyEvent *eventKey = static_cast<QKeyEvent*>(event);  // static_cast<QKeyEvent>(event);
      quint32 modifers = eventKey->nativeModifiers();

      switch (eventKey->key())
      {

      case Qt::Key::Key_Up:
        qDebug() << "SnippedPanel_Item::eventFilter Key_Up";
        //this->findChild<ControlBox*>("snippedBox")->hasFocus();
        //if (this->findChild<ControlBox*>("snippedBox")->hasFocus())

        if (snippetBox->hasFocus())
        {
          codeEditor->setFocus();
        }
        else
        {
          snippetBox->setFocus();
        }
        break;

      case Qt::Key::Key_Down:
        qDebug() << "SnippedPanel_Item::eventFilter Key_Down";

        if (codeEditor->hasFocus())
        {
          snippetBox->setFocus();
          // qDebug() << "SnippedBox ma focus" << this->findChild<ControlBox*>("codeBox")->hasFocus();
        }
        else
        {
          codeEditor->setFocus();
          //  codeEditor->update();
        }
        break;

      case Qt::Key::Key_Return:
        qDebug() << "SnippedPanel_Item::eventFilter Key_ENTER";
        snippetBox->enterFocus();
        snippetBox->update();
        return true;

      case Qt::Key::Key_Escape:
        qDebug() << "SnippedPanel_Item::eventFilter Key_Escape";
        this->setFocus();
        //        this->update();
        return true;
        break;
      }

      //qDebug() << "SnippedPanel_Item::eventFilter UPDATE";
      this->update();
    }

    return QWidget::eventFilter(watched, event);
  }

  void SnippedPanel_Item::resizeEvent(QResizeEvent *event)
  {
    snippetBox->setGeometry(5, 5, this->width() - 10, 20);
    codeEditor->setGeometry(5, 30, this->width() - 10, 20);
    this->onCodeHeightChanged();
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



