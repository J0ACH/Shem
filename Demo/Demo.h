#ifndef DEMO_H
#define DEMO_H


#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
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
		void pdefPlay();
		void consoleAddMsg(QString);

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
		QPushButton *butt2;

		QTextEdit *console;

		
	};

}

#endif // DEMO

