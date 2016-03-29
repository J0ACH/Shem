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
	enum EdgePosition
	{
		LEFT,
		TOP,
		RIGHT,
		BOTTOM,
		ALL
	};

	class EdgeControler : public QWidget
	{
		Q_OBJECT

	public:
		EdgeControler(QWidget *parent = 0);
		~EdgeControler();

		QRect bounds();

		void setDirection(EdgePosition);
		void setEdgeOffset(int);
		void setParentObject(QObject*);
		void fitGeometry();

	signals:
		void moveAct(EdgePosition, int);

	public slots:
		void alphaUpdate();

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);

	private:
		bool isOver;
		EdgePosition side;
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



