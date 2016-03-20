#ifndef EDGES_H
#define EDGES_H


#include <QWidget>
#include <QPainter>
#include <QList>
#include <QEvent>
//#include <list>

namespace Jui
{

	enum EdgePosition
	{
		LEFT,
		TOP,
		RIGHT,
		BOTTOM,
		ALL
	};


	// EdgeControler musi byt nacten pred Edge
	class EdgeControler : public QWidget
	{
		Q_OBJECT

	public:
		EdgeControler(QWidget *parent = 0);
		~EdgeControler();

		QRect bounds();

		void setDirection(EdgePosition);

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		
	private:
		EdgePosition side;
	};



	class Edges : public QWidget
	{
		Q_OBJECT

	public:
		Edges(QWidget *parent = 0);
		~Edges();

		QRect bounds();

	private:
		EdgeControler *testEdge;

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

	};

	
}
#endif // EDGES_H



