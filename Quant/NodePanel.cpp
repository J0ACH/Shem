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
		buttAddNode = new Button(this);
		buttAddNode->setGeometry(200, 5, 80, 30);
		buttAddNode->setText("AddNode");

		testNode = new Node(this);
		testNode->setGeometry(15, 35, 400, 200);
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