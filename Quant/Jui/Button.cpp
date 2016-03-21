#include "Button.h"

#include "string.h"

namespace Jui
{

	Button::Button(QWidget *parent) : QWidget(parent)
	{
		isPressed = false;
		isOver = false;

		//name = "";
		//icon = new QString();
		iconOffset = 0;

		backgroundAlpha = 0;
		fadeTimeIn = 1000;
		fadeTimeOut = 250;
		fps = 25;

		timer = new QTimer(this);

		connect(timer, SIGNAL(timeout()), this, SLOT(alphaUpdate()));
	}

	void Button::setText(QString buttonName) { name = buttonName; }

	void Button::setIcon(QImage img, int offset = 0) { icon = img; iconOffset = offset; }

	QRect Button::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void Button::alphaUpdate()
	{
		if (isOver)
		{
			backgroundAlpha += 255 / fps;
			if (backgroundAlpha >= 255)
			{
				timer->stop();
				backgroundAlpha = 255;
			}
		}
		else
		{
			backgroundAlpha -= 255 / fps;
			if (backgroundAlpha <= 0)
			{
				timer->stop();
				backgroundAlpha = 0;
			}
		}
		//qDebug() << tr("buttonAlpha %1").arg(QString::number(backgroundAlpha));
		update();
	}

	void Button::paintEvent(QPaintEvent *)
	{
		QPainter painter(this);

		QPen *pen;
		if (isPressed){
			pen = new QPen(Qt::red, 3);
		}
		else
		{
			if (isOver){
				pen = new QPen(Qt::white, 3);
			}
			else
			{
				pen = new QPen(Qt::white, 1);
			}
		}

		if (!icon.isNull()){

			painter.fillRect(bounds(), QColor(120, 20, 20, backgroundAlpha));

			QRectF target(
				iconOffset,
				iconOffset,
				this->width() - 2 * iconOffset - 1,
				this->height() - 2 * iconOffset - 1
				);
			QRectF source(0, 0, icon.width(), icon.height());

			painter.drawImage(target, icon, source);
		}
		else
		{
			painter.fillRect(bounds(), QColor(120, 20, 20, backgroundAlpha));
			painter.setPen(*pen);
			painter.setBrush(QBrush(QColor(120, 20, 20, backgroundAlpha), Qt::SolidPattern));
			painter.drawRect(bounds());

			if (!name.isNull())
			{
				painter.drawText(10, 15, name);
			}
			else
			{
				painter.drawText(10, 15, QString::number(backgroundAlpha));
			}
		}
	}

	void Button::mousePressEvent(QMouseEvent *event)
	{
		isPressed = true;
		update();

		float time = 1.5;
		int frames = 10;
		float stepTime = time / frames;
		float stemAdd = 1 / frames;

		emit pressAct();
		qDebug() << tr("Button (%1) pressed").arg(name);
	}

	void Button::mouseReleaseEvent(QMouseEvent *event)
	{
		isPressed = false;
		update();

		QWidget::mouseReleaseEvent(event);
	}

	void Button::enterEvent(QEvent *event)
	{
		timer->stop();
		timer->setInterval(fadeTimeIn / fps);
		timer->start();

		isOver = true;
		//emit enterAct(tr("Button_EnterAct [%1]").arg(name));

		qDebug() << tr("Button::enterEvent");

		QWidget::enterEvent(event);
	}

	void Button::leaveEvent(QEvent *event)
	{
		timer->stop();
		timer->setInterval(fadeTimeOut / fps);
		timer->start();

		isOver = false;
		//emit leaveAct(tr("Button_LeaveAct [%1]").arg(name));
		qDebug() << tr("Button (%1, icon: %2)::leaveEvent");

		QWidget::leaveEvent(event);
	}

	Button::~Button()
	{

	}
}