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
		void setColorBackground(QColor);
		void setColorTitle(QColor);

		void setEdgeControler(EdgeControler::Direction, bool);

	public slots:
		void onConfigData(QMap<QString, QVariant*> config);
		
	signals:
		void resizeAct();
	
	protected:
		void mousePressEvent(QMouseEvent *mouseEvent);
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

	private:
		QString title;

		QColor colorPanelBackground, colorNormal, colorOver, colorActive, colorText;
		QFont fontTextBig;

		Button *closeButton;
		Edges *edges;

		
	};



}

#endif // PANEL