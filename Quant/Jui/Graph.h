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
		GraphPoint(QWidget *parent);
		~GraphPoint();


		bool isOver;
		QRect bounds();

	protected:
		void paintEvent(QPaintEvent *);

	private:
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

	};

	class Graph : public QWidget
	{
		Q_OBJECT

	public:
		Graph(QWidget *parent);
		~Graph();

		QRect bounds();

	signals:
		void pressAct();

		public slots:


	protected:
		void paintEvent(QPaintEvent *);
		void mouseMoveEvent(QMouseEvent *);

	private:
		void mousePressEvent(QMouseEvent *mouseEvent);
		void mouseReleaseEvent(QMouseEvent *mouseEvent);

		QPoint cursorPos;

		QList<GraphPoint*> collectionPts;
	};

}
#endif // GRAPH_H

