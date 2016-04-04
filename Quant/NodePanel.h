#ifndef NODEPANEL_H
#define NODEPANEL_H

#include "Button.h"
#include "Panel.h"
#include "Node.h"

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

		Button *buttAddNode;

	protected:
		//void paintEvent(QPaintEvent *event);

	private:
		void initControl();
		
		Node *testNode;
	};
}

#endif // QUANT

