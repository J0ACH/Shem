#include "Button.h"
namespace Jui
{

	Button::Button(QWidget *parent)
		: QWidget(parent)
	{
		isPressed = false;
		isOver = false;

		name = new QString();
		icon = new QString();

		backgroundAlpha = 0;
		fadeTimeIn = 1000;
		fadeTimeOut = 250;
		fps = 25;

		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(alphaUpdate()));
	}

	void Button::setName(QString buttonName) { *name = buttonName; }

	void Button::setIcon(QString path) { *icon = path; }

	QRectF Button::boundingRect() const
	{
		return QRectF(1.5, 1.5, width() - 3, height() - 3);
	}

	void Button::alphaUpdate()
	{
		if (isOver)
		{
			if (backgroundAlpha >= 255)
			{
				timer->stop();
				backgroundAlpha = 255;
			}
			else
			{
				backgroundAlpha += 255 / fps;
			}
		}
		else
		{
			if (backgroundAlpha <= 0)
			{
				timer->stop();
				backgroundAlpha = 0;
			}
			else
			{
				backgroundAlpha -= 255 / fps;
			}
		}
		update();
	}


	void Button::paintEvent(QPaintEvent *)
	{
		QPainter painter(this);
		QRectF rect = boundingRect();

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
		if (!icon->isNull()){
			pen = new QPen(Qt::NoPen);

			QRectF target(5, 5, 20, 20);
			QRectF source(0, 0, 25, 25);
			QImage img(*icon);
			painter.drawImage(target, img, source);
		}


		painter.setPen(*pen);
		painter.setBrush(QBrush(QColor(120, 20, 20, backgroundAlpha), Qt::SolidPattern));
		painter.drawRect(boundingRect());

		painter.drawText(10, 15, QString::number(backgroundAlpha));
	}

	void Button::mousePressEvent(QMouseEvent *event)
	{
		isPressed = true;
		update();
		//QWidget::mousePressEvent(event);

		float time = 1.5;
		int frames = 10;
		float stepTime = time / frames;
		float stemAdd = 1 / frames;

		//timer->start(stepTime);

		emit pressAct();

		qDebug() << "Press"; // Or whatever
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
		emit enterAct(tr("Button_EnterAct [%1]").arg(*name));
	}

	void Button::leaveEvent(QEvent *event)
	{
		timer->stop();
		timer->setInterval(fadeTimeOut / fps);
		timer->start();

		isOver = false;
		emit leaveAct(tr("Button_LeaveAct [%1]").arg(*name));
	}

	Button::~Button()
	{

	}
}