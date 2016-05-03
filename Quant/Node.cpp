#include "Node.h"

namespace QuantIDE
{
	Node::Node(QWidget *parent, ScBridge *bridge) :
		QWidget(parent),
		mBridge(bridge)
	{
		setObjectName("Node");
		objectID = QUuid::createUuid();

		setFocusPolicy(Qt::StrongFocus);

		this->initControl();

		stateNodePlay = StateNodePlay::FREE;

		connect(this, SIGNAL(actCodeEvaluated(QString, bool, bool)), mBridge, SLOT(evaluateCode(QString, bool, bool)));
		connect(this, SIGNAL(bridgeQuestionAct(QUuid, int, QString, bool)), mBridge, SLOT(question(QUuid, int, QString, bool)));
		connect(mBridge, SIGNAL(answerAct(QUuid, int, QStringList)), this, SLOT(onBridgeAnswer(QUuid, int, QStringList)));

		connect(playButton, SIGNAL(pressAct()), this, SLOT(changeNodePlay()));
		connect(closeButton, SIGNAL(pressAct()), this, SLOT(close()));

		connect(sourceCode, SIGNAL(sendText(QString)), this, SLOT(sendSourceCode(QString)));

		this->sendInitNode();
	}

	void Node::initControl()
	{
		nameLabel = new QLabel(this);
		nameLabel->setText("node1");
		nameLabel->setGeometry(10, 5, 80, 30);

		closeButton = new Button(this);
		closeButton->setIcon(QImage(":/smallClose16.png"), 0);
		closeButton->setText("X");

		playButton = new Button(this);
		playButton->setText("play");
		playButton->setStateKeeping(Jui::Button::StateKeeping::HOLD);
		playButton->setGeometry(90, 10, 40, 20);

		sourceCode = new CodeEditor(this);

		labelNodeID = new QLabel(this);
		labelNamedControls = new QLabel(this);
	}
	void Node::onConfigData(QMap<QString, QVariant*> config)
	{
		colorPanelBackground = config.value("color_shem_PanelBackground")->value<QColor>();
		colorNormal = config.value("color_shem_Normal")->value<QColor>();
		colorOver = config.value("color_shem_Over")->value<QColor>();
		colorActive = config.value("color_shem_Active")->value<QColor>();
		fontTextBig = config.value("font_shem_TextBig")->value<QFont>();
		fontTextSmall = config.value("font_shem_TextSmall")->value<QFont>();
		fontTextCode = config.value("font_shem_TextCode")->value<QFont>();

		sourceCode->setFontCode(fontTextCode);

		closeButton->setColorNormal(colorNormal);
		closeButton->setColorOver(colorOver);
		closeButton->setColorActive(colorActive);

		playButton->setColorNormal(colorNormal);
		playButton->setColorOver(colorOver);
		playButton->setColorActive(colorActive);
		playButton->setFont(fontTextSmall);

		nameLabel->setFont(fontTextBig);
		labelNodeID->setFont(fontTextSmall);
		labelNamedControls->setFont(fontTextSmall);

		configData = config;
		//emit actConfigData(config);

		update();
	}

	QRect Node::bounds() { return QRect(0, 0, width() - 1, height() - 1); }
	void Node::setName(QString name) { nameLabel->setText(name); }
	QString Node::name() { return nameLabel->text(); }


	void Node::sendInitNode()
	{
		QString code = tr("~%1 = NodeProxy.audio(s, 2)").arg(name());
		emit actCodeEvaluated(code);

		onBridgeQuestion(QuestionType::nodeID);
	}
	void Node::sendFreeNode()
	{
		QString code = tr("~%1.free").arg(name());
		emit actCodeEvaluated(code);
	}

	void Node::setSourceCode(QString txt) 
	{
		sourceCode->setText(txt);
		this->sendSourceCode(txt);
	}
	void Node::sendSourceCode(QString txt)
	{
		QString code = tr("~%1[0] = {%2}").arg(name(), txt);
		emit actCodeEvaluated(code);

		onBridgeQuestion(QuestionType::namedControls);
	}

	void Node::sendSetNode(QString nameControl, QString txt)
	{
		QString code = tr("~%1.set('%2', %3);").arg(name(), nameControl, txt);

		/*
		for (int i = 0; i < conteinerControls.size(); i++)
		{
		QString key = conteinerControls.keys().at(i);
		QString value = conteinerControls[key]->toPlainText();
		code += tr("~%1.set('%2', %3);").arg(name(), key, value);
		}
		*/

		emit actCodeEvaluated(code);
	}

