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
		void addManipulator(EdgeControler::Direction);
		void removeManipulator(EdgeControler::Direction);

  void hide();
  void show();
		
	public slots:
		void edgePressed();
		void edgeMoved(EdgeControler::Direction, int);
		
	private:
		QWidget *parent;
		ParentType parentType;

		QPoint mEdgePressActGrandParentOriginGlobal;
		int edgeOffset;

  QList<EdgeControler*> conteiner;
	};
}
#endif // EDGES_H



