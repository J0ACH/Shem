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
		testNode1->setName("Node1");
		testNode1->setSourceCode("SinOsc.ar(90!2)");

		testNode2 = new Node(this);
		testNode2->setName("Node2");
		testNode2->setSourceCode("Saw.ar(120!2)");
	}

	void NodePanel::setTargetCanvan(Canvan *target) 
	{
		connect(testNode1, SIGNAL(evaluateAct(QString)), target, SLOT(println(QString)));
		connect(testNode2, SIGNAL(evaluateAct(QString)), target, SLOT(println(QString)));
	}

	void NodePanel::setTargetBridge(ScBridge *target)
	{
		connect(testNode1, SIGNAL(evaluateAct(QString)), target, SLOT(evaluateCode(QString)));
		connect(testNode2, SIGNAL(evaluateAct(QString)), target, SLOT(evaluateCode(QString)));
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