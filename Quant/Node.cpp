#include "Node.h"

namespace QuantIDE
{

	Node::Node(QWidget *parent) : QWidget(parent)
	{
		setObjectName("Node");

		this->initControl();
		this->fitGeometry();

		//connect(closeButton, SIGNAL(pressAct()), this, SLOT(close()));
	}

	QRect Node::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void Node::initControl()
	{
		nameLabel = new QLabel(this);
		nameLabel->setText("Node1");

		testButton = new QPushButton(this);
		testButton->setText("test");
	}

	void Node::fitGeometry()
	{
		nameLabel->setGeometry(10, 5, 80, 30);
		testButton->setGeometry(10, 35, 80, 30);
	}

	void Node::paintEvent(QPaintEvent *paintEvent)
	{
		QPainter painter(this);
		painter.fillRect(this->bounds(), QColor(30, 30, 30));

		painter.setPen(QColor(230, 230, 230));
		painter.drawLine(0, 0, width(), 0);
		painter.drawLine(0, height()-1, width(), height()-1);
	}

	Node::~Node()
	{

	}
}