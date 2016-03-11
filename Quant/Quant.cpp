#include "Quant.h"
#include <QApplication>

using namespace Jui;
using namespace SupercolliderBridge;



int main(int argc, char** argv){

	QApplication app(argc, argv);
	
	QuantIDE::Quant *win = new QuantIDE::Quant();
	//QMainWindow *win = new QMainWindow();
	//win->setWindowIcon(QIcon(":/Resources/Qnt_AppIcon_16px.ico"));
	win->setGeometry(100, 100, 1100, 400);
	win->show();

	return app.exec();
}



namespace QuantIDE
{
	
	Quant::Quant(QWidget *parent)
	{

		bridge = new ScBridge(this);
		this->msgConsole(QString("ScBridge init..."));

		this->initControl();
		

		
		
		this->setTitle("Quant2");
		//win.setVersion(Qnt_VERSION_MAJOR, Qnt_VERSION_MINOR, Qnt_VERSION_PATCH);
		
		//app.setPalette(darkPalette);

		

		connect(buttAddNode, SIGNAL(pressAct()), this, SLOT(msgConsole(QString("AddNode"))));
			
	}
	
	void Quant::initControl()
	{
		this->msgConsole(QString("Control init..."));

		buttAddNode = new Button(this);
		buttAddNode->setGeometry(200, 100, 80, 30);
	}
	

	void Quant::consoleAddMsg(QString msg)
	{
		console->append(msg);
	}
	void Quant::beep()
	{
		bridge->evaluateCode("().play");
	}

	void Quant::pdefPlay()
	{
		bridge->evaluateCode("Pdef('test', Pbind('instrument', 'default', 'dur', Pseq([1,1,0.5,0.5], 2), 'freq', 90)).play;");
	}

	void Quant::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);
		
		painter.setPen(QPen(Qt::red, 3));
		painter.setBrush(QBrush(QColor(120, 20, 20), Qt::SolidPattern));
		painter.drawRect(QRect(500,200,50,50));

		painter.drawLine(150, 250, 300, 250);

		//QRectF target(0, 0, width(), height());
		QRectF target(150, 250, 168/2, 128/2);
		QRectF source(0, 0, 168, 128);
		
		QImage image(":/logo128.png");

		painter.drawImage(target, image, source);

		//painter.drawText(10, 15, QString::number(backgroundAlpha));
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

