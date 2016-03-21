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
		EdgeControler *testEdge;
		Button *testButton;
		int edgeOffset;
		
	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

	};


}
#endif // EDGES_H



