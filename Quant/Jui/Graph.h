#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QPainter>
#include <QFrame>
#include <QTimer>
#include <QDebug>

#include <QMouseEvent>

namespace Jui
{
	class GraphPoint : public QWidget
	{
		Q_OBJECT

	public:
		GraphPoint(QWidget *parent, int ptID); // !!! cesta jak udelat konstruktor neodvozeny od materske tridy
		~GraphPoint();

		bool isOver;
		QRect bounds();

	signals:
		void actDelete(int ID);

	protected:
		void paintEvent(QPaintEvent *);
		virtual bool eventFilter(QObject * watched, QEvent * event);

	private:
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

		int ID;

	};

	class Graph : public QWidget
	{
		Q_OBJECT

	public:
		Graph(QWidget *parent);
		~Graph();

		QRect bounds();
		
	public slots:
		void onDeletePoint(int ID);

	protected:
		void paintEvent(QPaintEvent *);
		void mouseMoveEvent(QMouseEvent *);

	private:
		void mousePressEvent(QMouseEvent *mouseEvent);
		void mouseReleaseEvent(QMouseEvent *mouseEvent);

		QPoint cursorPos;

		QMap<int, GraphPoint*> collectionPts;
		
	};

}
#endif // GRAPH_H

