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

		connect(this, SIGNAL(resizeAct()), this, SLOT(fitGeometry()));
		connect(buttAddNode, SIGNAL(pressAct()), this, SLOT(addNode()));
	}

	QRect NodePanel::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

	void NodePanel::setTargetBridge(ScBridge *target) { mBridge = target; }

	void NodePanel::initControl()
	{
		buttAddNode = new Button(this);
		buttAddNode->setText("AddNode");

		scrollArea = new QScrollArea(this);
		//scrollArea->setFrameStyle(QFrame::NoFrame);
		scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		scrollArea->setStyleSheet("background-color: rgba(0,0,0,0)");
		//scrollArea->setStyleSheet("background-color: rgba(120,120,120,255)");
		//scrollArea->setWidgetResizable(true);

		scrollWidget = new QWidget();
		scrollWidget->setAutoFillBackground(true);
		//scrollWidget->setStyleSheet("background-color: rgba(0,0,0,0)");
		scrollWidget->setStyleSheet("background-color: rgba(120,30,30,255)");
		scrollArea->setWidget(scrollWidget);

		scrollWidget->show();
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

		Node *newNode = new Node(scrollWidget);
		newNode->setName(tr("test%1").arg(QString::number(dictNode.values().size())));
		newNode->setSourceCode("SinOsc.ar(\\freq.kr(90)!2, mul: Saw.kr(2, 0.5, 0.5))");
		newNode->connectBridge(mBridge);
		newNode->show();

		connect(this, SIGNAL(resizeAct()), newNode, SLOT(fitGeometry()));
		connect(newNode, SIGNAL(killAct(QString)), this, SLOT(deleteNode(QString)));
		connect(this, SIGNAL(actConfigData(QMap<QString, QVariant*>)),
			newNode, SLOT(onConfigData(QMap<QString, QVariant*>)));

		//connect(newNode, SIGNAL(resizeAct()), this, SLOT(fitGeometry()));

		dictNode.insert(newNode->name(), newNode);

		this->fitGeometry();
		//emit resizeAct();
		emit actConfigData(configData);

		scrollArea->ensureWidgetVisible(newNode, 50, 50);

		
	}

	void NodePanel::deleteNode(QString nodeName)
	{
		dictNode.remove(nodeName);
		//emit resizeAct();
		this->fitGeometry();
	}

	void NodePanel::fitGeometry()
	{

		buttAddNode->setGeometry(200, 10, 50, 20);
		scrollArea->setGeometry(5, 50, width() - 10, height() - 100);

		int nextNodeOriginY = 0;
		for each (Node *oneNode in dictNode.values())
		{
			oneNode->setGeometry(5, nextNodeOriginY, scrollArea->width() - 10, 300);
			oneNode->update();

			nextNodeOriginY += oneNode->height() + 10;

			//qDebug() << "NodePanel::fitGeometry - nextNodeOrigin: " << oneNode->geometry();
			//scrollArea->setGeometry(5, 50, width() - 10, height() - 100);

			//qDebug() << "NodePanel::fitGeometry - widget.height: " << scrollArea->widget()->height();
			//qDebug() << "NodePanel::fitGeometry - viewport.height: " << scrollArea->viewport()->height();
			//update();
		}
	
		qDebug() << "NodePanel::fitGeometry - nextNodeOrigin: " << nextNodeOriginY;

		//scrollWidget = scrollArea->takeWidget();
		//QWidget *newScrollWidget = new QWidget();
		//newScrollWidget->setGeometry(0, 0, scrollArea->width(), nextNodeOriginY);
		//scrollArea->setWidget(newScrollWidget);

		//update();
		//emit resizeAct();

	}

	void NodePanel::resizeEvent(QResizeEvent *e)
	{
		Panel::resizeEvent(e);
		
		qDebug("NodePanel::resizeEvent");

	}
	void NodePanel::paintEvent(QPaintEvent *event)
	{
		Panel::paintEvent(event);

		QPainter painter(this);
		painter.fillRect(bounds(), QColor(120,30,30,30));
		
	}

	NodePanel::~NodePanel()
	{

	}
}