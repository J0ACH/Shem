#include "Button.h"

#include "string.h"

namespace Jui
{

	Button::Button(QWidget *parent) : QWidget(parent)
	{
		isPressed = false;
		isOver = false;

		normalColor = QColor(90, 90, 90);
		overColor = QColor(230, 230, 230);
		activeColor = QColor(20, 180, 240);

		iconOffset = 0;

		backgroundAlpha = 0;
		fadeTimeIn = 500;
		fadeTimeOut = 250;
		fps = 25;

		timer = new QTimer(this);

		connect(timer, SIGNAL(timeout()), this, SLOT(alphaUpdate()));
	}

	void Button::setText(QString buttonName) { name = buttonName; }

	void Button::setIcon(QImage img, int offset = 0) { icon = img; iconOffset = offset; }

	void Button::setNormalColor(QColor color){ normalColor = color; }
	void Button::setOverColor(QColor color){ overColor = color; }
	void Button::setActiveColor(QColor color){ activeColor = color; }

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

	static QColor blendColorAA(const QColor& color1, const QColor& color2, qreal ratio)
	{
		int r = color1.red()*(1 - ratio) + color2.red()*ratio;
		int g = color1.green()*(1 - ratio) + color2.green()*ratio;
		int b = color1.blue()*(1 - ratio) + color2.blue()*ratio;

		return QColor(r, g, b, 255);
	}

	void Button::paintEvent(QPaintEvent *event)
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
			float moveX = (this->width() - icon.width()) / 2 - 1;
			float moveY = (this->height() - icon.height()) / 2;

			painter.fillRect(bounds(), QColor(120, 20, 20, backgroundAlpha));

			/*
			QRectF target(
			iconOffset,
			iconOffset,
			this->width() - 2 * iconOffset - 1,
			this->height() - 2 * iconOffset - 1
			);
			*/

			QRectF target(moveX, moveY, icon.width(), icon.height());
			QRectF source(0, 0, icon.width(), icon.height());
			//painter.drawImage(target, icon, source);

			QImage renderedIcon(icon);
			// fill with color
			if (isOver)
			{
				renderedIcon.fill(overColor);
			}
			else
			{
				if (isPressed) { renderedIcon.fill(activeColor); }
				else { renderedIcon.fill(normalColor); };
			}
			//renderedIcon.fill(QColor(90, 90, 90));
			// set alpha-map, black pixels -> opacity of 0, white pixels -> opacity 1
			renderedIcon.setAlphaChannel(icon);
			painter.drawImage(target, renderedIcon, source);  // draw image to QWidget

		}
		else
		{
			painter.fillRect(bounds(), QColor(120, 20, 20, backgroundAlpha));
			painter.setPen(*pen);
			painter.setBrush(QBrush(QColor(120, 20, 20, backgroundAlpha), Qt::SolidPattern));
			painter.drawRect(bounds());

			if (!name.isNull())
			{
				QTextOption opt;
				opt.setAlignment(Qt::AlignCenter);

				painter.drawText(bounds(), name, opt);
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