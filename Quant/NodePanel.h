#ifndef NODEPANEL_H
#define NODEPANEL_H

#include "Button.h"
#include "Panel.h"
#include "Node.h"
#include "Canvan.h"
#include "ScBridge.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPainter>

using namespace Jui;
using namespace SupercolliderBridge;

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
		Node *testNode1, *testNode2;

		void setTargetCanvan(Canvan*);
		void setTargetBridge(ScBridge*);

	public slots:
		void fitGeometry();

	private:
		void initControl();
	};
}

#endif // QUANT

