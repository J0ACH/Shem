#include "Canvan.h"
#include "JuiConfig.h"

namespace Jui
{
  CanvanNEW::CanvanNEW(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
  {
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("CanvanNEW");
    this->setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    headerSize = 50;
    tailSize = 30;
    isMoveing = false;

    mapPanels = new QMap<QString, Panel*>();
    mapButtonBars = new QMap<QString, CanvanNEW_ToolBar*>();

    colorBackground = QColor(25, 25, 25);

    this->initControl();

  }
  void CanvanNEW::initControl()
  {
    screen = new QWidget(this);

    menuBar = new CanvanNEW_MenuBar(this);
    menuBar->installEventFilter(this);
    menuBar->setFixedHeight(headerSize);
    this->setMenuBar(menuBar);

    statusBar = new CanvanNEW_StatusBar(this);
    statusBar->setFixedHeight(tailSize);
    this->setStatusBar(statusBar);

    fileMenu = new QMenu("file");
    fileMenu->installEventFilter(this);
    fileMenu->hide(); // skryte
    //menuBar->addMenu(fileMenu);

    fileMenu->addAction("setup");
    fileMenu->addAction("exit");

    closeButton = new Button(menuBar);
    closeButton->setIcon(QImage(":/close16.png"), 0);
    connect(closeButton, SIGNAL(pressAct()), this, SLOT(onEmitClose()));

    maximizeButton = new Button(menuBar);
    maximizeButton->setIcon(QImage(":/maximize16.png"), 0);
    maximizeButton->setStateKeeping(Button::StateKeeping::HOLD);
    connect(maximizeButton, SIGNAL(pressAct()), this, SLOT(onCanvanMaximized()));

    minimizeButton = new Button(menuBar);
    minimizeButton->setIcon(QImage(":/minimize16.png"), 0);
    connect(minimizeButton, SIGNAL(pressAct()), this, SLOT(onCanvanMinimized()));

    this->setCentralWidget(screen);
  }

  void CanvanNEW::addPanel(Panel *panel, QString name, Qt::DockWidgetArea startPosition)
  {
    panel->setTitle(name);
    panel->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    this->addDockWidget(startPosition, panel);

    mapPanels->insert(name, panel);
  }
  QWidget* CanvanNEW::getPanel(QString name)  { return mapPanels->value(name); }

  void CanvanNEW::addButtonBar(CanvanNEW_ToolBar *buttonBar, QString name, Qt::ToolBarArea startPosition)
  {
    //buttonBar->setAllowedAreas(Qt::ToolBarArea::TopToolBarArea | Qt::ToolBarArea::BottomToolBarArea);

    mapButtonBars->insert(name, buttonBar);
    this->addToolBar(startPosition, buttonBar);
  }

  CanvanNEW_ToolBar* CanvanNEW::getButtonBar(QString name)
  {
    return mapButtonBars->value(name);
  }

  QWidget* CanvanNEW::getStaustBar() { return statusBar; }

  void CanvanNEW::setColorBackground(QColor color)  { colorBackground = color; }
  QColor CanvanNEW::getColorBackground() { return colorBackground; }

  void CanvanNEW::onEmitClose()  { emit actClose(); }
  void CanvanNEW::onCanvanClose()
  {
    qDebug("CanvanNEW::onCanvanClose");
    this->close();
    this->deleteLater();
  }
  void CanvanNEW::onCanvanMaximized()
  {
    if (this->windowState() == Qt::WindowNoState)
    {
      this->showMaximized();
      this->setWindowState(Qt::WindowMaximized);
    }
    else
    {
      this->showNormal();
      this->setWindowState(Qt::WindowNoState);
    }
  }
  void CanvanNEW::onCanvanMinimized()  { this->showMinimized(); }

  bool CanvanNEW::eventFilter(QObject *target, QEvent *event)
  {
    if (target == fileMenu)
    {
      //qDebug() << "CanvanNEW::eventFilter event: " << event;
      if (event->type() == QEvent::MouseButtonRelease)
      {
        isMoveing = false;
        return true;
      }
    }
    else if (target == menuBar)
    {
      if (event->type() == QEvent::MouseButtonPress)
      {
        // qDebug() << "CanvanNEW::eventFilter target: " << target;
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        isMoveing = true;
        cursorScreen = mouseEvent->globalPos();
        cursorCanvan = mouseEvent->pos();
        originCanvanScreen = QPoint(cursorScreen.x() - cursorCanvan.x(), cursorScreen.y() - cursorCanvan.y());

      }
      if (event->type() == QEvent::MouseMove)
      {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (this->windowState() == Qt::WindowNoState) {
          if (isMoveing)
          {
            QPoint mouseCurrentGlobal = mouseEvent->globalPos();
            int posX = originCanvanScreen.x() - cursorScreen.x() + mouseCurrentGlobal.x();
            int posY = originCanvanScreen.y() - cursorScreen.y() + mouseCurrentGlobal.y();
            this->move(posX, posY);
          }
        }
      }
      if (event->type() == QEvent::MouseButtonRelease) { isMoveing = false; }
    }

    return QMainWindow::eventFilter(target, event);
  }

  void CanvanNEW::resizeEvent(QResizeEvent *resizeEvent)
  {
    closeButton->setGeometry(width() - 36, 10, 24, 24);
    maximizeButton->setGeometry(width() - 60, 10, 24, 24);
    minimizeButton->setGeometry(width() - 84, 10, 24, 24);
  }
  void CanvanNEW::paintEvent(QPaintEvent *paintEvent)
  {
    QPainter painter(this);
    painter.fillRect(QRect(0, headerSize, width(), height() - headerSize), colorBackground);

    painter.setPen(QColor(150, 150, 150));
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
  }

  CanvanNEW::~CanvanNEW()
  {
    qDebug("CanvanNEW closed");
  }

  ///////////////////////////////////////////////////////////////////////////

  CanvanNEW_MenuBar::CanvanNEW_MenuBar(QWidget *parent) : QMenuBar(parent)
  {
    colorBackground = QColor(220, 120, 120);
  }
  void CanvanNEW_MenuBar::setColorBackground(QColor color)  { colorBackground = color; }
  QColor CanvanNEW_MenuBar::getColorBackground() { return colorBackground; }
  void CanvanNEW_MenuBar::paintEvent(QPaintEvent *event)
  {

    QPainter painter(this);
    painter.fillRect(QRect(1, 1, width() - 2, height() - 1), colorBackground);

    QMenuBar::paintEvent(event);

  }
  void CanvanNEW_MenuBar::resizeEvent(QResizeEvent *event)
  {
    this->setMask(QRegion(1, 1, width() - 2, height() - 1));
  }
  CanvanNEW_MenuBar::~CanvanNEW_MenuBar() { }

  ///////////////////////////////////////////////////////////////////////////

  CanvanNEW_StatusBar::CanvanNEW_StatusBar(QWidget *parent) : QStatusBar(parent) {
    colorBackground = QColor(120, 120, 120);
  }
  void CanvanNEW_StatusBar::setColorBackground(QColor color)  { colorBackground = color; }
  QColor CanvanNEW_StatusBar::getColorBackground() { return colorBackground; }
  void CanvanNEW_StatusBar::paintEvent(QPaintEvent *event)
  {

    QPainter painter(this);
    painter.fillRect(QRect(1, 1, width() - 2, height() - 2), colorBackground);

    QStatusBar::paintEvent(event);
  }
  CanvanNEW_StatusBar::~CanvanNEW_StatusBar() { }

  ///////////////////////////////////////////////////////////////////////////

  CanvanNEW_ToolBar::CanvanNEW_ToolBar(QWidget *parent) : QToolBar(parent)
  {
    this->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    this->setMinimumWidth(32);
    colorBackground = QColor(120, 120, 120);
  }
  void CanvanNEW_ToolBar::addButton(QString name, QImage icon, const QObject* reciver, const char* member)
  {
    QAction *action = new QAction(this);
    //action->rise
    this->addAction(action);


    Button *button = new Button(this->widgetForAction(action));
    button->setGeometry(0, 0, 28, 28);
    button->setToolTip(name);
    button->setIcon(QImage(icon), 0);
    button->setStateKeeping(Button::StateKeeping::HOLD);

    connect(button, SIGNAL(actPressed()), reciver, member);

    buttonsList.insert(name, button);
  }
  Button* CanvanNEW_ToolBar::getButton(QString name)  { return buttonsList.value(name); }
  void CanvanNEW_ToolBar::setColorBackground(QColor color)  { colorBackground = color; }
  QColor CanvanNEW_ToolBar::getColorBackground() { return colorBackground; }
  void CanvanNEW_ToolBar::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.fillRect(QRect(1, 1, width() - 2, height() - 2), colorBackground);

    // QToolBar::paintEvent(event);
  }
  CanvanNEW_ToolBar::~CanvanNEW_ToolBar() { }

}
