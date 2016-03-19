#include "Panel.h"

namespace Jui
{

	Panel::Panel(QWidget *parent) : QWidget(parent)
	{
		this->setParent(parent);

		title = "panel";
		backColor = QColor(120, 120, 120);

		this->initControl();

		connect(closeButton, SIGNAL(pressAct()), this, SLOT(close()));
	}

	QRect Panel::bounds()
	{
		return QRect(0, 0, width() - 1, height() - 1);
	}

	void Panel::initControl()
	{
		closeButton = new Button(this);
		//closeButton->installEventFilter(this);
		closeButton->setGeometry(this->width() - 30, 10, 20, 20);
		closeButton->setText("X");

		edges = new Edges(this);
		//edges->installEventFilter(this);
		//closeButton->installEventFilter(this);
	}

	void Panel::setTitle(QString name)
	{
		title = name;
	}

	void Panel::setBackground(QColor backgroundColor)
	{
		backColor = backgroundColor;
	}

	void Panel::paintEvent(QPaintEvent *event)
	{
		QPainter painter(this);
		painter.fillRect(bounds(), backColor);

		painter.setPen(QPen(QColor(200, 200, 200), 1));
		painter.drawRect(bounds().adjusted(5, 5, -5, -5));
		painter.drawText(15, 25, title);

		//qDebug("Panel::paintEvent");
	}

	void Panel::resizeEvent(QResizeEvent *resizeEvent)
	{
		closeButton->setGeometry(this->width() - 30, 10, 20, 20);

		edges->setGeometry(0, 0, width(), height());

		qDebug("Panel::resizeEvent");
	}

	/*
	bool Panel::eventFilter(QObject* target, QEvent* event)
	{
	if (target == this) {
	if (event->type() == QEvent::Resize) {
	closeButton->setGeometry(width() - 30, 10, 20, 20);
	//QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
	//QResizeEvent *resizeEvent = static_cast<QResizeEvent*>(event);
	qDebug() << "Panel: closeButton resized";//<< resizeEvent ->key();
	return true;
	}
	else {
	return false;
	}
	}
	else {
	// pass the event on to the parent class
	return QWidget::eventFilter(target, event);
	}
	}
	*/


	Panel::~Panel()
	{

	}
}