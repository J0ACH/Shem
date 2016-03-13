#ifndef CANVAN_H
#define CANVAN_H

#include <QtWidgets/QMainWindow>
#include <QDockWidget>
#include <QPushButton>
#include <QTimer>
#include <QMenuBar>
#include <QStatusBar>
#include <QLabel>
#include <QStyleFactory>

//#include "ui_canvan.h"

#include "Console.h"
#include "Button.h"

namespace Jui
{

	class Canvan : public QWidget
	{
		Q_OBJECT

	public:

		explicit Canvan(QWidget *parent = 0);
		~Canvan();

		void setTitle(QString titleName);
		void setVersion(int major, int minor, int patch);

		void setColor_background(QColor);
		void setColor_foreground(QColor);

		Console *console;

		QWidget *header;
		QWidget *screen;
		QWidget *tail;
		
		void setHeaderHeight(int);
		void setTailHeight(int);

	signals:
		void sendToConsole(QString);

	public slots:
		void msgConsole(QString);
		void closeCanvan();
		void minimizeCanvan();
		void maximizeCanvan();

	protected:
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void resizeEvent(QResizeEvent *event);
		virtual	void paintEvent(QPaintEvent *event);

	private:
		//Ui::CanvanClass ui;

		//QMenuBar *menu;
		int headerSize;
		int tailSize;

		void refresh();

		Button *testButton;
		Button *closeButton;
		Button *maximizeButton;
		Button *minimizeButton;

		QPushButton *buttTest;
		QPushButton *buttTest2;


		QDockWidget *dock;


		QLabel *title;
		QLabel *version;


		void mySetPalette();


		QPalette *palette;
		QColor *textColor;
		QColor *activeColor;
		QColor *backColor;
		QColor *panelColor;

		QPoint *mCursor;
	};

}

#endif // CANVAN_H
