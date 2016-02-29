#include "Quant.h"

int main(int argc, char** argv){

	QApplication app(argc, argv);

	QWidget *win = new QWidget();
	win->setGeometry(100, 100, 300, 200);
	win->show();
	/*
	SC_LanguageClient * client = createLanguageClient("sclang");

	if (!client)
		return 1;

	int returnCode = client->run(argc, argv);

	destroyLanguageClient(client);
	*/

	return app.exec();
}


