#include "EdgeControler.h"

namespace Jui
{
	EdgeControler::EdgeControler(QWidget *parent) : QWidget(parent)
	{

		setObjectName("EdgeControler");
		setEdgeOffset(15);

		isOver = false;

		backgroundAlpha = 0;
		fadeTimeIn = 1000;
		fadeTimeOut = 250;
		fps = 25;

		timer = new QTimer(this);

		connect(timer, SIGNAL(timeout()), this, SLOT(alphaUpdate()));
	}

	void EdgeControler::setParentObject(QObject *parentConteiner)
	{
		conteiner = parentConteiner;

		connect(this, SIGNAL(pressAct()), conteiner, SLOT(edgePressed()));
		connect(
			this, SIGNAL(moveAct(EdgeControler::Direction, int)),
			conteiner, SLOT(edgeMoved(EdgeControler::Direction, int))
			);
	}

	QRect EdgeControler::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void EdgeControler::setDirection(Direction direction)
	{
		side = direction;
		this->fitGeometry();
	}

	void EdgeControler::fitGeometry()
	{
		switch (side)
		{
		case LEFT:
			this->setGeometry(
				0,
				edgeOffset,
				edgeOffset,
				this->parentWidget()->height() - 2 * edgeOffset
				);
			break;
		case TOP:
			this->setGeometry(
				edgeOffset,
				0,
				this->parentWidget()->width() - 2 * edgeOffset,
				edgeOffset
				);
			break;
		case RIGHT:
			this->setGeometry(
				this->parentWidget()->width() - edgeOffset,
				edgeOffset,
				edgeOffset,
				this->parentWidget()->height() - 2 * edgeOffset
				);
			break;
		case BOTTOM:
			this->setGeometry(
				edgeOffset,
				this->parentWidget()->height() - edgeOffset,
				this->parentWidget()->width() - 2 * edgeOffset,
				edgeOffset
				);
			break;
		default:
			break;
		}
	}

	void EdgeControler::setEdgeOffset(int offset) { edgeOffset = offset; }

	void EdgeControler::alphaUpdate()
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

	void EdgeControler::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		if (isOver){
			painter.setPen(QPen(Qt::white, 1));
		}
		else
		{
			painter.setPen(QPen(QColor(90, 90, 90), 1));
		}

		int lineOffset = 3;
		switch (side)
		{
		case LEFT:
			painter.drawLine(QLine(lineOffset, 0, lineOffset, height()));
			break;
		case TOP:
			painter.drawLine(QLine(0, lineOffset, width(), lineOffset));
			break;
		case RIGHT:
			painter.drawLine(QLine(width() - lineOffset - 1, 0, width() - lineOffset - 1, height()));
			break;
		case BOTTOM:
			painter.drawLine(QLine(0, height() - lineOffset - 1, width(), height() - lineOffset - 1));
			break;
		default:
			painter.drawRect(bounds());
			break;
		}
	}

	void EdgeControler::mousePressEvent(QMouseEvent *mouseEvent)
	{
		emit pressAct();
		mouseEvent->accept();
	}

	void EdgeControler::mouseMoveEvent(QMouseEvent *mouseEvent)
	{
		int newValue;
		switch (side)
		{
		case LEFT:
			newValue = mouseEvent->globalX();
			break;
		case TOP:
			newValue = mouseEvent->globalY();
			break;
		case RIGHT:
			newValue = mouseEvent->globalX();
			break;
		case BOTTOM:
			newValue = mouseEvent->globalY();
			break;
		default:
			break;
		}

		emit moveAct(side, newValue);
		this->fitGeometry();
		mouseEvent->accept();
	}

	void EdgeControler::enterEvent(QEvent *event)
	{
		timer->stop();
		timer->setInterval(fadeTimeIn / fps);
		timer->start();

		isOver = true;
	}

	void EdgeControler::leaveEvent(QEvent *event)
	{
		timer->stop();
		timer->setInterval(fadeTimeOut / fps);
		timer->start();

		isOver = false;
	}

	EdgeControler::~EdgeControler()	{	}

}
