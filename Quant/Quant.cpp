#include "Quant.h"
#include <QApplication>

//using namespace Jui;
using namespace SupercolliderBridge;

int main(int argc, char** argv){

	QApplication app(argc, argv);

	
	QuantIDE::Quant *win = new QuantIDE::Quant();
	win->setGeometry(50, 50, 1100, 700);
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
		
		resizeManipulator = new Jui::Edges(resizeTest);
		resizeManipulator->installEventFilter(this);
		//resizeManipulator->setGeometry(0, 0, 20, 100);
		//, EdgePosition::RIGHT

		//canvan.setVersion(Qnt_VERSION_MAJOR, Qnt_VERSION_MINOR, Qnt_VERSION_PATCH);

		connect(buttAddNode, SIGNAL(pressAct()), this, SLOT(addNode()));

	}

	void Quant::initControl()
	{
		canvan->msgConsole(QString("Control init..."));

		buttAddNode = new Button(canvan->screen);
		buttAddNode->setGeometry(400, 5, 80, 30);
		buttAddNode->setText("AddNode");
		buttAddNode->installEventFilter(this);


	}
	
	void Quant::resizeEvent(QResizeEvent *resizeEvent)
	{
		canvan->resizeEvent(resizeEvent);


		canvan->msgConsole(tr("TOPView resize [%1, %2]").arg(QString::number(width()), QString::number(height())));
	}
	/*
	void Quant::paintEvent(QPaintEvent *event)
	{

		QPainter painter(this);
		
		painter.setPen(QPen(Qt::yellow, 3));
		painter.drawRect(QRect(0, 0, 500, 500));

		painter.setPen(QPen(Qt::red, 1));
		painter.drawLine(canvan->screen->geometry().bottomLeft(), canvan->screen->geometry().topRight());
		painter.drawLine(canvan->screen->geometry().bottomRight(), canvan->screen->geometry().topLeft());

		painter.setPen(QPen(Qt::red, 3));
		painter.drawRect(canvan->screen->geometry().adjusted(5, 5, -5, -5));

		painter.setPen(Qt::NoPen);
		//painter.setBrush(QBrush(QColor(120, 20, 20, 150), Qt::SolidPattern));
		//painter.drawRect(resizeTest->geometry());
		painter.fillRect(resizeTest->geometry(), QColor(120, 20, 20, 150));

		qDebug("Quant::paintEvent");
	}
	*/

	void Quant::addNode()
	{
		canvan->msgConsole(QString("Node add..."));
	}

	void Quant::consoleAddMsg(QString msg)
	{
		canvan->console->append(msg);
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

