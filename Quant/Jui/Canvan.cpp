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

		//this->setStyleSheet("QTextEdit{background-color: QColor(Qt::transparent);}");
		this->initControl();
		//this->setCanvanPalette();
		this->setCanvanStyleSheet();

		connect(this, SIGNAL(sendToConsole(QString)), panelConsole, SLOT(addLine(QString)));
		connect(closeButton, SIGNAL(pressAct()), this, SLOT(closeCanvan()));

		msgConsole(tr("start"));
	}

	void Canvan::initControl()
	{
		edges = new Edges(this);

		closeButton = new Button(header);
		closeButton->setIcon(QImage(":/close16.png"), 0);

		maximizeButton = new Button(header);
		maximizeButton->setIcon(QImage(":/maximize16.png"), 0);

		minimizeButton = new Button(header);
		minimizeButton->setIcon(QImage(":/minimize16.png"), 0);

		panelConsole = new Console(this);
		//panelConsole->installEventFilter(this);
		panelConsole->setTitle("Console");
		panelConsole->setBackground(QColor(30, 30, 30));
		panelConsole->setGeometry(0, 0, 150, 150);
	}

	void Canvan::resizeEvent(QResizeEvent *resizeEvent)
	{
		this->setGeometry(0, 0, win->width(), win->height());

		closeButton->setGeometry(width() - 35, 10, 25, 25);
		maximizeButton->setGeometry(width() - 65, 10, 25, 25);
		minimizeButton->setGeometry(width() - 95, 10, 25, 25);

		header->setGeometry(0, 0, this->width(), headerSize);
		screen->setGeometry(0, headerSize, this->width(), this->height() - tailSize - headerSize);
		tail->setGeometry(0, this->height() - tailSize, this->width(), tailSize);

		panelConsole->setGeometry(this->width() - 510, headerSize + 5, 500, this->height() - headerSize - tailSize - 10);

		emit resizeAct();

		msgConsole(tr("resize [%1, %2]").arg(QString::number(width()), QString::number(height())));
	}

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

		//painter.setBrush(QBrush(QColor(130, 130, 30), Qt::SolidPattern));
		//painter.drawText(100, 15, title->text());

		QRectF target(5, 5, 56, 43);
		QRectF source(0, 0, 168, 128);
		painter.drawImage(target, logo, source);

		//qDebug("Canvan::paintEvent");
		//msgConsole("Canvan draw...");
	}

	void Canvan::setHeaderHeight(int height) { headerSize = height; }

	void Canvan::setTailHeight(int height) { tailSize = height; }

	void Canvan::setLogo(QImage img) { logo = img; }

	void Canvan::mousePressEvent(QMouseEvent *mouseEvent)
	{
		*mCursorGlobal = mouseEvent->globalPos();
		*mCursorLocal = mouseEvent->pos();
		*mFrameOriginGlobal = QPoint(
			mCursorGlobal->x() - mCursorLocal->x(),
			mCursorGlobal->y() - mCursorLocal->y()
			);

		msgConsole(tr("pressedGlobal [%1,%2]").arg(QString::number(mCursorGlobal->x()), QString::number(mCursorGlobal->y())));
		msgConsole(tr("pressedLocal [%1,%2]").arg(QString::number(mCursorLocal->x()), QString::number(mCursorLocal->y())));
		msgConsole(tr("frameOriginGlobal [%1,%2]").arg(QString::number(mFrameOriginGlobal->x()), QString::number(mFrameOriginGlobal->y())));
	}

	void Canvan::mouseMoveEvent(QMouseEvent *mouseEvent)
	{
		QPoint mouseCurrentGlobal = mouseEvent->globalPos();
		int posX = mFrameOriginGlobal->x() - mCursorGlobal->x() + mouseCurrentGlobal.x();
		int posY = mFrameOriginGlobal->y() - mCursorGlobal->y() + mouseCurrentGlobal.y();

		msgConsole(tr("mCursor [%1,%2]").arg(QString::number(posX), QString::number(posY)));

		win->move(posX, posY);
	}



	void Canvan::addScreen(QWidget *inScreen)
	{
		inScreen->setGeometry(100, 100, 300, 300);
		screen = inScreen;
	}

	void Canvan::setColor_background(QColor color)
	{
		msgConsole(QString::number(color.red()));
		msgConsole(QString::number(color.green()));
		msgConsole(QString::number(color.blue()));

		QString strR = QString::number(color.red());
		QString strG = QString::number(color.green());
		QString strB = QString::number(color.blue());
		QString strR2 = QString::number(color.red() + 10);
		QString strG2 = QString::number(color.green() + 10);
		QString strB2 = QString::number(color.blue() + 10);


		QString strBackColor = tr("background-color: QColor(%1,%2,%3);").arg(strR, strG, strB);
		QString strBackColor2 = tr("background-color: QColor(%1,%2,%3);").arg(strR2, strG2, strB2);

		QString strFrontColor = tr("color: QColor(%1,%2,%3);").arg(strR, strG, strB);

		screen->setStyleSheet(
			tr("background-color: QColor(%1,%2,%3); color: white;").arg(strR, strG, strB)
			);
		panelConsole->setStyleSheet(strBackColor);
		//menu->setStyleSheet(strBackColor2);
		tail->setStyleSheet(strBackColor2);

	}

	void Canvan::setColor_foreground(QColor color)
	{
		msgConsole(QString::number(color.red()));
		msgConsole(QString::number(color.green()));
		msgConsole(QString::number(color.blue()));

		QString strR = QString::number(color.red());
		QString strG = QString::number(color.green());
		QString strB = QString::number(color.blue());

		QString strFrontColor = tr("color: QColor(%1,%2,%3);").arg(strR, strG, strB);

		screen->setStyleSheet(strFrontColor);
		panelConsole->setStyleSheet(strFrontColor);
		//menu->setStyleSheet(strFrontColor);
		//tail->setStyleSheet(strFrontColor);

	}

	void Canvan::msgConsole(QString text)
	{
		emit sendToConsole(text);
	}



	/*
	void Canvan::setTitle(QString name)
	{
	title = new QLabel(header);
	title->setGeometry(10, 10, 100, 50);
	title->setText(name);
	}

	void Canvan::setVersion(int major = 0, int minor = 0, int patch = 0)
	{
	QString text = tr("version %1.%2%3").arg(QString::number(major), QString::number(minor), QString::number(patch));

	version = new QLabel(this);
	version->setGeometry(300, 300, 100, 50);
	version->setText(text);
	//tail->showMessage(text);
	}
	*/

	void Canvan::setCanvanStyleSheet()
	{
		backColor = QColor(30, 30, 30);
		panelColor = QColor(20, 20, 20);
		textColor = QColor(230, 230, 230);
		activeColor = QColor(20, 180, 240);
		
		//qDebug() << "backHex:" << backColor.name();
		
		QString txt;
		txt.append(tr("QTextEdit { color: %1; background-color: %2; selection-background-color: %3;}").arg(textColor.name(), backColor.name(), activeColor.name()));
		
		txt.append("QScrollBar:vertical { border: 2px solid grey; background: #32CC99; width: 15px;	margin: 22px 0 22px 0; }");

		qDebug() << "TXT:" << txt;
		
		this->setStyleSheet(txt);
		
		//this->setStyleSheet();
			/*
			"QTextEdit{background-color: };}"

			"QScrollBar:vertical{ border: 2px solid grey; background: #32CC99; width: 15px;	margin: 22px 0 22px 0; }"
			"QScrollBar::handle:vertical{background: white;min-height: 20px;}"
			"QScrollBar::add-line:vertical{border: 2px solid grey; background: #32CC99; height: 20px; subcontrol-position: bottom; subcontrol-origin: margin;}"
			"QScrollBar::sub-line:vertical{border: 2px solid grey; background: #32CC99; height: 20px; subcontrol-position: top; subcontrol-origin: margin;}"
			"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow : vertical{ border: 2px solid grey; width: 3px; height: 3px; background: white;}"
			);
			*/
	}

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

	void Canvan::closeCanvan() { /*close();*/ win->close(); }
	void Canvan::minimizeCanvan()
	{
		//emit minimizeAct();
		//showMinimized();
	}
	void Canvan::maximizeCanvan()
	{
		//qDebug() << "Maximize canvan";
		//resizeCanvan();
		//emit maximizeAct();
	}


	Canvan::~Canvan()
	{

	}

}
