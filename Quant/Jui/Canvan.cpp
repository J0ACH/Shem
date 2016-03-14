#include "Canvan.h"
#include "JuiConfig.h"

namespace Jui
{
	/*
	Canvan::Canvan(int originX, int originY, int sizeX, int sizeY)
	: QMainWindow()
	{
	*/

	Canvan::Canvan(QWidget *window) : QWidget(window)
	{
		parent = window;

		//parent->resizeEvent(QResizeEvent *event);

#ifdef JUI_CANVAN_SYSTEMFRAME
		parent->setWindowFlags(Qt::CustomizeWindowHint);
#else
		parent->setWindowFlags(Qt::CustomizeWindowHint);
		parent->setWindowFlags(Qt::FramelessWindowHint);
#endif
		parent->setWindowTitle("New Title");
		parent->window()->setWindowOpacity(0.95);

		//ui.setupUi(this);
		//this->setIconSize(QSize(100, 100));
		//this->window()->setWindowIcon(QIcon("Qnt_Logo_128px.png"));


		setGeometry(0, 0, parent->width(), parent->height());



		//setIconSize(QSize(100, 100));

		mCursorGlobal = new QPoint(0, 0);
		mCursorLocal = new QPoint(0, 0);
		mFrameOriginGlobal = new QPoint(0, 0);

		//this->setGeometry(parent->geometry());
		//this->setMouseTracking(true);

		//menu = new QMenuBar(this);
		header = new QWidget(this);
		screen = new QWidget(this);
		tail = new QWidget(this);

		headerSize = 100;
		tailSize = 50;

		panelConsole = new Panel(this);
		panelConsole->setGeometry(QRect(300, 200, 200, 400));
		panelConsole->show();

		//header->setGeometry(0, 0, this->width(), 100);
		//screen->setGeometry(0, 100, this->width(), 250);
		//tail->setGeometry(0, this->height()-50, this->width(), 50);

		//setMenuBar(menu);
		//setCentralWidget(screen);
		//setStatusBar(tail);

		//menu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		//tail->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		//header->setGeometry(0, 0, width()/2, 150);
		//menu->setCornerWidget(header);


		//menu->setFixedHeight(70);
		//menu->addMenu(tr("&File"));
		//menu->addMenu(tr("&Edit"));

		//infoLabel->setText(tr("Invoked <b>File|New</b>"));

		buttTest = new QPushButton(screen);
		buttTest->setGeometry(350, 150, 100, 50);
		buttTest->setText("TEST");

		/*
		buttTest2 = new QPushButton(menu);
		buttTest2->setGeometry(350, 10, 100, 30);
		buttTest2->setText("CLOSE");
		*/

		testButton = new Button(screen);
		testButton->setGeometry(50, 50, 100, 30);
		testButton->setName(tr("testButton"));

		closeButton = new Button(header);
		minimizeButton = new Button(header);
		maximizeButton = new Button(header);

		//closeButton->show();


		//console = new QDockWidget(QString("Console"), this);

		dock = new QDockWidget(tr("Console"), screen);
		dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

		console = new Console(dock);
		dock->setWidget(console);
		//addDockWidget(Qt::RightDockWidgetArea, dock);




		connect(this, SIGNAL(sendToConsole(QString)), console, SLOT(addLine(QString)));

		connect(closeButton, SIGNAL(pressAct()), this, SLOT(closeCanvan()));
		connect(testButton, SIGNAL(pressAct()), this, SLOT(closeCanvan()));

		connect(testButton, SIGNAL(enterAct(QString)), console, SLOT(addLine(QString)));
		connect(testButton, SIGNAL(leaveAct(QString)), console, SLOT(addLine(QString)));


		msgConsole(tr("start"));
	}


	void Canvan::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		QRectF rect = screen->geometry();

		QPen *pen;
		pen = new QPen(Qt::red, 1);

		/*
		painter.setPen(Qt::NoPen);
		painter.setBrush(QBrush(QColor(20, 20, 20), Qt::SolidPattern));
		painter.drawRect(screen->geometry());

		*/


		painter.setPen(Qt::NoPen);
		painter.setBrush(QBrush(QColor(30, 30, 30), Qt::SolidPattern));
		painter.drawRect(header->geometry());
		painter.drawRect(tail->geometry());
		//painter.drawRect(panelConsole->geometry());

		painter.setPen(QPen(Qt::red, 1));
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(QRect(0, 0, width() - 1, height() - 1));



		painter.setPen(QPen(Qt::red, 1));
		painter.setBrush(QBrush(QColor(130, 130, 30), Qt::SolidPattern));
		painter.drawText(100, 15, title->text());



		//msgConsole("Canvan draw...");

		//Quant::draw(event);
	}

	void Canvan::setHeaderHeight(int height)
	{
		headerSize = height;
	}

	void Canvan::setTailHeight(int height)
	{
		tailSize = height;
	}



	void Canvan::resizeEvent(QResizeEvent *resizeEvent)
	{
		this->setGeometry(0, 0, parent->width(), parent->height());

		closeButton->setGeometry(width() - 40, 10, 30, 30);
		maximizeButton->setGeometry(width() - 70, 10, 30, 30);
		minimizeButton->setGeometry(width() - 100, 10, 30, 30);

		header->setGeometry(0, 0, this->width(), headerSize);
		screen->setGeometry(0, headerSize, this->width(), this->height() - tailSize - headerSize);
		tail->setGeometry(0, this->height() - tailSize, this->width(), tailSize);

		panelConsole->setGeometry(this->width() - 300, headerSize, 300, this->height() - tailSize);

		msgConsole(tr("resize [%1, %2]").arg(QString::number(width()), QString::number(height())));
	}

	void Canvan::mousePressEvent(QMouseEvent *mouseEvent)
	{
		*mCursorGlobal = mouseEvent->globalPos();
		*mCursorLocal = mouseEvent->pos();
		*mFrameOriginGlobal = QPoint(
			mCursorGlobal->x() - mCursorLocal->x(),
			mCursorGlobal->y() - mCursorLocal->y()
			);

		//*mCursor = mouseEvent->pos();
		msgConsole(tr("pressedGlobal [%1,%2]").arg(QString::number(mCursorGlobal->x()), QString::number(mCursorGlobal->y())));
		msgConsole(tr("pressedLocal [%1,%2]").arg(QString::number(mCursorLocal->x()), QString::number(mCursorLocal->y())));
		msgConsole(tr("frameOriginGlobal [%1,%2]").arg(QString::number(mFrameOriginGlobal->x()), QString::number(mFrameOriginGlobal->y())));
	}

	void Canvan::mouseMoveEvent(QMouseEvent *mouseEvent)
	{
		QPoint mouseCurrentGlobal = mouseEvent->globalPos();
		int posX = mFrameOriginGlobal->x() - mCursorGlobal->x() + mouseCurrentGlobal.x();
		int posY = mFrameOriginGlobal->y() - mCursorGlobal->y() + mouseCurrentGlobal.y();

		/*
		setGeometry(QRect(
			posX,
			posY,
			width(),
			height()
			));
		*/
		msgConsole(tr("mCursor [%1,%2]").arg(QString::number(posX), QString::number(posY)));
		/*
		msgConsole(tr("move [%1,%2]").arg(QString::number(mouseCoor.x()), QString::number(mouseCoor.y())));
		msgConsole(tr("moveLocal [%1,%2]").arg(
			QString::number(mouseLocalCoor->x()),
			QString::number(mouseLocalCoor->y())
			));
		*/
		//parent->setGeometry(posX, posY, parent->width(), parent->height());
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
