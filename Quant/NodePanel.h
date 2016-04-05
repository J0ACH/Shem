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

	public slots:
		void fitGeometry();

	private:
		void initControl();
		
		Node *testNode1, *testNode2;
	};
}

#endif // QUANT

