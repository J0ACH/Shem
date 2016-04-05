#include "NodePanel.h"

namespace QuantIDE
{
	NodePanel::NodePanel(QWidget *parent) : Panel(parent)
	{
		setObjectName("NodePanel");

		this->initControl();
		this->fitGeometry();

		connect(this, SIGNAL(resizeAct()), this, SLOT(fitGeometry()));
		connect(this, SIGNAL(resizeAct()), testNode1, SLOT(fitGeometry()));
		connect(this, SIGNAL(resizeAct()), testNode2, SLOT(fitGeometry()));
	}

	QRect NodePanel::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void NodePanel::initControl()
	{
		buttAddNode = new Button(this);
		buttAddNode->setText("AddNode");

		testNode1 = new Node(this);
		testNode2 = new Node(this);
	}

	void NodePanel::fitGeometry()
	{
		buttAddNode->setGeometry(200, 10, 50, 20);
		testNode1->setGeometry(10, 50, this->width() - 20, 200);
		testNode2->setGeometry(10, 260, this->width() - 20, 200);
	}

	NodePanel::~NodePanel()
	{

	}
}