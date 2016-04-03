#include "Node.h"

namespace QuantIDE
{

	Node::Node(QWidget *parent) : QWidget(parent)
	{
		setObjectName("Node");
				
		this->initControl();

		//connect(closeButton, SIGNAL(pressAct()), this, SLOT(close()));
	}

	QRect Node::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void Node::initControl()
	{
		nameLabel = new QLabel(this);
		nameLabel->setGeometry(5,5,80,30);
		nameLabel->setText("Node1");

		testButton = new QPushButton(this);
		testButton->setGeometry(15, 15, 80, 30);
		testButton->setText("test");
	}

	void Node::paintEvent(QPaintEvent *paintEvent)
	{
		QPainter painter(this);
		painter.fillRect(QRect(0, 0, width() - 1, height() - 1), QColor(120, 20, 20));
	}

	Node::~Node()
	{

	}
}