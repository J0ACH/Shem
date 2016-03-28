#include "EdgeControler.h"

namespace Jui
{
	// EdgeControler musi byt nacten pred Edge
		EdgeControler::EdgeControler(QWidget *parent) : QWidget(parent)
	{

		setObjectName("EdgeControler");
		qDebug("Class(%s) -> parent(%s)",
			qPrintable(objectName()),
			qPrintable(this->parent()->objectName())
			//qPrintable(this->parentWidget()->objectName())
			);

		setEdgeOffset(15);

		isOver = false;

		backgroundAlpha = 0;
		fadeTimeIn = 1000;
		fadeTimeOut = 250;
		fps = 25;

		timer = new QTimer(this);

		mCursorGlobal = new QPoint(0, 0);
		mCursorLocal = new QPoint(0, 0);
		mFrameOriginGlobal = new QPoint(0, 0);

		connect(timer, SIGNAL(timeout()), this, SLOT(alphaUpdate()));
		
	}

		
	void EdgeControler::setParentObject(QObject *parentConteiner)
	{
		conteiner = parentConteiner;
		//connect(this, SIGNAL(moveAct()), conteiner, SLOT(edgeMoved2()));
		connect(this, SIGNAL(moveAct(EdgePosition)), conteiner, SLOT(edgeMoved(EdgePosition)));
	}
		

	QRect EdgeControler::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void EdgeControler::setDirection(EdgePosition direction)
	{
		side = direction;
		this->fitGeometry();
		//		update();
	}

	void EdgeControler::fitGeometry()
	{
		switch (side)
		{
		case Jui::LEFT:
			this->setGeometry(0, 0, edgeOffset, this->parentWidget()->height());
			break;
		case Jui::TOP:
			break;
		case Jui::RIGHT:
			this->setGeometry(
				this->parentWidget()->width() - edgeOffset,
				0,
				edgeOffset,
				this->parentWidget()->height()
				);
			break;
		case Jui::BOTTOM:
			break;
		case Jui::ALL:
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

	void EdgeControler::resizeEvent(QResizeEvent *resizeEvent)
	{
		this->fitGeometry();
		qDebug("EdgeControler::resizeEvent");
	}

	void EdgeControler::mousePressEvent(QMouseEvent *mouseEvent)
	{
		*mCursorGlobal = mouseEvent->globalPos();
		*mCursorLocal = mouseEvent->pos();
		*mFrameOriginGlobal = QPoint(
			mCursorGlobal->x() - mCursorLocal->x(),
			mCursorGlobal->y() - mCursorLocal->y()
			);

		qDebug() << tr("pressedGlobal [%1,%2]").arg(
			QString::number(mCursorGlobal->x()),
			QString::number(mCursorGlobal->y())
			);

		//msgConsole(tr("pressedLocal [%1,%2]").arg(QString::number(mCursorLocal->x()), QString::number(mCursorLocal->y())));
		//msgConsole(tr("frameOriginGlobal [%1,%2]").arg(QString::number(mFrameOriginGlobal->x()), QString::number(mFrameOriginGlobal->y())));
	}

	void EdgeControler::mouseMoveEvent(QMouseEvent *mouseEvent)
	{
		QPoint mouseCurrentGlobal = mouseEvent->globalPos();
		//int posX = mFrameOriginGlobal->x() - mCursorGlobal->x() + mouseCurrentGlobal.x();
		int posX = -mCursorGlobal->x() + mouseCurrentGlobal.x();
		int posY = 0;

		//msgConsole(tr("mCursor [%1,%2]").arg(QString::number(posX), QString::number(posY)));


		//this->move(posX, 0);
		this->parentWidget()->setGeometry(posX, 0, this->parentWidget()->width(), this->parentWidget()->height());

		switch (side)
		{
		case Jui::LEFT:
			//this->setGeometry(0, 0, edgeOffset, this->parentWidget()->height());
			//	this->parent.move(posX, posY);
			break;
		case Jui::TOP:
			break;
		case Jui::RIGHT:
			this->move(posX, 0);

			break;
		case Jui::BOTTOM:
			break;
		case Jui::ALL:
			break;
		default:
			break;
		}
				
		emit moveAct(side);
		qDebug("Signal moveAct send...");
	}

	void EdgeControler::enterEvent(QEvent *event)
	{
		timer->stop();
		timer->setInterval(fadeTimeIn / fps);
		timer->start();

		isOver = true;
		//emit enterAct(tr("Button_EnterAct [%1]").arg(name));

		qDebug("EdgeControler::enterEvent");
	}

	void EdgeControler::leaveEvent(QEvent *event)
	{
		timer->stop();
		timer->setInterval(fadeTimeOut / fps);
		timer->start();

		isOver = false;
		//emit leaveAct(tr("Button_LeaveAct [%1]").arg(name));
		qDebug("EdgeControler::leaveEvent");
	}

	EdgeControler::~EdgeControler()	{	}

}
