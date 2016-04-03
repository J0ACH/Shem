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

		void initControl();
		
		void setColor_background(QColor);
		void setColor_foreground(QColor);
		void drawCanvan(QPaintEvent *event);
		void addScreen(QWidget *inScreen);

		//Console *console;

		QWidget *win;
		QWidget *header;
		QWidget *screen;
		QWidget *tail;

		void setHeaderHeight(int);
		void setTailHeight(int);
		void setTitle(QString titleName);
		void setLogo(QImage);
		//void setCanvanPalette();
		void setCanvanStyleSheet();
		void setVersion(int major, int minor, int patch);
		
	signals:
		void resizeAct();
		void sendToConsole(QString);

	public slots:
		void msgConsole(QString);
		void closeCanvan();
		void minimizeCanvan();
		void maximizeCanvan();
				
	protected:
		void resizeEvent(QResizeEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		//Ui::CanvanClass ui;

		//QMenuBar *menu;
		
		int headerSize;
		int tailSize;
		
		Button *closeButton;
		Button *maximizeButton;
		Button *minimizeButton;

		Console *panelConsole;

		Edges *edges;

		QImage logo;
		QLabel *version;
		//QLabel *title;
		
		QPalette *palette;
		QColor textColor;
		QColor activeColor;
		QColor backColor;
		QColor panelColor;

		QPoint *mCursorGlobal;
		QPoint *mCursorLocal;
		QPoint *mFrameOriginGlobal;
	};

}

#endif // CANVAN_H
