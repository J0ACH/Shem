#include "Quant.h"
#include <QApplication>

//using namespace Jui;
using namespace SupercolliderBridge;

int main(int argc, char** argv){

	QApplication app(argc, argv);
	
	QuantIDE::Quant *win = new QuantIDE::Quant();
	win->setGeometry(50, 50, 1100, 700);
	win->show();
	
	return app.exec();
}

namespace QuantIDE
{

	Quant::Quant(QWidget *parent) : QWidget(parent)
	{
		canvan = new Canvan(this);
		bridge = new ScBridge(this);

		canvan->msgConsole(QString("ScBridge init..."));

		this->initControl();

		canvan->setHeaderHeight(50);
		canvan->setTailHeight(30);
		canvan->setLogo(QImage(":/logo128.png"));

		canvan->msgConsole(QString("Control init..."));

		//this->setMouseTracking(true);

		//canvan->setTitle("Quant");
		//canvan.setVersion(Qnt_VERSION_MAJOR, Qnt_VERSION_MINOR, Qnt_VERSION_PATCH);

		connect(buttAddNode, SIGNAL(pressAct()), this, SLOT(addNode()));

	}

	void Quant::initControl()
	{
		buttAddNode = new Button(canvan->screen);
		buttAddNode->setGeometry(400, 5, 80, 30);
		buttAddNode->setText("AddNode");
	
		testPanel = new Panel(this);
		testPanel->setGeometry(100, 100, 300, 300);
		testPanel->setTitle("Test");
		testPanel->setBackground(Qt::black);

	}
	
	void Quant::paintEvent(QPaintEvent *paintEvent)
	{
		QPainter painter(this);
		painter.fillRect(QRect(0,0,width()-1, height()-1), QColor(20, 20, 20));
	}

	void Quant::resizeEvent(QResizeEvent *resizeEvent)
	{
		canvan->msgConsole(tr("TOPView resize [%1, %2]").arg(QString::number(width()), QString::number(height())));
	}

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

