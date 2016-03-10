#ifndef DEMO_H
#define DEMO_H


#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>

#include "ScBridge.h"

using namespace SupercolliderBridge;

namespace DemoIDE {


	class Demo : public QWidget
	{
		Q_OBJECT

	public:
		Demo();
		~Demo();

	public slots:
		void beep();
		void sendCode();
		void consoleAddMsg(QString);
		
	signals:
		void sendCode(QString);
		
	protected:
		void closeEvent(QCloseEvent *event);

	private:
		ScBridge *bridge;

		void initControlers();

		QPushButton *buttStartLang;
		QPushButton *buttStartServer;
		QPushButton *buttKillLang;
		QPushButton *buttKillServer;
		QPushButton *buttBeep;
		QPushButton *buttCode;
		
		QLabel *runningLang;
		QLabel *runningServer;
		
		QTextEdit *console;
		QTextEdit *editor;

		
	};

}

#endif // DEMO

