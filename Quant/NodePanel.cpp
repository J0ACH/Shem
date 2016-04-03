#include "NodePanel.h"

namespace QuantIDE
{
	NodePanel::NodePanel(QWidget *parent) : Panel(parent)
	{
		setObjectName("NodePanel");

	//	this->initControl();

		//connect(closeButton, SIGNAL(pressAct()), this, SLOT(close()));
	}

	QRect NodePanel::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void NodePanel::initControl()
	{
		nameLabel = new QLabel(this);
		nameLabel->setGeometry(5, 5, 80, 30);
		nameLabel->setText("Node1");

		testButton = new QPushButton(this);
		testButton->setGeometry(15, 15, 80, 30);
		testButton->setText("test");
	}

	/*
	void NodePanel::paintEvent(QPaintEvent *paintEvent)
	{
	QPainter painter(this);
	painter.fillRect(QRect(0, 0, width() - 1, height() - 1), QColor(120, 20, 20));
	}
	*/

	NodePanel::~NodePanel()
	{

	}
}