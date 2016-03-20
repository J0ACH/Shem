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

		backgroundAlpha = 0;
		fadeTimeIn = 1000;
		fadeTimeOut = 250;
		fps = 25;

		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(alphaUpdate()));
	}

	void Button::setText(QString buttonName) { name = buttonName; }

	void Button::setIcon(QString path) { icon = path; }

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
			//pen = new QPen(Qt::NoPen);
			painter.fillRect(bounds(), QColor(120, 20, 20, backgroundAlpha));

			pen = new QPen(Qt::red, 1);

			QRectF target(7, 7, 16, 16);
			QRectF source(0, 0, 128, 128);
			//QImage img("C:/GitHub/Shem/Quant/Jui/Images/IconClose.png");
			QImage img(":/close128.png");
			if (img.isNull())
			{
				qDebug() << tr("ButtonImg nenalezen");
			};
			painter.drawImage(target, img, source);
			
			painter.setPen(*pen);
		//	painter.drawRect(bounds());

			qDebug() << tr("ButtonImg [%1, %2, %3, %4]").arg(
				QString::number(img.rect().left()),
				QString::number(img.rect().top()),
				QString::number(img.size().width()),
				QString::number(img.size().height())
				);
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
		emit enterAct(tr("Button_EnterAct [%1]").arg(name));

		QVariant posX(bounds().left());
		QVariant posY(bounds().top());
		QVariant sizeX(bounds().width());
		QVariant sizeY(bounds().height());

		qDebug() << tr("Button (%1, icon: %2)::enterEvent [%3, %4, %5, %6]").arg(
			name, 
			icon,
			posX.toString(),
			posY.toString(),
			sizeX.toString(),
			sizeY.toString()
			);
	}

	void Button::leaveEvent(QEvent *event)
	{
		timer->stop();
		timer->setInterval(fadeTimeOut / fps);
		timer->start();

		isOver = false;
		emit leaveAct(tr("Button_LeaveAct [%1]").arg(name));
		qDebug("Button::leaveEvent");
	}

	Button::~Button()
	{

	}
}