#include "Node.h"

namespace QuantIDE
{

	Node::Node(QWidget *parent) : QWidget(parent)
	{
		setObjectName("Node");

		this->initControl();
		this->fitGeometry();

		connect(buttEvaluate, SIGNAL(pressed()), this, SLOT(evaluateCode()));
	}

	QRect Node::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void Node::initControl()
	{
		nameLabel = new QLabel(this);
		nameLabel->setText("Node1");

		buttEvaluate = new QPushButton(this);
		buttEvaluate->setText("evaluate");

		sourceCode = new QTextEdit(this);
	}

	void Node::setName(QString name)
	{
		nameLabel->setText(name);
	}

	void Node::setSourceCode(QString code)
	{
		sourceCode->setText(code);
	}

	void Node::evaluateCode()
	{
		QString name = nameLabel->text();
		QString text = sourceCode->toPlainText();
		QString code = tr("Ndef('%1', {%2}).play").arg(name, text);
		qDebug() << "EvaluateAct: " << code;
		emit evaluateAct(code);
	}

	void Node::fitGeometry()
	{
		nameLabel->setGeometry(10, 5, 80, 30);
		buttEvaluate->setGeometry(10, 35, 80, 20);
		sourceCode->setGeometry(10, 60, 300, 120);
	}

	void Node::paintEvent(QPaintEvent *paintEvent)
	{
		QPainter painter(this);
		painter.fillRect(this->bounds(), QColor(30, 30, 30));

		painter.setPen(QColor(230, 230, 230));
		painter.drawLine(0, 0, width(), 0);
		painter.drawLine(0, height() - 1, width(), height() - 1);
	}

	Node::~Node()
	{

	}
}