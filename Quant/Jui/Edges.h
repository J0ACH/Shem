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
		
	class Edges : public QWidget
	{
		Q_OBJECT

	public:
		Edges(QWidget *parent = 0);
		~Edges();

		QRect bounds();
		void setEdgeOffset(int);

	private:
		QList<EdgeControler*> collEdges;
		QVector<QRect> *collRect;
		EdgeControler *testEdge;
		Button *testButton;
		int edgeOffset;
		bool drawRegion;
		QRegion maskRegion;

		void addManipulator(EdgePosition);
		
	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		void moveEvent(QMoveEvent * event);
	};


}
#endif // EDGES_H



