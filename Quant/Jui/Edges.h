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
		Edges(){};
		Edges(QWidget *parent);
		~Edges();

		QRect bounds();

		bool eventFilter(QObject *target, QEvent *event);

	protected:
		//void paintEvent(QPaintEvent *event);

	private:
		//EdgeControler *ahoj;
		//QWidget *parent;		
	};

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

}
#endif // EDGES_H



