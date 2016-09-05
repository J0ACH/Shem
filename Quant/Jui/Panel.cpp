#include "Panel.h"

namespace Jui
{

  Panel::Panel(QWidget *parent) : QDockWidget(parent, Qt::FramelessWindowHint)
  {
    this->setFeatures(QDockWidget::DockWidgetMovable);
    this->setTitle("Panel");
    this->setMinimumWidth(250);
    //this->setAllowedAreas(Qt::DockWidgetArea::AllDockWidgetAreas);
    //this->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea);

    colorBackground = QColor(130, 30, 30);
    colorHeader = QColor(140, 40, 40);
    colorTitle = QColor(230, 30, 30);

    closeButton = new Button(this);
    closeButton->setIcon(QImage(":/smallClose16.png"), 0);
    connect(closeButton, SIGNAL(pressAct()), this, SLOT(onSwitchVisible()));
    connect(closeButton, SIGNAL(pressAct()), this, SLOT(onClose()));
  }

  void Panel::setTitle(QString name)  { title = name; this->setWindowTitle(title); }

  void Panel::setColorHeader(QColor color) { colorHeader = color; update(); }
  void Panel::setColorBackground(QColor color) { colorBackground = color; update(); }
  void Panel::setColorTitle(QColor color) { colorTitle = color; update(); }
  
  QColor Panel::getColorHeader() { return colorHeader; }
  QColor Panel::getColorBackground() { return colorBackground; }
  QColor Panel::getColorTitle() { return colorTitle; }
  
  void Panel::setFontTitle(QFont font)  { fontTitle = font; update(); }
  void Panel::setFontBig(QFont font)  { fontBig = font; update(); }
  void Panel::setFontSmall(QFont font)  { fontSmall = font; update(); }
 // void Panel::setFontConsole(QFont font)  { fontConsole = font; update(); }
  
  QFont Panel::getFontTitle() { return fontTitle; }
  QFont Panel::getFontBig() { return fontBig; }
  QFont Panel::getFontSmall() { return fontSmall; }
 // QFont Panel::getFontConsole() { return fontConsole; }

  void Panel::setPanelAllowedSides(Qt::DockWidgetAreas sides) { this->setAllowedAreas(sides); }
  //void Panel::setPanelSide(Qt::DockWidgetArea side) { this->parent setCorneAl(side); }

  void Panel::onSwitchVisible()
  {
    if (this->isVisible()) { this->setVisible(false); }
    else { this->setVisible(true); }
  }
  void Panel::onClose()  { emit actClosed(); }

  void Panel::resizeEvent(QResizeEvent *resizeEvent)
  {
    closeButton->setGeometry(this->width() - 18, 2, 16, 16);

    QDockWidget::resizeEvent(resizeEvent);
    this->update();
  }

  void Panel::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.fillRect(QRect(1, 0, this->width() - 2, 20), colorHeader);
    painter.fillRect(QRect(1, 20, this->width() - 2, this->height() - 21), colorBackground);

    painter.setFont(fontTitle);
    painter.setPen(colorTitle);
    painter.drawText(10, 14, title);

    bool showScreen = false;
    if (showScreen)
    {
      painter.setPen(QColor(30, 30, 130));
      painter.drawLine(this->geometry().topLeft(), this->geometry().bottomRight());
      painter.drawLine(this->geometry().bottomLeft(), this->geometry().topRight());
    }
  }

  Panel::~Panel() {}

}