#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QEvent>

#include "Button.h"
#include "Edges.h"


namespace Jui
{
	class Panel : public QWidget
	{
		Q_OBJECT

	public:
		Panel(QWidget *parent = 0);
		~Panel();

		QRect bounds();

		void initControl();

		void setTitle(QString);
		void setBackground(QColor);

		void setEdgeControler(EdgeControler::Direction, bool);

		//bool eventFilter(QObject *target, QEvent *event);
	signals:
		void resizeAct();
	
	protected:
		void mousePressEvent(QMouseEvent *mouseEvent);
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

	private:
		QString title;
		QColor backColor;
				
		Button *closeButton;

		Edges *edges;
	};



}

#endif // PANEL