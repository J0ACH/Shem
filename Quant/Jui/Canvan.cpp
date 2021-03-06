#include "Canvan.h"
#include "JuiConfig.h"

namespace Jui
{
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


  /*
  void Canvan::onConfigData(QMap<QString, QVariant*> config)
  {

  connect(this, SIGNAL(actConfigData(QMap<QString, QVariant*>)),
  mConsole, SLOT(onConfigData(QMap<QString, QVariant*>)));

  colorAppHeaderBackground = config.value("color_shem_AppHeaderBackground")->value<QColor>();
  colorPanelBackground = config.value("color_shem_PanelBackground")->value<QColor>();
  colorNormal = config.value("color_shem_Normal")->value<QColor>();
  colorOver = config.value("color_shem_Over")->value<QColor>();
  colorActive = config.value("color_shem_Active")->value<QColor>();
  colorText = config.value("color_shem_Text")->value<QColor>();
  fontTextSmall = config.value("font_shem_TextSmall")->value<QFont>();
  fontCode = config.value("font_shem_TextCode")->value<QFont>();

  mConsole->setFont(fontTextSmall);

  closeButton->setColorNormal(colorNormal);
  maximizeButton->setColorNormal(colorNormal);
  minimizeButton->setColorNormal(colorNormal);

  closeButton->setColorOver(colorOver);
  maximizeButton->setColorOver(colorOver);
  minimizeButton->setColorOver(colorOver);

  closeButton->setColorActive(colorActive);
  maximizeButton->setColorActive(colorActive);
  minimizeButton->setColorActive(colorActive);

  emit actConfigData(config);

  update();
  }
  void Canvan::onCustomize()
  {
  qDebug("Canvan::onCustomize");

  colorAppHeaderBackground = this->property("color_shem_AppHeaderBackground").value<QColor>();
  colorPanelBackground = this->property("color_shem_PanelBackground").value<QColor>();
  colorNormal = this->property("color_shem_Normal").value<QColor>();
  colorOver = this->property("color_shem_Over").value<QColor>();
  colorActive = this->property("color_shem_Active").value<QColor>();
  colorText = this->property("color_shem_Text").value<QColor>();
  fontTextSmall = this->property("font_shem_TextSmall").value<QFont>();
  fontCode = this->property("font_shem_TextCode").value<QFont>();

  mConsole->setFont(fontTextSmall);

  closeButton->setColorNormal(colorNormal);
  maximizeButton->setColorNormal(colorNormal);
  minimizeButton->setColorNormal(colorNormal);

  closeButton->setColorOver(colorOver);
  maximizeButton->setColorOver(colorOver);
  minimizeButton->setColorOver(colorOver);

  closeButton->setColorActive(colorActive);
  maximizeButton->setColorActive(colorActive);
  minimizeButton->setColorActive(colorActive);
  }
  */

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