	void Node::onBridgeQuestion(QuestionType selector, QString args)
	{
		QString questionCode;
		int selectorNum;

		switch (selector)
		{
		case nodeID:
			selectorNum = QuestionType::nodeID;
			questionCode = tr("~%1.nodeID").arg(name());
			emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
			break;
		case namedControls:
			selectorNum = QuestionType::namedControls;
			questionCode = tr("~%1.controlKeys").arg(name());
			emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
			break;
		case namedValues:
			selectorNum = QuestionType::namedValues;
			questionCode = tr("~%1.controlKeysValues (['%2'])").arg(name(), args);
			emit bridgeQuestionAct(objectID, selectorNum, questionCode, false);
			break;
		}
	}
	void Node::onBridgeAnswer(QUuid id, int selectorNum, QStringList answer)
	{
		if (id == objectID)
		{
			QuestionType selector = static_cast<QuestionType>(selectorNum);
			QString txt = "";

			switch (selector)
			{
			case nodeID:
				qDebug() << "Node::onBridgeAnswer::target: " << selector;
				labelNodeID->setText(tr("nodeID: %1").arg(answer[0]));
				break;

			case namedControls:
				foreach(QString oneAnsw, answer)
				{
					txt += tr("%1; ").arg(oneAnsw);
				}
				qDebug() << "Node::onBridgeAnswer::target: " << selector;
				labelNamedControls->setText(tr("controls: %1").arg(txt));
				this->initControlsEditor(answer);
				break;

			case namedValues:
				qDebug() << "Node::onBridgeAnswer::namedValues: " << answer[1];
				//conteinerControls[answer[0]]->setText(answer[1]);

				//conteinerControlsGraph[answer[0]]->setDomainX(0, 1);
				//conteinerControlsGraph[answer[0]]->setDomainY(0, answer[1].toInt()*1.5);
				/*
				double pointFromX = 0;
				double pointFromY = answer[1].toDouble();
				double pointToX = 1;
				double pointToY = answer[1].toDouble();
				*/
				//conteinerControlsGraph[answer[0]]->addValuePoint(pointFromX, pointFromY);
				//conteinerControlsGraph[answer[0]]->addValuePoint(pointToX, pointToY);
				break;


			}
		}
	}

	void Node::initControlsEditor(QStringList namedControls)
	{
		QStringList existKeys = conteinerControlsGraph.keys();
		foreach(QString key, existKeys)
		{
			if (!namedControls.contains(key)) { this->removeControl(key); }
		}

		foreach(QString key, namedControls)
		{
			if (!conteinerControlsGraph.contains(key)) { this->addControl(key); }
		}
	}

	void Node::addControl(QString controlName)
	{
		ControlEnvelope *newGraph = new ControlEnvelope(this, mBridge, controlName);
		newGraph->setFixedHeight(250);
		newGraph->show();

		conteinerControlsGraph.insert(controlName, newGraph);

		onBridgeQuestion(QuestionType::namedValues, controlName);

		this->fitControlsPosition();
	}
	void Node::removeControl(QString controlName)
	{
		conteinerControlsGraph.value(controlName)->sendFreeBusIndex();
		conteinerControlsGraph.value(controlName)->close();
		conteinerControlsGraph.remove(controlName);

		this->fitControlsPosition();
	}

	void Node::changeNodePlay()
	{
		QString code;
		switch (stateNodePlay)
		{
		case QuantIDE::StateNodePlay::PLAY:
			code = tr("~%1.stop(4)").arg(name());
			stateNodePlay = StateNodePlay::STOP;
			//emit evaluateAct(code);
			break;
		case QuantIDE::StateNodePlay::STOP:
			code = tr("~%1.play(vol: 1, fadeTime: 4);").arg(name());
			stateNodePlay = StateNodePlay::PLAY;
			//emit evaluateAct(code);
			break;
		case QuantIDE::StateNodePlay::FREE:
			code = tr("~%1.play").arg(name());
			stateNodePlay = StateNodePlay::PLAY;
			//emit evaluateAct(code);
			break;
		}

		onBridgeQuestion(QuestionType::nodeID);
		onBridgeQuestion(QuestionType::namedControls);
	}

	void Node::fitControlsPosition()
	{
		int nextControlOriginY = 110;
		for each(ControlEnvelope *oneEnv in conteinerControlsGraph.values())
		{
			oneEnv->setGeometry(10, nextControlOriginY, oneEnv->width(), oneEnv->height());
			nextControlOriginY += oneEnv->bounds().height() + 10;
		}
		this->setFixedHeight(nextControlOriginY);
	}

	bool Node::eventFilter(QObject* target, QEvent* event)
	{
		if (event->type() == QEvent::FocusIn)
		{
			qDebug() << "QEvent::FocusIn";
			installEventFilter(this);
			update();
			return true;
		}
		if (event->type() == QEvent::FocusOut)
		{
			qDebug() << "QEvent::FocusOut";
			update();
			return true;
		}

		QWidget::eventFilter(target, event);
		return false;
	}

	void Node::resizeEvent(QResizeEvent *resizeEvent)
	{
		closeButton->setGeometry(this->width() - 30, 10, 16, 16);
		sourceCode->setGeometry(10, 45, width() - 20, 60);
		labelNodeID->setGeometry(this->width() - 200, 20, 100, 20);
		labelNamedControls->setGeometry(this->width() - 200, 5, 100, 20);
		
		for each(ControlEnvelope *oneEnv in conteinerControlsGraph.values())
		{
			oneEnv->setFixedWidth(width() - 20);
		}
		

		emit actChangedHeight();
		//qDebug("Node::resizeEvent");
	}

	void Node::paintEvent(QPaintEvent *paintEvent)
	{
		QPainter painter(this);
		painter.fillRect(this->bounds(), colorPanelBackground);
		//painter.fillRect(this->bounds(), QColor(120, 30, 30));

		if (this->hasFocus()) { painter.setPen(colorActive); }
		else { painter.setPen(colorNormal); }

		//painter.setPen(colorOver);
		painter.drawLine(0, 0, width(), 0);
		painter.drawLine(0, height() - 1, width(), height() - 1);
	}

	void Node::closeEvent(QCloseEvent *event)
	{
		this->sendFreeNode();
		stateNodePlay = StateNodePlay::FREE;
		emit killAct(name());
		qDebug("Node::closeEvent()");
	}

	Node::~Node()
	{

	}
}