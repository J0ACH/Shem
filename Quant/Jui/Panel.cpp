#include "Panel.h"

namespace Jui
{

  PanelNEW::PanelNEW(QWidget *parent) : QDockWidget(parent, Qt::FramelessWindowHint)
  {
    this->setFeatures(QDockWidget::DockWidgetMovable);
    this->setTitle("Panel");
    this->setMinimumWidth(250);
    //this->setAllowedAreas(Qt::DockWidgetArea::AllDockWidgetAreas);
    //this->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea);

    this->setColorBackground(QColor(30, 30, 30));
    this->setColorHeader(QColor(40, 40, 40));
    this->setColorTitle(QColor(130, 30, 30));

    closeButton = new Button(this);
    closeButton->setIcon(QImage(":/smallClose16.png"), 0);
    connect(closeButton, SIGNAL(pressAct()), this, SLOT(onSwitchVisible()));
  }

  void PanelNEW::setTitle(QString name)  { title = name; this->setWindowTitle(title); }
  void PanelNEW::setFontTitle(QFont font)  { fontTitle = font; }
  void PanelNEW::setColorTitle(QColor color) { colorText = color; update(); }
  void PanelNEW::setColorBackground(QColor color) { colorBackground = color; update(); }
  void PanelNEW::setColorHeader(QColor color) { colorHeader = color; update(); }
  void PanelNEW::setColorNormal(QColor color){ closeButton->setColorNormal(color); }
  void PanelNEW::setColorOver(QColor color){ closeButton->setColorOver(color); }
  void PanelNEW::setColorActive(QColor color){ closeButton->setColorActive(color); }

  void PanelNEW::setPanelAllowedSides(Qt::DockWidgetAreas sides) { this->setAllowedAreas(sides); }
  //void PanelNEW::setPanelSide(Qt::DockWidgetArea side) { this->parent setCorneAl(side); }

  void PanelNEW::onSwitchVisible()
  {
    if (this->isVisible()) { this->setVisible(false); }
    else { this->setVisible(true); }
  }

  void PanelNEW::resizeEvent(QResizeEvent *resizeEvent)
  {
    closeButton->setGeometry(this->width() - 18, 2, 16, 16);
    
    QDockWidget::resizeEvent(resizeEvent);
    this->update();
  }

  void PanelNEW::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setFont(fontTitle);
    painter.fillRect(QRect(1, 0, this->width() - 2, 20), colorHeader);
    painter.fillRect(QRect(1, 20, this->width() - 2, this->height() - 21), colorBackground);

    painter.setPen(colorText);
    painter.drawText(10, 14, title);

    bool showScreen = false;
    if (showScreen)
    {
      painter.setPen(QColor(30, 30, 130));
      painter.drawLine(this->geometry().topLeft(), this->geometry().bottomRight());
      painter.drawLine(this->geometry().bottomLeft(), this->geometry().topRight());
    }
  }

  PanelNEW::~PanelNEW() {}



  Panel::Panel(QWidget *parent) : QWidget(parent)
  {
    //this->setParent(parent);
    setObjectName("Panel");

    title = "panel";
    colorPanelBackground = QColor(120, 120, 120);

    this->initControl();

    connect(closeButton, SIGNAL(pressAct()), this, SLOT(hide()));
  }

  QRect Panel::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

  void Panel::initControl()
  {
    closeButton = new Button(this);
    closeButton->setIcon(QImage(":/smallClose16.png"), 0);
    closeButton->setText("X");
    closeButton->hide(); // duvod proc neni videt;

    edges = new Edges(this);
  }

  /*
  void Panel::onConfigData(QMap<QString, QVariant*> config)
  {

  colorPanelBackground = config.value("color_shem_PanelBackground")->value<QColor>();
  colorNormal = config.value("color_shem_Normal")->value<QColor>();
  colorOver = config.value("color_shem_Over")->value<QColor>();
  colorActive = config.value("color_shem_Active")->value<QColor>();
  colorText = config.value("color_shem_Text")->value<QColor>();
  fontTextBig = config.value("font_shem_TextBig")->value<QFont>();


  this->setColorBackground(colorPanelBackground);
  this->setColorTitle(colorText);

  closeButton->setColorNormal(colorNormal);
  closeButton->setColorOver(colorOver);
  closeButton->setColorActive(colorActive);

  update();
  }
  void Panel::onCustomize()
  {
  qDebug("Panel::onCustomize");

  colorPanelBackground = this->property("color_shem_PanelBackground").value<QColor>();
  colorNormal = this->property("color_shem_Normal").value<QColor>();
  colorOver = this->property("color_shem_Over").value<QColor>();
  colorActive = this->property("color_shem_Active").value<QColor>();
  colorText = this->property("color_shem_Text").value<QColor>();
  fontTextBig = this->property("font_shem_TextBig").value<QFont>();


  this->setColorBackground(colorPanelBackground);
  this->setColorTitle(colorText);

  closeButton->setColorNormal(colorNormal);
  closeButton->setColorOver(colorOver);
  closeButton->setColorActive(colorActive);

  //update();
  }
  */

  void Panel::setTitle(QString name) { title = name; }

  void Panel::setColorBackground(QColor color) { colorPanelBackground = color; update(); }
  void Panel::setColorTitle(QColor color) { colorText = color; update(); }
  void Panel::setFontTitle(QFont font)  { fontTitle = font; }

  void Panel::setEdgeControler(EdgeControler::Direction direction, bool visible)
  {
    switch (direction)
    {
    case EdgeControler::Direction::LEFT:
      if (visible)
      {
        edges->addManipulator(EdgeControler::Direction::LEFT);
      }
      break;
    case EdgeControler::Direction::TOP:
      if (visible)
      {
        edges->addManipulator(EdgeControler::Direction::TOP);
      }
      break;
    case EdgeControler::Direction::RIGHT:
      if (visible)
      {
        edges->addManipulator(EdgeControler::Direction::RIGHT);
      }
      break;
    case EdgeControler::Direction::BOTTOM:
      if (visible)
      {
        edges->addManipulator(EdgeControler::Direction::BOTTOM);
      }
      break;
    }
  }

  void Panel::mousePressEvent(QMouseEvent *mouseEvent) { mouseEvent->accept(); }

  void Panel::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setFont(fontTitle);

    painter.fillRect(bounds(), colorPanelBackground);

    painter.setPen(QPen(colorText, 1));
    painter.drawText(15, 25, title);

    bool showScreen = false;
    if (showScreen)
    {
      painter.setPen(QColor(30, 30, 130));
      painter.drawLine(this->geometry().topLeft(), this->geometry().bottomRight());
      painter.drawLine(this->geometry().bottomLeft(), this->geometry().topRight());
    }
  }

  void Panel::resizeEvent(QResizeEvent *resizeEvent)
  {
    closeButton->setGeometry(this->width() - 30, 10, 16, 16);
    emit resizeAct(); // zustava prozatim pouze pro Edges
  }

  Panel::~Panel()
  {

  }
}