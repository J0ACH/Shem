#ifndef EDGECONTROLER_H
#define EDGECONTROLER_H


#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QList>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

//#include "Edges.h"

namespace Jui
{


	class EdgeControler : public QWidget
	{
		Q_OBJECT

	public:
		enum Direction
		{
			LEFT,
			TOP,
			RIGHT,
			BOTTOM,
			ALL
		};

		EdgeControler(QWidget *parent = 0);
		~EdgeControler();

		QRect bounds();

		void setDirection(Direction);
		void setEdgeOffset(int);
		void setParentObject(QObject*);

	signals:
		void pressAct();
		void moveAct(EdgeControler::Direction, int);

	public slots:
		void alphaUpdate();
		void fitGeometry();

	protected:
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);

	private:
		bool isOver;
		Direction side;
		QObject *conteiner;

		int fadeTimeIn;
		int fadeTimeOut;
		int fps;
		int backgroundAlpha;

		QTimer *timer;

		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

		int edgeOffset;

		QPoint *mCursorGlobal;
		QPoint *mCursorLocal;
		QPoint *mFrameOriginGlobal;

	};
}

#endif // EDGECONTROLER_H



