#ifndef CANVAN_H
#define CANVAN_H

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QFontDatabase>
#include <QMap>
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
		void setEdgeControler(EdgeControler::Direction, bool);

		void connectWindow(QWidget*);

		void addScreen(QWidget *inScreen);

		QWidget *win;
		QWidget *header, *screen, *tail;

		Console *mConsole;

		void setHeaderHeight(int);
		void setTailHeight(int);
		void setTitle(QString titleName);
		void setLogo(QImage);
		void setCanvanStyleSheet();
		void setVersion(int major, int minor, int patch);

	signals:
		void resizeAct();
		void resizeScreenAct();
		void closeAct();

		void consolePrintAct(QString, QColor, bool);

	public slots:
		void print(QString, QColor);
		void println(QString, QColor);

		void closeCanvan();
		void minimizeCanvan();
		void maximizeCanvan();

		void fitScreen();
		void onConfigData(QMap<QString, QVariant*> config);

	protected:
		void resizeEvent(QResizeEvent *resizeEvent);
		void mousePressEvent(QMouseEvent *mouseEvent);
		void mouseReleaseEvent(QMouseEvent *mouseEvent);
		void mouseMoveEvent(QMouseEvent *mouseEvent);
		void paintEvent(QPaintEvent *paintEvent);

	private:
		bool isPressed;
		bool showScreen;

		int headerSize, tailSize;

		Button *closeButton, *maximizeButton, *minimizeButton;
		Edges *edges;
		QMap<QString, QVariant*> config;

		QImage logo;
		QLabel *version;
		//QLabel *title;

		QColor textColor, activeColor, backColor, panelColor;

		QPoint *mCursorGlobal;
		QPoint *mCursorLocal;
		QPoint *mFrameOriginGlobal;


	};

}

#endif // CANVAN_H
