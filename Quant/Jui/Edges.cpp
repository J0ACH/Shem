#include "Edges.h"

namespace Jui
{
	// EdgeControler musi byt nacten pred Edge
	EdgeControler::EdgeControler(QWidget *parent) : QWidget(parent)
	{
		setEdgeOffset(15);

		isOver = false;

		backgroundAlpha = 0;
		fadeTimeIn = 1000;
		fadeTimeOut = 250;
		fps = 25;

		timer = new QTimer(this);

		connect(timer, SIGNAL(timeout()), this, SLOT(alphaUpdate()));
	}

	QRect EdgeControler::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void EdgeControler::setDirection(EdgePosition direction)
	{
		side = direction;
		update();
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

		qDebug("EdgeControler::resizeEvent");
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






	Edges::Edges(QWidget *parent) : QWidget(parent)
	{
		setGeometry(0, 0, parent->width(), parent->height());
		setEdgeOffset(15);

		//setAttribute(Qt::WA_TransparentForMouseEvents, true);
		//setAttribute(Qt::WA_NoMousePropagation);

		//testEdge = new EdgeControler(this);
		testEdge = new EdgeControler(this);
		testEdge->setGeometry(0, 0, 150, 150);
		testEdge->setDirection(EdgePosition::LEFT);
		//testEdge->setDirection(EdgePosition::RIGHT);

		testButton = new Button(this);
		testButton->setGeometry(100, 100, 50, 50);

		//std::list<EdgeControler> controlers;
		//std::list<QWidget> controlers;

		//QList<EdgeControler*> test;
		//QList<EdgeControler> test;

		//this->setGeometry(parent->geometry());
		//EdgePosition position(EdgePosition::RIGHT);

		//QRect frame = parent->geometry();
		//EdgeControler *edge = new EdgeControler(this);
		//ahoj = new EdgeControler(this);


		//controlers.emplace_back(edge);
		//controlers.push_back(edge);
		//test.append(edge);


	}

	QRect Edges::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void Edges::setEdgeOffset(int offset) { edgeOffset = offset; }

	void Edges::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);

		// r3: intersection

		//painter.setClipRegion(r3);

		painter.fillRect(bounds(), QColor(255, 30, 30, 100));

		painter.setPen(QPen(Qt::red, 1));
		//painter.drawRect(bounds().adjusted(2, 2, -2, -2));
		painter.drawRect(bounds());

		//	qDebug("Edges::paintEvent");
	}

	void Edges::resizeEvent(QResizeEvent *resizeEvent)
	{

		QRegion r1(bounds());
		QRegion r2(bounds().adjusted(edgeOffset, edgeOffset, -edgeOffset, -edgeOffset));
		QRegion r3 = r1.subtracted(r2);
		this->setMask(r3);

		qDebug("Edges::resizeEvent");
	}

	Edges::~Edges()
	{

	}






}