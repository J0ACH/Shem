#include "NodePanel.h"

namespace QuantIDE
{
	NodePanel::NodePanel(QWidget *parent) :
		Panel(parent),
		mBridge(NULL)
	{
		setObjectName("NodePanel");

		this->initControl();
		this->fitGeometry();

		testID = 0;

		connect(this, SIGNAL(resizeAct()), this, SLOT(fitGeometry()));
		connect(buttAddNode, SIGNAL(pressAct()), this, SLOT(addNode()));
	}

	QRect NodePanel::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void NodePanel::setTargetBridge(ScBridge *target) { mBridge = target; }

	void NodePanel::initControl()
	{
		buttAddNode = new Button(this);
		buttAddNode->setText("AddNode");

		scrollArea = new QScrollArea(this);
		scrollArea->setFrameStyle(QFrame::NoFrame);
		scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		scrollArea->setStyleSheet("background-color: rgba(0,0,0,0)");

		scrollWidget = new QWidget();
		scrollWidget->setAutoFillBackground(true);
		scrollWidget->setStyleSheet("background-color: rgba(0,0,0,0)");
		scrollArea->setWidget(scrollWidget);

		scrollWidget->show();
	}

	void NodePanel::addNode()
	{
		qDebug("NodePanel::addNode()");

		Node *newNode = new Node(scrollWidget);
		newNode->setName(tr("test%1").arg(QString::number(testID)));
		newNode->setSourceCode("SinOsc.ar(\\freq.kr(90)!2, mul: Saw.kr(2, 0.5, 0.5))");
		newNode->connectBridge(mBridge);
		newNode->show();

		connect(this, SIGNAL(resizeAct()), newNode, SLOT(fitGeometry()));
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
		int nextNodeOriginY = 0;
		buttAddNode->setGeometry(200, 10, 50, 20);
		scrollArea->setGeometry(5, 50, width() - 10, height() - 100);

		for (auto e : dictNode.keys())
		{
			Node *oneNode = dictNode.value(e);
			//	fout << e << "," << dictNode->value(e) << '\n';
			oneNode->setGeometry(0, nextNodeOriginY, scrollArea->width() - 5, 200);
			nextNodeOriginY = dictNode.value(e)->geometry().top() + dictNode.value(e)->height();

			scrollArea->widget()->setGeometry(0, 0, scrollArea->width(), nextNodeOriginY);
			nextNodeOriginY += 10;
		}
	}

	NodePanel::~NodePanel()
	{

	}
}