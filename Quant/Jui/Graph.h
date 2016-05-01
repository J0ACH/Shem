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
		void mouseReleaseEvent(QMouseEvent *mouseEvent);

		virtual bool eventFilter(QObject * watched, QEvent * event);

	private:
		int ID;
		int pixelX, pixelY;
		double valueX, valueY;
		int pointSize;

		//QPoint mousePressCoor;
		bool isPressed;
		QPoint *mCursorLocal;
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

		void drawPoint(double valueX, double valueY);
		void drawLine(double valueX1, double valueY1, double valueX2, double valueY2);
		void drawPolyline(QVector<QPointF> collPoints);

		void deleteGraph();

		public slots:
		void onDeletePoint(int ID);

	signals:
		void actPointAdded(double valueX, double valueY);

	protected:
		void paintEvent(QPaintEvent *);
		void resizeEvent(QResizeEvent *event);

	private:
		void mousePressEvent(QMouseEvent *mouseEvent);
		void mouseReleaseEvent(QMouseEvent *mouseEvent);

		int frameOffset;
		int minDomainX, maxDomainX, minDomainY, maxDomainY;
		//QPoint cursorPos;
		QMap<int, GraphPoint*> collectionPts;

		QList<QPointF*> collDrawPoints;
		QList<QLineF*> collDrawLines;
		QPolygonF *graphPolylines;

		QList<double> graphValues;

		int newPointID;
		double getValueX(int displayX);
		double getValueY(int displayY);
		double getPixelX(double valueX);
		double getPixelY(double valueY);

	};

}
#endif // GRAPH_H

