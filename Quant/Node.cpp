#include "Node.h"

namespace QuantIDE
{
	Node::Node(QWidget *parent) :
		QWidget(parent),
		mBridge(NULL)
	{
		setObjectName("Node");
		objectPattern = QString::null;

		this->initControl();
		this->fitGeometry();

		stateNodePlay = StateNodePlay::FREE;

		connect(playButton, SIGNAL(pressAct()), this, SLOT(changeNodePlay()));
		connect(closeButton, SIGNAL(pressAct()), this, SLOT(close()));

		connect(sourceCode, SIGNAL(sendText(QString)), this, SLOT(onReciveText(QString)));
	}

	QRect Node::bounds() { return QRect(0, 0, width() - 1, height() - 1); }

	void Node::initControl()
	{
		nameLabel = new QLabel(this);
		nameLabel->setText("node1");
		
		closeButton = new Button(this);
		closeButton->setGeometry(this->width() - 30, 10, 16, 16);
		closeButton->setIcon(QImage(":/smallClose16.png"), 0);
		closeButton->setText("X");

		playButton = new Button(this);
		playButton->setText("play");
		playButton->setStateKeeping(Jui::Button::StateKeeping::HOLD);

		sourceCode = new CodeEditor(this);

		labelNodeID = new QLabel(this);		
		labelNamedControls = new QLabel(this);		
	}

	void Node::onConfigData(QMap<QString, QVariant*> config)
	{
		colorPanelBackground = config.value("shem_colorPanelBackground")->value<QColor>();
		colorNormal = config.value("shem_colorNormal")->value<QColor>();
		colorOver = config.value("shem_colorOver")->value<QColor>();
		colorActive = config.value("shem_colorActive")->value<QColor>();
		fontTextBig = config.value("shem_fontTextBig")->value<QFont>();
		fontTextSmall = config.value("shem_fontTextSmall")->value<QFont>();

		closeButton->setColorNormal(colorNormal);
		closeButton->setColorOver(colorOver);
		closeButton->setColorActive(colorActive);

		playButton->setColorNormal(colorNormal);
		playButton->setColorOver(colorOver);
		playButton->setColorActive(colorActive);

		nameLabel->setFont(fontTextBig);
		labelNodeID->setFont(fontTextSmall);
		labelNamedControls->setFont(fontTextSmall);

		configData = config;
		//emit actConfigData(config);

		update();
	}

	void Node::connectBridge(ScBridge *bridge)
	{
		mBridge = bridge;
		connect(this, SIGNAL(evaluateAct(QString)), mBridge, SLOT(evaluateCode(QString)));
		connect(this, SIGNAL(bridgeQuestionAct(QString, int, QString, bool)), mBridge, SLOT(question(QString, int, QString, bool)));
		connect(mBridge, SIGNAL(answerAct(QString, int, QStringList)), this, SLOT(onBridgeAnswer(QString, int, QStringList)));

		stateNodePlay = StateNodePlay::STOP;
		QString code;
		code = tr("~%1 = NodeProxy.audio(s, 2)").arg(name());
		emit evaluateAct(code);

		code = tr("~%1[0] = {%2}").arg(name(), sourceCode->toPlainText());
		emit evaluateAct(code);

		onBridgeQuestion(QuestionType::nodeID);
		onBridgeQuestion(QuestionType::namedControls);

	}

	void Node::setName(QString name)
	{
		nameLabel->setText(name);
		objectPattern = tr("%1/%2").arg(this->objectName(), name);
	}
	QString Node::name() { return nameLabel->text(); }

	void Node::setSourceCode(QString code) { sourceCode->setText(code); }

	void Node::onReciveText(QString text)
	{
		QString code;
		code = tr("~%1 = NodeProxy.audio(s, 2)").arg(name());
		emit evaluateAct(code);

		code = tr("~%1[0] = {%2}").arg(name(), text);
		emit evaluateAct(code);

		onBridgeQuestion(QuestionType::namedControls);
	}

