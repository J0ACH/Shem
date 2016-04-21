#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QPainter>
#include <QFrame>
#include <QTimer>
#include <QDebug>

#include <QMouseEvent>

namespace Jui
{
	class GraphPoint : public QWidget
	{
		Q_OBJECT

	public:
		GraphPoint(QWidget *parent, int ptID); // !!! cesta jak udelat konstruktor neodvozeny od materske tridy
		~GraphPoint();

		bool isOver;
		QRect bounds();

	signals:
		void actDelete(int ID);

	protected:
		void paintEvent(QPaintEvent *);
		virtual bool eventFilter(QObject * watched, QEvent * event);

	private:
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

		int ID;
		//int posX, posY;
		//float valueX, valueY;
	};

	class GraphCurve : public QWidget
	{
		Q_OBJECT

	public:
		GraphCurve(QWidget *parent, GraphPoint *from, GraphPoint *to);
		~GraphCurve();

	private:
		GraphPoint *startPoint, *endPoint;
		bool isOver;
	};


	class Graph : public QWidget
	{
		Q_OBJECT

	public:
		Graph(QWidget *parent);
		~Graph();

		QRect bounds();
		QRect boundsGraph();
		void setDomainX(int min, int max);
		
	public slots:
		void onDeletePoint(int ID);

	protected:
		void paintEvent(QPaintEvent *);
		void mouseMoveEvent(QMouseEvent *);

	private:
		void mousePressEvent(QMouseEvent *mouseEvent);
		void mouseReleaseEvent(QMouseEvent *mouseEvent);

		int frameOffset;
		int minDomainX, maxDomainX, minDomainY, maxDomainY;
		QPoint cursorPos;
		QMap<int, GraphPoint*> collectionPts;

		double getValueX(int displayX);
		double getValueY(int displayY);
		
	};

}
#endif // GRAPH_H

