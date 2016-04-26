#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QPainter>
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
		//QRegion region;

	signals:
		void actDelete(int ID);

	protected:
		void paintEvent(QPaintEvent *);
		void mousePressEvent(QMouseEvent *);
		void mouseMoveEvent(QMouseEvent *);
		virtual bool eventFilter(QObject * watched, QEvent * event);
		
	private:
		int ID;
		int pixelX, pixelY;
		double valueX, valueY;
		int pointSize;

		QPoint mousePressCoor;
	};
	

	// GRAPH POINT END
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// GRAPH 
	

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
		QList<double> getDomainX();
		QList<double> getDomainY();

		void addPixelPoint(int pixelX, int pixelY);
		void addValuePoint(double valueX, double valueY);
		void addLine(double valueX1, double valueY1, double valueX2, double valueY2);
		void deleteGraph();
		
	public slots:
		void onDeletePoint(int ID);

	protected:
		void paintEvent(QPaintEvent *);
								
	private:
		void mousePressEvent(QMouseEvent *mouseEvent);
		void mouseReleaseEvent(QMouseEvent *mouseEvent);

		int frameOffset;
		int minDomainX, maxDomainX, minDomainY, maxDomainY;
		QPoint cursorPos;
		QMap<int, GraphPoint*> collectionPts;
		QList<QLine*> collectionLines;

		int newPointID;
		double getValueX(int displayX);
		double getValueY(int displayY);
		double getDisplayX(double valueX);
		double getDisplayY(double valueY);
		
	};

}
#endif // GRAPH_H

