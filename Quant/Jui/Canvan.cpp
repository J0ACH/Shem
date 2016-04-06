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

		headerSize = 100;
		tailSize = 50;
		isPressed = false;
		showScreen = false;

		this->initControl();
		this->setCanvanStyleSheet();

		connect(this, SIGNAL(sendToConsole(QString)), panelConsole, SLOT(addLine(QString)));

		connect(closeButton, SIGNAL(pressAct()), this, SLOT(closeCanvan()));
		connect(maximizeButton, SIGNAL(pressAct()), this, SLOT(maximizeCanvan()));
		connect(minimizeButton, SIGNAL(pressAct()), this, SLOT(minimizeCanvan()));

		connect(panelConsole, SIGNAL(resizeAct()), this, SLOT(fitScreen()));
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

		version = new QLabel(tail);
		version->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

		panelConsole = new Console(this);
		panelConsole->setTitle("Console");
		panelConsole->setBackground(QColor(30, 30, 30));
		panelConsole->setGeometry(0, 0, 500, 150);

		edges = new Edges(this);
		setEdgeControler(EdgeControler::Direction::LEFT, true);
		setEdgeControler(EdgeControler::Direction::TOP, true);
		setEdgeControler(EdgeControler::Direction::RIGHT, true);
		setEdgeControler(EdgeControler::Direction::BOTTOM, true);
	}

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

		closeButton->setGeometry(width() - 35, 10, 25, 25);
		maximizeButton->setGeometry(width() - 65, 10, 25, 25);
		minimizeButton->setGeometry(width() - 95, 10, 25, 25);

		header->setGeometry(0, 0, this->width(), headerSize);
		this->fitScreen();
		tail->setGeometry(0, this->height() - tailSize, this->width(), tailSize);

		version->setGeometry(tail->width() - 180, 0, 170, tail->height() - 5);

		panelConsole->setGeometry(
			this->width() - panelConsole->width(),
			headerSize + 1,
			panelConsole->width(),
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
			this->width() - panelConsole->width(),
			this->height() - tailSize - headerSize
			);
		emit resizeScreenAct();
	}

	void Canvan::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);

		painter.setPen(Qt::NoPen);
		//painter.fillRect(screen->geometry(), QColor(20, 20, 20));
		painter.fillRect(header->geometry(), QColor(40, 40, 40));
		painter.fillRect(tail->geometry(), QColor(40, 40, 40));

		painter.setPen(QPen(QColor(200, 200, 200), 1));
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

		if (showScreen)
		{
			painter.setPen(QColor(230, 30, 30));
			painter.drawLine(screen->geometry().topLeft(), screen->geometry().bottomRight());
			painter.drawLine(screen->geometry().bottomLeft(), screen->geometry().topRight());
		}

		//painter.setBrush(QBrush(QColor(130, 130, 30), Qt::SolidPattern));
		//painter.drawText(100, 15, title->text());
		/*
		QRectF target(5, 5, 56, 43);
		QRectF source(0, 0, 168, 128);
		painter.drawImage(target, logo, source);
		*/

		QRectF target(10, 10, logo.width(), logo.height());
		QRectF source(0, 0, logo.width(), logo.height());

		QImage renderedIcon(logo);
		renderedIcon.fill(QColor(120,120,120));

		renderedIcon.setAlphaChannel(logo);
		painter.drawImage(target, renderedIcon, source);

		//painter.drawRect(target);

		//qDebug("Canvan::paintEvent");
		//msgConsole("Canvan draw...");
	}

	void Canvan::setHeaderHeight(int height) { headerSize = height; }

	void Canvan::setTailHeight(int height) { tailSize = height; }

	void Canvan::setLogo(QImage img) { logo = img; }

	void Canvan::setTitle(QString name)	{ win->setWindowTitle(name); }

	void Canvan::mousePressEvent(QMouseEvent *mouseEvent)
	{
		isPressed = true;
		*mCursorGlobal = mouseEvent->globalPos();
		*mCursorLocal = mouseEvent->pos();
		*mFrameOriginGlobal = QPoint(
			mCursorGlobal->x() - mCursorLocal->x(),
			mCursorGlobal->y() - mCursorLocal->y()
			);

		msgConsole(tr("pressedGlobal [%1,%2]").arg(QString::number(mCursorGlobal->x()), QString::number(mCursorGlobal->y())));
		//	msgConsole(tr("pressedLocal [%1,%2]").arg(QString::number(mCursorLocal->x()), QString::number(mCursorLocal->y())));
		//	msgConsole(tr("frameOriginGlobal [%1,%2]").arg(QString::number(mFrameOriginGlobal->x()), QString::number(mFrameOriginGlobal->y())));
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

	void Canvan::setColor_background(QColor color) { }

	void Canvan::setColor_foreground(QColor color) { }

	void Canvan::msgConsole(QString text) { emit sendToConsole(text); }

	void Canvan::setVersion(int major = 0, int minor = 0, int patch = 0)
	{
		QString text = tr("v%1.%2%3").arg(QString::number(major), QString::number(minor), QString::number(patch));
		version->setText(text);
	}

	void Canvan::setCanvanStyleSheet()
	{
		backColor = QColor(30, 30, 30);
		panelColor = QColor(20, 20, 20);
		textColor = QColor(230, 230, 230);
		activeColor = QColor(20, 180, 240);

		QString txt;
		txt.append(tr("QLabel { color: %1; }").arg(textColor.name()));

		txt.append(tr("QPushButton { background-color: %1; }").arg(backColor.name()));
		txt.append(tr("QPushButton { color: %1; }").arg(textColor.name()));
		txt.append(tr("QPushButton { border-style: outset; border-width: 1px; border-color: %1}").arg(textColor.name()));
		txt.append(tr("QPushButton:pressed{ background-color: %1; border-style: inset; }").arg(activeColor.name()));

		txt.append(tr("QTextEdit { color: %1; }").arg(textColor.name()));
		txt.append(tr("QTextEdit { background-color: %1; }").arg(backColor.name()));
		txt.append(tr("QTextEdit { selection-background-color: %1; }").arg(activeColor.name()));

		txt.append("QScrollBar:vertical { width: 5px; }");
		txt.append("QScrollBar:horizontal { height: 5px; }");
		txt.append(tr("QScrollBar:vertical { background: %1; }").arg(backColor.name()));
		txt.append(tr("QScrollBar::handle:vertical{	background: %1;	min-height: 20px; }").arg(textColor.name()));
		//txt.append("QScrollBar:vertical { margin: 50px 0 50px 0; }");

		//qDebug() << "TXT:" << txt;
		this->setStyleSheet(txt);
	}

	void Canvan::closeCanvan() { win->close(); }
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

	/*
	void Canvan::setCanvanPalette()
	{
	backColor = QColor(40, 40, 40);
	panelColor = QColor(20, 20, 20);
	textColor = QColor(230, 230, 230);
	activeColor = QColor(20, 180, 240);

	palette = new QPalette();
	palette->setColor(QPalette::Window, backColor);
	palette->setColor(QPalette::WindowText, Qt::white);
	palette->setColor(QPalette::Base, Qt::transparent);
	palette->setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	palette->setColor(QPalette::ToolTipBase, Qt::white);
	palette->setColor(QPalette::ToolTipText, Qt::white);
	palette->setColor(QPalette::WindowText, textColor);
	palette->setColor(QPalette::Text, textColor);
	palette->setColor(QPalette::Highlight, activeColor);
	palette->setColor(QPalette::Button, Qt::transparent);
	palette->setColor(QPalette::Background, panelColor);

	this->setPalette(*palette);
	}
	*/

	/*
	bool Canvan::eventFilter(QObject* target, QEvent* event)
	{
	if (target == panelConsole) {
	if (event->type() == QEvent::Resize) {

	//QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
	QResizeEvent *resizeEvent = static_cast<QResizeEvent*>(event);
	qDebug() << "Canvan: ConsolePanel resized" << resizeEvent->size();
	return true;
	}
	else {
	return false;
	}

	if (event->type() == QEvent::Paint) {

	QPaintEvent *paintEvent = static_cast<QPaintEvent*>(event);
	qDebug() << "Canvan: ConsolePanel paint";// << paintEvent->rect();
	}
	else
	{

	return false;
	}
	}
	else {
	// pass the event on to the parent class
	return QWidget::eventFilter(target, event);
	}


	}
	*/


	Canvan::~Canvan()
	{

	}

}
