#include "QuantPanels.h"

namespace QuantIDE
{
  // QuantPanel /////////////////////////////////////////////////////////////////////////////////

  QuantPanel::QuantPanel(QWidget *parent) : PanelNEW(parent)
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

    PanelNEW::resizeEvent(event);
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
}