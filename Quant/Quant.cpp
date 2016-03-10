#include "Quant.h"
#include <QApplication>

using namespace Jui;
using namespace SupercolliderBridge;



int main(int argc, char** argv){

	QApplication app(argc, argv);
	
	QuantIDE::Quant *canvan = new QuantIDE::Quant();
			
	//Canvan *canvan = new Canvan(); //(100, 100, 600, 400);
	
	canvan->show();

	

	//QuantIDE::Quant qnt(canvan);
	
	return app.exec();
}



namespace QuantIDE
{
	
	Quant::Quant(QWidget *parent)
	{

		bridge = new ScBridge(this);
		

		
		

		
		//Canvan win(100, 100, 700, 400);
		this->setTitle("Quant");
		//win.setVersion(Qnt_VERSION_MAJOR, Qnt_VERSION_MINOR, Qnt_VERSION_PATCH);
		//win.show();


		//app.setPalette(darkPalette);
		this->msgConsole(QString("pre-bridge"));

		//ScBridge bridge;
		this->msgConsole(QString("bridge"));




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

	Quant::~Quant()
	{

	}

	void Quant::closeEvent(QCloseEvent *event)
	{
		bridge->evaluateCode("Server.local.quit;"); // not working?
		bridge->killLang();
		
	}
}

