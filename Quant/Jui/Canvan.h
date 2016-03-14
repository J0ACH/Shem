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
#include "Panel.h"

namespace Jui
{

	class Canvan : public QWidget
	{
		Q_OBJECT

	public:

		Canvan(QWidget *parent = 0);
		~Canvan();

		void setTitle(QString titleName);
		void setVersion(int major, int minor, int patch);

		void setColor_background(QColor);
		void setColor_foreground(QColor);
		void drawCanvan();
		void addScreen(QWidget *inScreen);

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
		void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

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

		Panel *panelConsole;

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
