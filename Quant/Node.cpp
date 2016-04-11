#include "Node.h"

namespace QuantIDE
{
	Node::Node(QWidget *parent) : QWidget(parent)
	{
		setObjectName("Node");

		this->initControl();
		this->fitGeometry();

		stateNodePlay = StateNodePlay::FREE;

		connect(sourceCode, SIGNAL(sendText(QString)), this, SLOT(reciveText(QString)));
		connect(buttNodePlay, SIGNAL(pressAct()), this, SLOT(changeNodePlay()));
		connect(closeButton, SIGNAL(pressAct()), this, SLOT(close()));
	}

	QRect Node::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void Node::initControl()
	{
		nameLabel = new QLabel(this);
		nameLabel->setText("node1");

		closeButton = new Button(this);
		closeButton->setGeometry(this->width() - 30, 10, 16, 16);
		closeButton->setIcon(QImage(":/smallClose16.png"), 0);
		closeButton->setText("X");

		buttNodePlay = new Button(this);
		buttNodePlay->setText("play");
		buttNodePlay->setStateKeeping(Jui::Button::StateKeeping::HOLD);

		sourceCode = new CodeEditor(this);
	}

	void Node::setName(QString name) { nameLabel->setText(name); }
	QString Node::name() { return nameLabel->text(); }

	void Node::setSourceCode(QString code) { sourceCode->setText(code); }

	void Node::reciveText(QString text)
	{
		QString code;
		code = tr("~%1 = NodeProxy.audio(s, 2)").arg(name());
		emit evaluateAct(code);

		code = tr("~%1[0] = {%2}").arg(name(), text);
		emit evaluateAct(code);
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
	}

	void Node::fitGeometry()
	{
		nameLabel->setGeometry(10, 5, 80, 30);
		closeButton->setGeometry(this->width() - 30, 10, 16, 16);
		buttNodePlay->setGeometry(90, 10, 40, 20);
		sourceCode->setGeometry(10, 45, width() - 20, 80);
	}

	void Node::paintEvent(QPaintEvent *paintEvent)
	{
		QPainter painter(this);
		painter.fillRect(this->bounds(), QColor(30, 30, 30));

		painter.setPen(QColor(230, 230, 230));
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