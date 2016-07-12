#include "Canvan.h"
#include "JuiConfig.h"

namespace Jui
{

  CanvanNEW::CanvanNEW(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
  {
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("CanvanNEW");

    headerSize = 50;
    isMoveing = false;

    this->initControl();
  }
  void CanvanNEW::initControl()
  {
    screen = new QWidget(this);
    this->setCentralWidget(screen);
    //this->setCorner(Qt::Corner::TopLeftCorner, Qt::DockWidgetArea::);
    menuBar = new QMenuBar(this);
    menuBar->installEventFilter(this);
    menuBar->setFixedHeight(headerSize);
    this->setMenuBar(menuBar);

    fileMenu = new QMenu("file");
    fileMenu->installEventFilter(this);
    menuBar->addMenu(fileMenu);

    fileMenu->addAction("setup");
    fileMenu->addAction("exit");

    console = new Console(this);

    testDock1 = new QDockWidget(this);
    testDock1->setWindowTitle("Console");
    //testDock1->setFixedWidth(300);
    //testDock1->setFeatures(QDockWidget::DockWidgetMovable);
    testDock1->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea); 
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, testDock1);
    
    //testDock1->setAllowedAreas(Qt::DockWidgetArea::RightDockWidgetArea);
    testDock1->setWidget(console);

    closeButton = new Button(menuBar);
    closeButton->setIcon(QImage(":/close16.png"), 0);
    connect(closeButton, SIGNAL(pressAct()), this, SLOT(onCanvanClosed()));

    maximizeButton = new Button(menuBar);
    maximizeButton->setIcon(QImage(":/maximize16.png"), 0);
    maximizeButton->setStateKeeping(Button::StateKeeping::HOLD);
    connect(maximizeButton, SIGNAL(pressAct()), this, SLOT(onCanvanMaximized()));

