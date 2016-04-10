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
		testNode1->setName("node1");
		testNode1->setSourceCode("SinOsc.ar(90!2, mul: Saw.kr(2, 0.5, 0.5))");

		testNode2 = new Node(this);
		testNode2->setName("node2");

		QString longCode;
		longCode = "var osc = SinOsc.ar(( \\freq.kr(160) + Saw.kr(1/4, 5, 10) ), mul: 1, add: 0);";
		longCode.append("\nvar noise = BrownNoise.ar(\\noise.kr(0.1), (1 - \\noise.kr(0.1)));");
		longCode.append("\nvar sig = osc * noise;");
		longCode.append("\nPan2.ar(sig * \\amp.kr(0.3), \\pan.kr(0));");
		testNode2->setSourceCode(longCode);
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