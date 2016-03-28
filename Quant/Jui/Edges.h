#ifndef EDGES_H
#define EDGES_H


#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QList>
#include <QEvent>
#include <QMouseEvent>
//#include <list>
#include "EdgeControler.h"


namespace Jui
{

	class Edges : public QObject
	{
		Q_OBJECT

	public:
		Edges(QWidget *parent = 0);
		~Edges();

		QRect bounds();
		void setEdgeOffset(int);

	public slots:
		void edgeMoved(EdgePosition);
		
	private:
		QWidget *collParent;
		QVector<EdgeControler*> *collEdges;
		//QVector<QRect> *collRect;
		EdgeControler *testEdge;
		//Button *testButton;
		int edgeOffset;
		//bool drawRegion;
		//QRegion maskRegion;

		//QWidget *testWidget;

		void addManipulator(EdgePosition);

	protected:
		//bool eventFilter(QObject *obj, QEvent *event);
		//void paintEvent(QPaintEvent *event);
		//void resizeEvent(QResizeEvent *event);
		//void moveEvent(QMoveEvent * event);
	};


}
#endif // EDGES_H



