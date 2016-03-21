#ifndef EDGES_H
#define EDGES_H


#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QList>
#include <QEvent>
//#include <list>

#include "Button.h"

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


	// EdgeControler musi byt nacten pred Edge
	class EdgeControler : public QWidget
	{
		Q_OBJECT

	public:
		EdgeControler(QWidget *parent = 0);
		~EdgeControler();

		QRect bounds();

		void setDirection(EdgePosition);
		void setEdgeOffset(int);

	public slots:
		void alphaUpdate();

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

	private:
		bool isOver;
		EdgePosition side;

		int fadeTimeIn;
		int fadeTimeOut;
		int fps;
		int backgroundAlpha;

		QTimer *timer;

		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

		int edgeOffset;

	};



	class Edges : public QWidget
	{
		Q_OBJECT

	public:
		Edges(QWidget *parent = 0);
		~Edges();

		QRect bounds();
		void setEdgeOffset(int);

	private:
		EdgeControler *testEdge;
		Button *testButton;
		int edgeOffset;
		
	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

	};


}
#endif // EDGES_H



