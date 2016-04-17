#include "Panel.h"

namespace Jui
{

	Panel::Panel(QWidget *parent) : QWidget(parent)
	{
		//this->setParent(parent);
		setObjectName("Panel");

		title = "panel";
		colorPanelBackground = QColor(120, 120, 120);

		this->initControl();

		connect(closeButton, SIGNAL(pressAct()), this, SLOT(hide()));
	}

	QRect Panel::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

	void Panel::initControl()
	{
		closeButton = new Button(this);
		closeButton->setGeometry(this->width() - 30, 10, 16, 16);
		closeButton->setIcon(QImage(":/smallClose16.png"), 0);
		closeButton->setText("X");

		edges = new Edges(this);
	}

	void Panel::setTitle(QString name) { title = name; }

	void Panel::setColorBackground(QColor color) { colorPanelBackground = color; update(); }
	void Panel::setColorTitle(QColor color) { colorText = color; update(); }

	void Panel::setEdgeControler(EdgeControler::Direction direction, bool visible)
	{
		switch (direction)
		{
		case EdgeControler::Direction::LEFT:
			if (visible)
			{
				edges->addManipulator(EdgeControler::Direction::LEFT);
			}
			break;
		case EdgeControler::Direction::TOP:
			if (visible)
			{
				edges->addManipulator(EdgeControler::Direction::TOP);
			}
			break;
		case EdgeControler::Direction::RIGHT:
			if (visible)
			{
				edges->addManipulator(EdgeControler::Direction::RIGHT);
			}
			break;
		case EdgeControler::Direction::BOTTOM:
			if (visible)
			{
				edges->addManipulator(EdgeControler::Direction::BOTTOM);
			}
			break;
		}
	}

	void Panel::mousePressEvent(QMouseEvent *mouseEvent) { mouseEvent->accept(); }

	void Panel::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);
		painter.setFont(QFont("Univers Condensed", 12, QFont::Normal));
		painter.fillRect(bounds(), colorPanelBackground);

		painter.setPen(QPen(colorText, 1));
		painter.drawText(15, 25, title);
	}

	void Panel::resizeEvent(QResizeEvent *resizeEvent)
	{
		closeButton->setGeometry(this->width() - 30, 10, 16, 16);
		emit resizeAct();

		//qDebug("Panel::resizeEvent");
	}

	Panel::~Panel()
	{

	}
}