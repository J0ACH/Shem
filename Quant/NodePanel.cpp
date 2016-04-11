#include "NodePanel.h"

namespace QuantIDE
{
	NodePanel::NodePanel(QWidget *parent) :
		Panel(parent),
		mBridge(NULL)
	{
		setObjectName("NodePanel");
		//dictNode = new QMap<QString, Node>();
		this->initControl();
		this->fitGeometry();

		testID = 0;

		connect(this, SIGNAL(resizeAct()), this, SLOT(fitGeometry()));
		//connect(this, SIGNAL(resizeAct()), testNode1, SLOT(fitGeometry()));
		//connect(this, SIGNAL(resizeAct()), testNode2, SLOT(fitGeometry()));

		connect(buttAddNode, SIGNAL(pressAct()), this, SLOT(addNode()));
	}

	QRect NodePanel::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void NodePanel::initControl()
	{
		buttAddNode = new Button(this);
		buttAddNode->setText("AddNode");
		

		QString longCode;
		longCode = "var osc = SinOsc.ar(( \\freq.kr(160) + Saw.kr(1/4, 5, 10) ), mul: 1, add: 0);";
		longCode.append("\nvar noise = BrownNoise.ar(\\noise.kr(0.1), (1 - \\noise.kr(0.1)));");
		longCode.append("\nvar sig = osc * noise;");
		longCode.append("\nPan2.ar(sig * \\amp.kr(0.3), \\pan.kr(0));");
		//testNode2->setSourceCode(longCode);
	}

	void NodePanel::setTargetBridge(ScBridge *target) { mBridge = target; }

	void NodePanel::addNode()
	{
		qDebug("NodePanel::addNode()");

		Node *newNode = new Node(this);
		newNode->setName(tr("test%1").arg(QString::number(testID)));
		newNode->setSourceCode("SinOsc.ar(90!2, mul: Saw.kr(2, 0.5, 0.5))");
		newNode->show();

		connect(this, SIGNAL(resizeAct()), newNode, SLOT(fitGeometry()));
		connect(newNode, SIGNAL(evaluateAct(QString)), mBridge, SLOT(evaluateCode(QString)));
		connect(newNode, SIGNAL(killAct(QString)), this, SLOT(deleteNode(QString)));

		dictNode.insert(newNode->name(), newNode);

		emit resizeAct();
		testID += 1;
	}

	void NodePanel::deleteNode(QString nodeName)
	{
		dictNode.remove(nodeName);
		emit resizeAct();
	}

	void NodePanel::fitGeometry()
	{
		int nextNodeOriginY = 50;
		buttAddNode->setGeometry(200, 10, 50, 20);

		for (auto e : dictNode.keys())
		{
			Node *oneNode = dictNode.value(e);
			//	fout << e << "," << dictNode->value(e) << '\n';
			//Node &oneNode = dictNode.value(e);
			//dictNode.value(e)->setGeometry(10, nextNodeOriginY, this->width() - 20, 200);
			//nextNodeOriginY = dictNode.value(e)->geometry().top() + dictNode.value(e)->height() + 10;
			oneNode->setGeometry(10, nextNodeOriginY, this->width() - 20, 200);
			nextNodeOriginY = dictNode.value(e)->geometry().top() + dictNode.value(e)->height() + 10;
		}
	}
	
	NodePanel::~NodePanel()
	{

	}
}