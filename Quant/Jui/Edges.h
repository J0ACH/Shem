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

	class Edges : public QWidget
	{
		Q_OBJECT

	public:
		Edges(QWidget *parent = 0);
		~Edges();

		QRect bounds();
		
	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

	private:
	//	QWidget *parent;		
		//EdgeControler *ahoj;
	};

	/*
	class EdgeControler : public QWidget
	{
		Q_OBJECT

	public:
		EdgeControler(QWidget *parent);
		~EdgeControler();

	protected:
		void paintEvent(QPaintEvent *event);

		//private:
		//QWidget *parent;

	};
	*/

}
#endif // EDGES_H



