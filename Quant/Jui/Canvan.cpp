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

		setGeometry(0, 0, win->width(), win->height());
		setAttribute(Qt::WA_NoMousePropagation);

		//this->setMouseTracking(true);

		mCursorGlobal = new QPoint(0, 0);
		mCursorLocal = new QPoint(0, 0);
		mFrameOriginGlobal = new QPoint(0, 0);

		//menu = new QMenuBar(this);
		header = new QWidget(this);
		screen = new QWidget(this);
		tail = new QWidget(this);

		QFontDatabase::addApplicationFont(":/fontText.ttf");
		QFontDatabase::addApplicationFont(":/fontCode.ttf");

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

	void Canvan::onConfigData(QMap<QString, QVariant*> config)
	{

		connect(this, SIGNAL(actConfigData(QMap<QString, QVariant*>)),
			mConsole, SLOT(onConfigData(QMap<QString, QVariant*>)));

		colorAppHeaderBackground = config.value("shem_colorAppHeaderBackground")->value<QColor>();
		colorPanelBackground = config.value("shem_colorPanelBackground")->value<QColor>();
		colorNormal = config.value("shem_colorNormal")->value<QColor>();
		colorOver = config.value("shem_colorOver")->value<QColor>();
		colorActive = config.value("shem_colorActive")->value<QColor>();
		colorText = config.value("shem_colorText")->value<QColor>();
		fontTextSmall = config.value("shem_fontTextSmall")->value<QFont>();
		fontCode = config.value("shem_fontCode")->value<QFont>();

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

		// STYLESHEET SETUP
		QString txt;
		txt.append(tr("QLabel { color: %1; }").arg(colorText.name()));

		txt.append(tr("QPushButton { background-color: %1; }").arg(colorPanelBackground.name()));
		txt.append(tr("QPushButton { color: %1; }").arg(colorText.name()));
		txt.append(tr("QPushButton { border-style: outset; border-width: 1px; border-color: %1}").arg(colorText.name()));
		txt.append(tr("QPushButton:pressed{ background-color: %1; border-style: inset; }").arg(colorActive.name()));

		txt.append(tr("QTextEdit { color: %1; }").arg(colorText.name()));
		txt.append(tr("QTextEdit { background-color: %1; }").arg(colorPanelBackground.name()));
		txt.append(tr("QTextEdit { selection-background-color: %1; }").arg(colorActive.name()));

		txt.append("QScrollBar:vertical { width: 2px; }");
		txt.append("QScrollBar:horizontal { height: 2px; }");
		txt.append(tr("QScrollBar:vertical { background: %1; }").arg(colorPanelBackground.name()));
		txt.append(tr("QScrollBar:horizontal { background: %1; }").arg(colorPanelBackground.name()));
		txt.append(tr("QScrollBar::handle:vertical{	background: %1;	min-height: 40px; }").arg(colorText.name()));
		txt.append(tr("QScrollBar::handle:horizontal{ background: %1; min-height: 40px; }").arg(colorText.name()));
		txt.append("QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }");
		txt.append("QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }");
		txt.append("QScrollBar::right-arrow:horizontal, QScrollBar::left-arrow:horizontal {	border: none; background: none;	color: none; }");
		txt.append("QScrollBar::top-arrow:vertical, QScrollBar::bottom-arrow:vertical {	border: none; background: none;	color: none; }");
		txt.append("QScrollBar::add-line:horizontal { border: none; background: none; }");
		txt.append("QScrollBar::sub-line:horizontal { border: none;	background: none; }");
		txt.append("QScrollBar::add-line:vertical { border: none; background: none; }");
		txt.append("QScrollBar::sub-line:vertical { border: none;	background: none; }");

		txt.append(tr("QToolTip { color: %1; }").arg(colorText.name()));
		txt.append(tr("QToolTip { background-color:  %1; }").arg(colorPanelBackground.name()));
		txt.append(tr("QToolTip { border: 1px solid white; }"));

		this->setStyleSheet(txt);

		emit actConfigData(config);

		update();
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

		emit consolePrintAct(
			tr("pressedGlobal [%1,%2]").arg(
			QString::number(mCursorGlobal->x()),
			QString::number(mCursorGlobal->y())
			), QColor(120, 120, 120), true);

	}

	void Canvan::mouseMoveEvent(QMouseEvent *mouseEvent)
	{
		if (isPressed)
		{
			QPoint mouseCurrentGlobal = mouseEvent->globalPos();
			int posX = mFrameOriginGlobal->x() - mCursorGlobal->x() + mouseCurrentGlobal.x();
			int posY = mFrameOriginGlobal->y() - mCursorGlobal->y() + mouseCurrentGlobal.y();
			//msgConsole(tr("mCursor [%1,%2]").arg(QString::number(posX), QString::number(posY)));

			win->move(posX, posY);
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
	void Canvan::setVersion(int major = 0, int minor = 0, int patch = 0)
	{
		QString strMajor = QString::number(major);
		QString strMinor = QString::number(minor);
		QString strPatch = QString::number(patch);
		if (patch < 10) { strPatch.prepend(QString::number(0)); }
		version = tr("v%1.%2%3").arg(strMajor, strMinor, strPatch);
	}

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
			win->move(0, 0);
			win->showMaximized();
			win->setWindowState(Qt::WindowMaximized);
		}
		if (win->windowState() == Qt::WindowMaximized) { win->showNormal(); }


		//resize(win->rect().size());
		//setGeometry(0, 0, win->width(), win->height());
		//		win->move(0, 0);
		//	win->resize(Qt:showFullScreen win->width(), win->height());
		//resizeCanvan();
		//emit resizeAct();
		//update();
	}

	Canvan::~Canvan() {	}
}
