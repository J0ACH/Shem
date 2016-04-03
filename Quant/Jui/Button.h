#ifndef BUTTON_H
#define BUTTON_H

#include <QWidget>
#include <QPainter>
#include <QFrame>
#include <QTimer>
#include <QDebug>

#include <QMouseEvent>

namespace Jui
{

	class Button : public QWidget
	{
		Q_OBJECT

	public:
		Button(QWidget *parent);
		~Button();

		QRect bounds();

		void setIcon(QImage, int);
		void setText(QString name);

		void setNormalColor(QColor);
		void setOverColor(QColor);
		void setActiveColor(QColor);

	signals:
		void pressAct();
		//void enterAct(QString name);
		//void leaveAct(QString name);

	public slots:
		void alphaUpdate();

	protected:
		void paintEvent(QPaintEvent *);

	private:
		bool isPressed, isOver;

		QString name;
		QImage icon;
		QColor normalColor, overColor, activeColor;
		QColor blendColor(QColor, QColor, qreal);
		int iconOffset;
		
		void mousePressEvent(QMouseEvent *mouseEvent);
		void mouseReleaseEvent(QMouseEvent *mouseEvent);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

		//void addAction();
		int fadeTimeIn;
		int fadeTimeOut;
		int fps;
		int backgroundAlpha;

		QTimer *timer;

	


	};

}
#endif // BUTTON_H