	void Node::onEvaluateNode()
	{
		QString code;
		code = tr("~%1 = NodeProxy.audio(s, 2);").arg(name());
		code += tr("~%1[0] = {%2};").arg(name(), sourceCode->toPlainText());

		for (int i = 0; i < conteinerControls.size(); i++)
		{
			QString key = conteinerControls.keys().at(i);
			QString value = conteinerControls[key]->toPlainText();
			code += tr("~%1.set('%2', %3);").arg(name(), key, value);
		}

		emit evaluateAct(code);
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
			emit bridgeQuestionAct(objectPattern, selectorNum, questionCode, true);
			break;
		case namedControls:
			selectorNum = QuestionType::namedControls;
			questionCode = tr("~%1.controlKeys").arg(name());
			emit bridgeQuestionAct(objectPattern, selectorNum, questionCode, true);
			break;
		case namedValues:
			selectorNum = QuestionType::namedValues;
			questionCode = tr("~%1.controlKeysValues (['%2'])").arg(name(), args);
			emit bridgeQuestionAct(objectPattern, selectorNum, questionCode, true);
			break;
		}
	}

	void Node::onBridgeAnswer(QString pattern, int selectorNum, QStringList answer)
	{
		qDebug() << "Node::onBridgeAnswer: " << pattern << answer;
		if (pattern == objectPattern)
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
				conteinerControls[answer[0]]->setText(answer[1]);
				break;

			default:
				qDebug() << "Node::onBridgeAnswer::DEFAULT";
				break;
			}
		}
	}

	void Node::initControlsEditor(QStringList namedControls)
	{
		QStringList existKeys = conteinerControls.keys();
		foreach(QString key, existKeys)
		{
			if (!namedControls.contains(key)) { this->removeControl(key); }
		}

		foreach(QString key, namedControls)
		{
			if (!conteinerControls.contains(key)) { this->addControl(key); }
		}
	}

	void Node::addControl(QString controlName)
	{
		CodeEditor *controlEditor = new CodeEditor(this);
		controlEditor->show();

		QLabel *controlLabel = new QLabel(this);
		controlLabel->setText(controlName);
		controlLabel->setFont(fontTextSmall);
		controlLabel->show();

		conteinerControls.insert(controlName, controlEditor);
		conteinerControlsLabel.insert(controlName, controlLabel);

		connect(controlEditor, SIGNAL(evaluateAct()), this, SLOT(onEvaluateNode()));

		onBridgeQuestion(QuestionType::namedValues, controlName);

		this->fitGeometry();
	}
	void Node::removeControl(QString controlName)
	{
		CodeEditor *removeEditor = conteinerControls.take(controlName);
		removeEditor->close();

		QLabel *removeLabel = conteinerControlsLabel.take(controlName);
		removeLabel->close();

		this->fitGeometry();
	}

	void Node::changeNodePlay()
	{
		QString code;
		switch (stateNodePlay)
		{
		case QuantIDE::StateNodePlay::PLAY:
			code = tr("~%1.stop(4)").arg(name());
			stateNodePlay = StateNodePlay::STOP;
			emit evaluateAct(code);
			break;
		case QuantIDE::StateNodePlay::STOP:
			code = tr("~%1.play(vol: 1, fadeTime: 4);").arg(name());
			stateNodePlay = StateNodePlay::PLAY;
			emit evaluateAct(code);
			break;
		case QuantIDE::StateNodePlay::FREE:
			code = tr("~%1.play").arg(name());
			stateNodePlay = StateNodePlay::PLAY;
			emit evaluateAct(code);
			break;
		}

		onBridgeQuestion(QuestionType::nodeID);
		onBridgeQuestion(QuestionType::namedControls);
	}

	void Node::fitGeometry()
	{
		nameLabel->setGeometry(10, 5, 80, 30);
		closeButton->setGeometry(this->width() - 30, 10, 16, 16);
		playButton->setGeometry(90, 10, 40, 20);
		sourceCode->setGeometry(10, 45, width() - 20, 60);

		labelNodeID->setGeometry(this->width() - 200, this->height() - 45, 180, 20);
		labelNamedControls->setGeometry(this->width() - 200, this->height() - 25, 180, 20);

		int controlID = 0;
		foreach(CodeEditor *oneControler, conteinerControls)
		{
			oneControler->setGeometry(60, 80 + 35 * (controlID + 1), 200, 25);
			controlID++;
		}
		controlID = 0;
		foreach(QLabel *oneLabel, conteinerControlsLabel)
		{
			oneLabel->setGeometry(15, 80 + 35 * (controlID + 1), 40, 25);
			controlID++;
		}
	}

	void Node::paintEvent(QPaintEvent *paintEvent)
	{
		QPainter painter(this);
		painter.fillRect(this->bounds(), colorPanelBackground);

		painter.setPen(colorOver);
		painter.drawLine(0, 0, width(), 0);
		painter.drawLine(0, height() - 1, width(), height() - 1);
	}

	void Node::closeEvent(QCloseEvent *event)
	{
		QString code = tr("~%1.free;").arg(name());
		stateNodePlay = StateNodePlay::FREE;
		emit evaluateAct(code);
		emit killAct(name());
		qDebug("Node::closeEvent()");
	}

	Node::~Node()
	{

	}
}