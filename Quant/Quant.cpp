#include "Quant.h"
#include <QApplication>

//using namespace Jui;
using namespace SupercolliderBridge;

int main(int argc, char** argv){

	QApplication app(argc, argv);

	
	QuantIDE::Quant *win = new QuantIDE::Quant();
	win->setGeometry(50, 100, 1100, 700);
	win->show();

	//QWidget win;
	//Canvan *win = new Canvan();

	//win.setGeometry(100, 100, 800, 600);
	//win.show();

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

		rectTest = Rect(this).offset(100,100,0,0);

		//this->setMouseTracking(true);

		canvan->msgConsole(tr("rectTest left:%1, top:%2, right:%3, bottom:%4").arg(
			QString::number(rectTest.left()),
			QString::number(rectTest.top()),
			QString::number(rectTest.right()),
			QString::number(rectTest.bottom())
			));

		
		
		//rectTest = Rect.offset(canvan->screen, 0, 0, 0, 0);

		resizeTest = new QWidget(canvan->screen);
		resizeTest->setGeometry(500, 50, 300, 300);

		resizeTest2 = new QWidget(resizeTest);
		resizeTest2->setGeometry(20, 20, 260, 260);
		
		testButton = new Button(resizeTest);
		testButton->setGeometry(20, 20, 260, 260);
		
		resizeManipulator = new Edges(resizeTest);
		//resizeManipulator->setGeometry(0, 0, 20, 100);
		//, EdgePosition::RIGHT

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
	
	void Quant::resizeEvent(QResizeEvent *resizeEvent)
	{
		canvan->resizeEvent(resizeEvent);


		canvan->msgConsole(tr("TOPView resize [%1, %2]").arg(QString::number(width()), QString::number(height())));
	}

	void Quant::paintEvent(QPaintEvent *event)
	{

		QPainter painter(this);
		//canvan->paintEvent(event);
		/*
		canvan->msgConsole(tr("rectTest2 left:%1, top:%2, right:%3, bottom:%4").arg(
			QString::number(rectTest.left()),
			QString::number(rectTest.top()),
			QString::number(rectTest.right()),
			QString::number(rectTest.bottom())
			));
		*/

		painter.setPen(QPen(Qt::yellow, 3));
		//painter.drawRect(rectTest);
		//painter.drawRect(Rect(this).offset(100,100,100,100));
		painter.drawRect(QRect(0, 0, 500, 500));


		painter.setPen(QPen(Qt::red, 1));
		painter.drawLine(canvan->screen->geometry().bottomLeft(), canvan->screen->geometry().topRight());
		painter.drawLine(canvan->screen->geometry().bottomRight(), canvan->screen->geometry().topLeft());

		painter.setPen(QPen(Qt::red, 3));
		painter.drawRect(canvan->screen->geometry().adjusted(5,5,-5,-5));

		painter.setPen(Qt::NoPen);
		painter.setBrush(QBrush(QColor(120, 20, 20, 150), Qt::SolidPattern));
		painter.drawRect(resizeTest->geometry());

		int width = resizeTest2->size().width() - 1;
		int height = resizeTest2->size().height() - 1;

		painter.fillRect(resizeTest->pos().x()+20, resizeTest->pos().y()+20, width, height, QColor(20, 20, 220));

		//painter.setBrush(QBrush(QColor(20, 120, 20), Qt::SolidPattern));
		//painter.drawRect(resizeTest2->geometry());

		painter.setPen(QPen(Qt::blue, 3));
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(resizeTest2->rect());


		/*
		painter.setPen(Qt::NoPen);
		painter.setBrush(QBrush(QColor(20, 20, 20), Qt::SolidPattern));
		painter.drawRect(QRect(0, 0, canvan->screen->width(), canvan->screen->height()));
		painter.drawRect(canvan->screen->geometry());


		//QRectF target(0, 0, width(), height());
		QRectF target(150, 250, 168 / 2, 128 / 2);
		QRectF source(0, 0, 168, 128);

		QImage image(":/logo128.png");

		painter.drawImage(target, image, source);

		//painter.drawText(10, 15, QString::number(backgroundAlpha));


		//

		//painter.setBrush(Qt::NoBrush);
		//QRect aaa(0, 0, resizeTest->width(), 100);
		//aaa.setTopLeft(resizeTest->pos());
		//painter.drawRect(aaa);




		//painter.drawRect(resizeManipulator->geometry());

		*/
		
		//canvan->msgConsole(QString("TOPView draw..."));
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

	Quant::~Quant()
	{

	}

	void Quant::closeEvent(QCloseEvent *event)
	{
		bridge->evaluateCode("Server.local.quit;"); // not working?
		bridge->killLang();

	}
}

