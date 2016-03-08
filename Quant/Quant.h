#ifndef QUANT_H
#define QUANT_H


#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include "ScBridge.h"


class Quant : public QWidget
{
	Q_OBJECT

public:
	Quant();
	~Quant();

public slots:
	void beep();
	void pdefPlay();
	void consoleAddMsg(QString);

protected:
	void closeEvent(QCloseEvent *event);

private:
	QPushButton *butt1;
	QPushButton *butt2;

	QTextEdit *console;

	ScBridge *bridge;
};

#endif // QUANT

