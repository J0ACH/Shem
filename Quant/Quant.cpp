#include "Quant.h"

int main(int argc, char** argv){

	QApplication app(argc, argv);

	QWidget *win = new QWidget();
	win->setGeometry(100, 100, 300, 200);
	win->show();

	ScBridge * bridge = new ScBridge();


	return app.exec();
}


