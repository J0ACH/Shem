#ifndef NODEPANEL_H
#define NODEPANEL_H

#include "Panel.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPainter>

using namespace Jui;

namespace QuantIDE
{

	class NodePanel : public Panel
	{
		Q_OBJECT

	public:
		NodePanel(QWidget *parent = 0);
		~NodePanel();

		QRect bounds();

	protected:
		//void paintEvent(QPaintEvent *event);

	private:
		void initControl();

		QPushButton *testButton;
		QLabel *nameLabel;
	};
}

#endif // QUANT

