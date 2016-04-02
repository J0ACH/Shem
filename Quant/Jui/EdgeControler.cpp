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

		painter.setPen(QPen(Qt::yellow, 3));
		painter.drawRect(bounds());

		if (isOver){
			painter.fillRect(bounds(), QColor(120, 120, 20, backgroundAlpha));
		};
		//qDebug("EdgesControler::paintEvent");
	}

	void EdgeControler::mousePressEvent(QMouseEvent *mouseEvent)
	{
		emit pressAct();
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
	}

	void EdgeControler::enterEvent(QEvent *event)
	{
		timer->stop();
		timer->setInterval(fadeTimeIn / fps);
		timer->start();

		isOver = true;
		//emit enterAct(tr("Button_EnterAct [%1]").arg(name));

		//	qDebug("EdgeControler::enterEvent");
	}

	void EdgeControler::leaveEvent(QEvent *event)
	{
		timer->stop();
		timer->setInterval(fadeTimeOut / fps);
		timer->start();

		isOver = false;
		//emit leaveAct(tr("Button_LeaveAct [%1]").arg(name));
		//	qDebug("EdgeControler::leaveEvent");
	}

	EdgeControler::~EdgeControler()	{	}

}