    minimizeButton = new Button(menuBar);
    minimizeButton->setIcon(QImage(":/minimize16.png"), 0);
    connect(minimizeButton, SIGNAL(pressAct()), this, SLOT(onCanvanMinimized()));
  }

  void CanvanNEW::setColor_Background_Header(QColor color)
  {
    QString style;
    style += tr("QMenuBar{background-color: %1; }").arg(color.name());
    style += tr("QMenuBar:item{background-color: %1; }").arg(color.name());
    menuBar->setStyleSheet(style);
  }

  void CanvanNEW::onCanvanClosed()
  {
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
    menuBar->setGeometry(QRect(1, 1, width() - 2, headerSize));
    fileMenu->setGeometry(QRect(150, 1, 30, 30));

    closeButton->setGeometry(width() - 36, 10, 24, 24);
    maximizeButton->setGeometry(width() - 60, 10, 24, 24);
    minimizeButton->setGeometry(width() - 84, 10, 24, 24);

  }
  void CanvanNEW::paintEvent(QPaintEvent *paintEvent)
  {
    QPainter painter(this);
    painter.fillRect(QRect(0, 0, width(), height()), QColor(30, 30, 30));

    painter.setPen(QColor(150, 150, 150));
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
  }

  CanvanNEW::~CanvanNEW()
  {
    qDebug("CanvanNEW closed");
  }


  Canvan::Canvan(QWidget *window) : QWidget(window)
  {
    win = window;
    setObjectName("Canvan");

    win->setWindowFlags(Qt::FramelessWindowHint);
    win->setWindowTitle("New Title");
    //win->window()->setWindowOpacity(0.95);

    // setGeometry(0, 0, win->width(), win->height());
    setAttribute(Qt::WA_NoMousePropagation);

    //this->setMouseTracking(true);

    mCursorGlobal = new QPoint(0, 0);
    mCursorLocal = new QPoint(0, 0);
    mFrameOriginGlobal = new QPoint(0, 0);

    //menu = new QMenuBar(this);
    header = new QWidget(this);
    screen = new QWidget(this);
    tail = new QWidget(this);

    headerSize = 100;
    tailSize = 50;
    isPressed = false;
    showScreen = false;

    this->initControl();

    connect(this, SIGNAL(consolePrintAct(QString, QColor, bool)), mConsole, SLOT(addText(QString, QColor, bool)));

    connect(closeButton, SIGNAL(pressAct()), this, SLOT(closeCanvan()));
    connect(maximizeButton, SIGNAL(pressAct()), this, SLOT(maximizeCanvan()));
    connect(minimizeButton, SIGNAL(pressAct()), this, SLOT(minimizeCanvan()));

    connect(mConsole, SIGNAL(resizeAct()), this, SLOT(fitScreen()));
  }

  void Canvan::initControl()
  {
    closeButton = new Button(header);
    closeButton->setIcon(QImage(":/close16.png"), 0);

    maximizeButton = new Button(header);
    maximizeButton->setIcon(QImage(":/maximize16.png"), 0);
    maximizeButton->setStateKeeping(Button::StateKeeping::HOLD);

    minimizeButton = new Button(header);
    minimizeButton->setIcon(QImage(":/minimize16.png"), 0);

    mConsole = new Console(this);
    mConsole->setTitle("Console");
    //mConsole->setColorText(QColor(70, 70, 70)); // default text color 

    mConsole->setGeometry(0, 0, 550, 150);

    edges = new Edges(this);
    setEdgeControler(EdgeControler::Direction::LEFT, true);
    setEdgeControler(EdgeControler::Direction::TOP, true);
    setEdgeControler(EdgeControler::Direction::RIGHT, true);
    setEdgeControler(EdgeControler::Direction::BOTTOM, true);
  }

  void Canvan::print(QString text, QColor col) { emit consolePrintAct(text, col, false); }
  void Canvan::println(QString text, QColor col) { emit consolePrintAct(text, col, true); }

  void Canvan::setEdgeControler(EdgeControler::Direction direction, bool visible)
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
    default:
      break;
    }
  }

  void Canvan::resizeEvent(QResizeEvent *resizeEvent)
  {
    this->setGeometry(0, 0, win->width(), win->height());

    closeButton->setGeometry(width() - 36, 10, 24, 24);
    maximizeButton->setGeometry(width() - 60, 10, 24, 24);
    minimizeButton->setGeometry(width() - 84, 10, 24, 24);

    header->setGeometry(0, 0, this->width(), headerSize);
    this->fitScreen();
    tail->setGeometry(0, this->height() - tailSize, this->width(), tailSize);

    mConsole->setGeometry(
      this->width() - mConsole->width(),
      headerSize + 1,
      mConsole->width(),
      this->height() - headerSize - tailSize - 1
      );

    emit resizeAct();

    //msgConsole(tr("resize [%1, %2]").arg(QString::number(width()), QString::number(height())));
  }

  void Canvan::fitScreen()
  {
    screen->setGeometry(
      0,
      headerSize,
      this->width() - mConsole->width(),
      this->height() - tailSize - headerSize
      );
    emit resizeScreenAct();
  }

  void Canvan::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setFont(QFont("Univers Condensed", 10, QFont::Normal));

    painter.setPen(Qt::NoPen);
    painter.fillRect(header->geometry(), colorAppHeaderBackground);
    painter.fillRect(tail->geometry(), colorAppHeaderBackground);

    painter.setPen(QPen(colorNormal, 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

    if (showScreen)
    {
      painter.setPen(QColor(230, 30, 30));
      painter.drawLine(screen->geometry().topLeft(), screen->geometry().bottomRight());
      painter.drawLine(screen->geometry().bottomLeft(), screen->geometry().topRight());
    }

    QRectF target(5, 5, logo.width(), logo.height());
    QRectF source(0, 0, logo.width(), logo.height());

    QImage renderedIcon(logo);
    renderedIcon.fill(colorNormal);
    renderedIcon.setAlphaChannel(logo);
    painter.drawImage(target, renderedIcon, source);

    painter.setPen(QPen(colorText, 1));
    QTextOption opt;
    opt.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    painter.drawText(tail->geometry().adjusted(tail->width() - 100, 5, -10, -5), version, opt);
  }

  void Canvan::mousePressEvent(QMouseEvent *mouseEvent)
  {
    isPressed = true;
    *mCursorGlobal = mouseEvent->globalPos();
    *mCursorLocal = mouseEvent->pos();
    *mFrameOriginGlobal = QPoint(
      mCursorGlobal->x() - mCursorLocal->x(),
      mCursorGlobal->y() - mCursorLocal->y()
      );
    /*
    emit consolePrintAct(
    tr("pressedGlobal [%1,%2]").arg(
    QString::number(mCursorGlobal->x()),
    QString::number(mCursorGlobal->y())
    ), QColor(120, 120, 120), true);
    */
  }

  void Canvan::mouseMoveEvent(QMouseEvent *mouseEvent)
  {
    if (win->windowState() == Qt::WindowNoState) {
      if (isPressed)
      {
        QPoint mouseCurrentGlobal = mouseEvent->globalPos();
        int posX = mFrameOriginGlobal->x() - mCursorGlobal->x() + mouseCurrentGlobal.x();
        int posY = mFrameOriginGlobal->y() - mCursorGlobal->y() + mouseCurrentGlobal.y();
        //msgConsole(tr("mCursor [%1,%2]").arg(QString::number(posX), QString::number(posY)));
        win->move(posX, posY);
      }
    }
  }

  void Canvan::mouseReleaseEvent(QMouseEvent *mouseEvent) { isPressed = false; }

  void Canvan::addScreen(QWidget *inScreen)
  {
    inScreen->setGeometry(100, 100, 300, 300);
    screen = inScreen;
  }

  void Canvan::setHeaderHeight(int height) { headerSize = height; }
  void Canvan::setTailHeight(int height) { tailSize = height; }
  void Canvan::setLogo(QImage img) { logo = img; }
  void Canvan::setTitle(QString name)	{ win->setWindowTitle(name); }
  void Canvan::setVersion(QString _version)
  {
    /*
  QString strMajor = QString::number(major);
  QString strMinor = QString::number(minor);
  QString strPatch = QString::number(patch);
  if (patch < 10) { strPatch.prepend(QString::number(0)); }
  */
    version = _version;
  }

  void Canvan::setColorHeader(QColor color) { colorAppHeaderBackground = color; }
  void Canvan::setColorNormal(QColor color)
  {
    colorNormal = color;
    closeButton->setColorNormal(colorNormal);
    maximizeButton->setColorNormal(colorNormal);
    minimizeButton->setColorNormal(colorNormal);
  }
  void Canvan::setColorOver(QColor color)
  {
    colorOver = color;
    closeButton->setColorOver(colorOver);
    maximizeButton->setColorOver(colorOver);
    minimizeButton->setColorOver(colorOver);
  }
  void Canvan::setColorActive(QColor color)
  {
    colorActive = color;

    closeButton->setColorActive(colorActive);
    maximizeButton->setColorActive(colorActive);
    minimizeButton->setColorActive(colorActive);
  }
  void Canvan::setColorText(QColor color) { colorText = color; }

  void Canvan::closeCanvan() {
    //	win->close();
    emit closeAct();
  }
  void Canvan::minimizeCanvan()
  {
    qDebug() << "Minimize canvan";
    //emit minimizeAct();
    win->showMinimized();
  }
  void Canvan::maximizeCanvan()
  {
    qDebug() << "Canvan state" << win->windowState();
    if (win->windowState() == Qt::WindowNoState) {
      qDebug() << "Maximize canvan";

      canvanOrigin = win->geometry().topLeft();
      canvanSize = win->geometry().size();

      win->setWindowState(Qt::WindowMaximized);
      QRect scr = QDesktopWidget().availableGeometry();
      win->move(0, 0);
      edges->edgeMoved(Jui::EdgeControler::Direction::RIGHT, scr.width() - 1);
      edges->edgeMoved(Jui::EdgeControler::Direction::BOTTOM, scr.height() - 1);
      edges->hide();
    }
    else if (win->windowState() == Qt::WindowMaximized)
    {
      win->setWindowState(Qt::WindowNoState);
      edges->edgeMoved(Jui::EdgeControler::Direction::RIGHT, canvanSize.width());
      edges->edgeMoved(Jui::EdgeControler::Direction::BOTTOM, canvanSize.height());
      win->move(canvanOrigin);
      edges->show();
    }
  }
  /*
  void Canvan::customizeEvent(QDynamicPropertyChangeEvent *event)
  {
  qDebug("Canvan::customizeEvent");
  }
  */

  Canvan::~Canvan() {	}
}
