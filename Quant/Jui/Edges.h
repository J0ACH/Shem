#ifndef EDGES_H
#define EDGES_H


#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QList>
#include <QEvent>
#include <QMouseEvent>

#include "EdgeControler.h"


namespace Jui
{

	class Edges : public QObject
	{
		Q_OBJECT

	public:
		enum ParentType
		{
			WIN,
			WIDGET
		};

		Edges(QWidget *parent = 0);
		~Edges();

		QRect bounds();
		void setEdgeOffset(int);

	public slots:
		void edgeMoved(EdgeControler::Direction, int);


	private:
		QWidget *parent;
		ParentType parentType;
		
		int edgeOffset;

		void addManipulator(EdgeControler::Direction);

	protected:
		//bool eventFilter(QObject *obj, QEvent *event);
		//void paintEvent(QPaintEvent *event);
		//void resizeEvent(QResizeEvent *event);
		//void moveEvent(QMoveEvent * event);
	};


}
#endif // EDGES_H



