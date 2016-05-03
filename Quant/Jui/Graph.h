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

		QRect bounds();
		int ID;
		int pixelX, pixelY;
		double valueX, valueY;

	signals:
		void actDelete(int ID);
		void actMoved(int ID, int pixelX, int pixelY);

	protected:
		void paintEvent(QPaintEvent *);
		void mousePressEvent(QMouseEvent *);
		void mouseMoveEvent(QMouseEvent *);
		void mouseReleaseEvent(QMouseEvent *mouseEvent);

		virtual bool eventFilter(QObject * watched, QEvent * event);

	private:
		
		int pointSize;

		QPoint mousePressCoor;
		QPoint mouseGlobalCoor;
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
		
		/*
		void setLevels(QList<double> levels);
		QList<double> getLevels();
		void setTimes(QList<double> times);
		QList<double> getTimes();
		void setCurves(QList<double> curves);
		*/

		void addPixelPoint(int pixelX, int pixelY);
		void addValuePoint(double valueX, double valueY);

		void drawPoint(double valueX, double valueY);
		void drawLine(double valueX1, double valueY1, double valueX2, double valueY2);
		void drawPolyline(QVector<QPointF> collPoints);

		void deleteGraph();

	public slots:
		void onDeletePoint(int ID);
		void onMovePoint(int ID, int pixelX, int pixelY);
		void onGraphEnv(QList<double> levels, QList<double> times, QList<double> curves);

	signals:
		void actPointAdded(double valueX, double valueY);
		//void actControlPointsChange(QVector<QPointF>);
		void actGraphEnv(QList<double> levels, QList<double> times, QList<double> curves);

	protected:
		void paintEvent(QPaintEvent *);
		void resizeEvent(QResizeEvent *event);

	private:
		void mousePressEvent(QMouseEvent *mouseEvent);
		void mouseReleaseEvent(QMouseEvent *mouseEvent);

		void makeEnv();

		int frameOffset;
		int minDomainX, maxDomainX, minDomainY, maxDomainY;

		QMap<int, GraphPoint*> controlPts;

		QList<QPointF*> collDrawPoints;
		QList<QLineF*> collDrawLines;
		QPolygonF *graphPolylines;

		QList<double> graphValues;

		//int newPointID;
		double getValueX(int displayX);
		double getValueY(int displayY);
		double getPixelX(double valueX);
		double getPixelY(double valueY);

	};

}
#endif // GRAPH_H

