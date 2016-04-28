#include "NodePanel.h"

namespace QuantIDE
{
	NodePanel::NodePanel(QWidget *parent) :
		Panel(parent),
		mBridge(NULL)
	{
		setObjectName("NodePanel");

		this->initControl();
		//this->fitGeometry();

		connect(this, SIGNAL(resizeAct()), this, SLOT(fitGeometry()));
		connect(buttAddNode, SIGNAL(pressAct()), this, SLOT(addNode()));
	}

	QRect NodePanel::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

	void NodePanel::setTargetBridge(ScBridge *target) { mBridge = target; }

	void NodePanel::initControl()
	{
		buttAddNode = new Button(this);
		buttAddNode->setText("AddNode");
		buttAddNode->setGeometry(200, 10, 50, 20);

		scrollArea = new QScrollArea(this);
		//scrollArea->setFrameStyle(QFrame::NoFrame);
		scrollArea->move(5, 50);
		scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		scrollArea->setStyleSheet("background-color: rgba(0,0,0,0)");
		//scrollArea->setStyleSheet("background-color: rgba(120,120,120,255)");
		//scrollArea->setWidgetResizable(true);

		scrollWidget = new QWidget(this);
		scrollWidget->setAutoFillBackground(true);
		//scrollWidget->setStyleSheet("background-color: rgba(0,0,0,0)");
		scrollWidget->setStyleSheet("background-color: rgba(30,30,130,150)");
		scrollWidget->setGeometry(0, 0, 300, 100);

		scrollArea->setWidget(scrollWidget);
	}

	void NodePanel::onConfigData(QMap<QString, QVariant*> config)
	{

		colorNormal = config.value("color_shem_Normal")->value<QColor>();
		colorOver = config.value("color_shem_Over")->value<QColor>();
		colorActive = config.value("color_shem_Active")->value<QColor>();
		fontTextSmall = config.value("font_shem_TextSmall")->value<QFont>();


		Panel::onConfigData(config);

		buttAddNode->setColorNormal(colorNormal);
		buttAddNode->setColorOver(colorOver);
		buttAddNode->setColorActive(colorActive);
		buttAddNode->setFont(fontTextSmall);

		configData = config;
		emit actConfigData(config);

		update();
	}

	void NodePanel::addNode()
	{
		qDebug("NodePanel::addNode()");

		//Node *newNode = new Node(scrollWidget);
		Node *newNode = new Node(scrollWidget);
		newNode->setName(tr("test%1").arg(QString::number(dictNode.values().size())));
		newNode->setSourceCode("SinOsc.ar(\\freq.kr(90)!2, mul: Saw.kr(2, 0.5, 0.5))");
		newNode->connectBridge(mBridge);
		newNode->setFixedWidth(scrollArea->width() - 10);
		//newNode->setFixedHeight(150);
		newNode->show();

		//connect(this, SIGNAL(resizeAct()), newNode, SLOT(fitGeometry()));
		connect(newNode, SIGNAL(killAct(QString)), this, SLOT(deleteNode(QString)));
		connect(this, SIGNAL(actConfigData(QMap<QString, QVariant*>)),
			newNode, SLOT(onConfigData(QMap<QString, QVariant*>)));

		connect(newNode, SIGNAL(actChangedHeight()), this, SLOT(fitNodesPosition()));

		dictNode.insert(newNode->name(), newNode);

		this->fitNodesPosition();


		emit actConfigData(configData);

		//scrollArea->ensureWidgetVisible(newNode, 50, 50);


	}

	void NodePanel::deleteNode(QString nodeName)
	{
		dictNode.remove(nodeName);
		//emit resizeAct();
		this->fitNodesPosition();
	}

	void NodePanel::fitNodesPosition()
	{
		int nextNodeOriginY = 0;
		for each (Node *oneNode in dictNode.values())
		{
			qDebug() << "NodePanel::NEW LOOP";
			oneNode->setGeometry(5, nextNodeOriginY, oneNode->width(), oneNode->height());
			qDebug() << "NodePanel::fitNodesPosition - moveY: " << nextNodeOriginY;
			nextNodeOriginY += nextNodeOriginY + oneNode->height() + 10;
			qDebug() << "NodePanel::fitNodesPosition - endLoopY: " << nextNodeOriginY;
			qDebug() << "NodePanel::fitNodesPosition - origin: " << oneNode->geometry().topLeft();
		}
		scrollWidget->setFixedHeight(nextNodeOriginY);
		qDebug() << "NodePanel::fitNodesPosition - nextNodeOriginY: " << nextNodeOriginY;
	}

	void NodePanel::resizeEvent(QResizeEvent *event)
	{
		Panel::resizeEvent(event); // send event to superclass
		
		scrollArea->resize(width() - 10, height() - 100);
		scrollWidget->setFixedWidth(scrollArea->width());

		for each (Node *oneNode in dictNode.values())
		{
			oneNode->setFixedWidth(scrollArea->width() - 10);
		}

		//qDebug("NodePanel::resizeEvent");

	}
	void NodePanel::paintEvent(QPaintEvent *event)
	{
		Panel::paintEvent(event); // send event to superclass

		QPainter painter(this);

		//painter.fillRect(scrollWidget->geometry(), QColor(120, 30, 30, 60));

	}

	NodePanel::~NodePanel()
	{

	}
}