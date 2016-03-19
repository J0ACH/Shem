#include "Canvan.h"
#include "JuiConfig.h"

namespace Jui
{
	Canvan::Canvan(QWidget *window) : QWidget(window)
	{
		parent = window;


#ifdef JUI_CANVAN_SYSTEMFRAME
		//parent->setWindowFlags(Qt::CustomizeWindowHint);
		parent->setWindowFlags(Qt::FramelessWindowHint);
#else
		parent->setWindowFlags(Qt::FramelessWindowHint);
#endif
		parent->setWindowTitle("New Title");
		parent->window()->setWindowOpacity(0.95);

		setGeometry(0, 0, parent->width(), parent->height());

		mCursorGlobal = new QPoint(0, 0);
		mCursorLocal = new QPoint(0, 0);
		mFrameOriginGlobal = new QPoint(0, 0);

		//menu = new QMenuBar(this);
		header = new QWidget(this);
		screen = new QWidget(this);
		tail = new QWidget(this);

		headerSize = 100;
		tailSize = 50;

		this->initControl();
		
		//closeButton->setIcon(QString(":/close.png"));
		//closeButton->show();
		//console = new QDockWidget(QString("Console"), this);

		dock = new QDockWidget(tr("Console"), screen);
		dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

		console = new Console(dock);
		dock->setWidget(console);
		//addDockWidget(Qt::RightDockWidgetArea, dock);
		

		connect(this, SIGNAL(sendToConsole(QString)), console, SLOT(addLine(QString)));
		connect(closeButton, SIGNAL(pressAct()), this, SLOT(closeCanvan()));

		msgConsole(tr("start"));
	}

	void Canvan::initControl()
	{
		closeButton = new Button(header);
		minimizeButton = new Button(header);
		maximizeButton = new Button(header);

		panelConsole = new Panel(this);
		//panelConsole->installEventFilter(this);
		panelConsole->setTitle("Console");
		panelConsole->setBackground(QColor(30, 30, 30));
		panelConsole->setGeometry(0, 0, 150, 150);
	}

	void Canvan::resizeEvent(QResizeEvent *resizeEvent)
	{
		this->setGeometry(0, 0, parent->width(), parent->height());

		closeButton->setGeometry(width() - 40, 10, 30, 30);
		maximizeButton->setGeometry(width() - 75, 10, 30, 30);
		minimizeButton->setGeometry(width() - 110, 10, 30, 30);

		header->setGeometry(0, 0, this->width(), headerSize);
		screen->setGeometry(0, headerSize, this->width(), this->height() - tailSize - headerSize);
		tail->setGeometry(0, this->height() - tailSize, this->width(), tailSize);

		panelConsole->setGeometry(this->width() - 300, headerSize + 5, 300, this->height() - headerSize - tailSize - 10);

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

		painter.setPen(Qt::NoPen);
		painter.fillRect(screen->geometry(), QColor(20, 20, 20));
		painter.fillRect(header->geometry(), QColor(40, 40, 40));
		painter.fillRect(tail->geometry(), QColor(40, 40, 40));
		
		painter.setPen(QPen(QColor(200, 200, 200), 1));
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
		
		painter.setBrush(QBrush(QColor(130, 130, 30), Qt::SolidPattern));
		painter.drawText(100, 15, title->text());
		
		//qDebug("Canvan::paintEvent");
		//msgConsole("Canvan draw...");
	}

	void Canvan::setHeaderHeight(int height)
	{
		headerSize = height;
	}

	void Canvan::setTailHeight(int height)
	{
		tailSize = height;
	}





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

		parent->move(posX, posY);
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
		console->setStyleSheet(strBackColor);
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
		console->setStyleSheet(strFrontColor);
		//menu->setStyleSheet(strFrontColor);
		//tail->setStyleSheet(strFrontColor);

	}

	void Canvan::msgConsole(QString text)
	{
		emit sendToConsole(text);
	}

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


	void Canvan::mySetPalette()
	{
		//	menu->setStyle(QStyleFactory::create("Fusion"));

		backColor = new QColor(40, 40, 40);
		panelColor = new QColor(20, 20, 20);
		textColor = new QColor(230, 230, 230);
		activeColor = new QColor(20, 180, 240);

		palette = new QPalette();
		palette->setColor(QPalette::Window, *backColor);
		palette->setColor(QPalette::WindowText, Qt::white);
		palette->setColor(QPalette::Base, Qt::transparent);
		palette->setColor(QPalette::AlternateBase, QColor(53, 53, 53));
		palette->setColor(QPalette::ToolTipBase, Qt::white);
		palette->setColor(QPalette::ToolTipText, Qt::white);
		palette->setColor(QPalette::WindowText, *textColor);
		palette->setColor(QPalette::Text, *textColor);
		palette->setColor(QPalette::Highlight, *activeColor);
		palette->setColor(QPalette::Button, Qt::transparent);
		palette->setColor(QPalette::Background, *panelColor);

		//palette->setBrush(QPalette::Button, QBrush((130, 30, 30), Qt::SolidPattern));

		qApp->setPalette(*palette);


		//qApp->setStyle(QStyleFactory::create("Fusion"));
		/*
		QPalette darkPalette;
		darkPalette.setColor(QPalette::Window, QColor(53,53,53));
		darkPalette.setColor(QPalette::WindowText, Qt::white);
		darkPalette.setColor(QPalette::Base, QColor(25,25,25));
		darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
		darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
		darkPalette.setColor(QPalette::ToolTipText, Qt::white);
		darkPalette.setColor(QPalette::Text, Qt::white);
		darkPalette.setColor(QPalette::Button, QColor(53,53,53));
		darkPalette.setColor(QPalette::ButtonText, Qt::white);
		darkPalette.setColor(QPalette::BrightText, Qt::red);
		darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

		darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
		darkPalette.setColor(QPalette::HighlightedText, Qt::black);

		//	setPalette(darkPalette);
		qApp->setPalette(darkPalette);

		// qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
		*/
	}

	void Canvan::closeCanvan() { /*close();*/ parent->close(); }
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
