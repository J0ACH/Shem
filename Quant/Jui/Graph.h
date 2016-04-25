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
		GraphPoint(QWidget *parent, int ptID, int pX, int pY, double valX, double valY); // !!! cesta jak udelat konstruktor neodvozeny od materske tridy
		~GraphPoint();

		bool isOver;
		QRect bounds();
		QRegion region;

	signals:
		void actDelete(int ID);

	protected:
		void paintEvent(QPaintEvent *);
		void mouseMoveEvent(QMouseEvent *);
		virtual bool eventFilter(QObject * watched, QEvent * event);

	private:
		int ID;
		int pixelX, pixelY;
		double valueX, valueY;
		int pointSize;
	};
	/*
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
	*/

	class Graph : public QWidget
	{
		Q_OBJECT

	public:
		Graph(QWidget *parent);
		~Graph();

		QRect bounds();
		QRect boundsGraph();
		void setDomainX(int min, int max);
		void setDomainY(int min, int max);

		void addPixelPoint(int pixelX, int pixelY);
		void addValuePoint(double valueX, double valueY);
		//GraphCurve addCurve(GraphPoint *from, GraphPoint *to);
		
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
		int newPointID;
		double getValueX(int displayX);
		double getValueY(int displayY);
		double getDisplayX(double valueX);
		double getDisplayY(double valueY);
		
	};

}
#endif // GRAPH_H

