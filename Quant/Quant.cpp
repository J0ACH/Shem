#include "Quant.h"
#include <QApplication>

//using namespace Jui;
using namespace SupercolliderBridge;



int main(int argc, char** argv){

	QApplication app(argc, argv);
	
	/*
	QWidget *win2 = new QWidget();
	Canvan *canvan = new Canvan(win2);
	canvan->show();
	*/
	
	QuantIDE::Quant *win = new QuantIDE::Quant();
	win->setGeometry(50, 100, 1100, 700);
	win->show();
	//win->setStyleSheet("QMenuBar {	background - color: black;	}");
	//canvan->show();

	//QMainWindow *win = new QMainWindow();
	//win->setWindowIcon(QIcon(":/Resources/Qnt_AppIcon_16px.ico"));

	return app.exec();
}



namespace QuantIDE
{
	
	Quant::Quant(QWidget *parent) 
	{
		canvan = new Canvan(this);
		bridge = new ScBridge(this);

		canvan->msgConsole(QString("ScBridge init..."));
		
		canvan->setTitle("Quant");

		

		this->initControl();
		
		canvan->setHeaderHeight(50);
		canvan->setTailHeight(30);

		resizeTest = new QWidget(canvan->screen);
		resizeTest->setGeometry(500, 100, 100, 100);
		
		//canvan.setVersion(Qnt_VERSION_MAJOR, Qnt_VERSION_MINOR, Qnt_VERSION_PATCH);
		
		connect(buttAddNode, SIGNAL(pressAct()), this, SLOT(addNode()));
	
		}
	
	void Quant::initControl()
	{
		canvan->msgConsole(QString("Control init..."));

		buttAddNode = new Button(canvan->screen);
		buttAddNode->setGeometry(500, 200, 80, 30);
		buttAddNode->setName("AddNode");
	}

	void Quant::addNode()
	{
		canvan->msgConsole(QString("Node add..."));
	}
	

	void Quant::consoleAddMsg(QString msg)
	{
		canvan->console->append(msg);
	}
	void Quant::beep()
	{
		bridge->evaluateCode("().play");
	}

	void Quant::pdefPlay()
	{
		bridge->evaluateCode("Pdef('test', Pbind('instrument', 'default', 'dur', Pseq([1,1,0.5,0.5], 2), 'freq', 90)).play;");
	}


	void Quant::resizeEvent(QResizeEvent *resizeEvent)
	{
		canvan->resizeEvent(resizeEvent);


		canvan->msgConsole(tr("TOPView resize [%1, %2]").arg(QString::number(width()), QString::number(height())));
	}



	void Quant::paintEvent(QPaintEvent *event)
	{
		
		QPainter painter(this);
		//canvan->paintEvent(event);

		
		painter.setPen(Qt::NoPen);
		painter.setBrush(QBrush(QColor(20, 20, 20), Qt::SolidPattern));
		//painter.drawRect(QRect(0, 0, canvan->screen->width(), canvan->screen->height()));
		painter.drawRect(canvan->screen->geometry());

		painter.drawLine(150, 250, 300, 250);

		//QRectF target(0, 0, width(), height());
		QRectF target(150, 250, 168 / 2, 128 / 2);
		QRectF source(0, 0, 168, 128);

		QImage image(":/logo128.png");

		painter.drawImage(target, image, source);

		//painter.drawText(10, 15, QString::number(backgroundAlpha));


		painter.setPen(QPen(Qt::red, 3));
		painter.setBrush(QBrush(QColor(120, 120, 20), Qt::SolidPattern));
		painter.drawRect(resizeTest->geometry());

		//canvan->msgConsole(QString("TOPView draw..."));
	}
	
	
	

	Quant::~Quant()
	{

	}

	void Quant::closeEvent(QCloseEvent *event)
	{
		bridge->evaluateCode("Server.local.quit;"); // not working?
		bridge->killLang();
		
	}
}

