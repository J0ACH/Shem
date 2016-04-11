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
#include <QMap>
#include <QDebug>
#include <QScrollArea>


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

		//Node *testNode1, *testNode2;

		void setTargetBridge(ScBridge*);

	public slots:
		void fitGeometry();

		void addNode();
		void deleteNode(QString name);

	private:
		void initControl();
		Button *buttAddNode;
		QScrollArea *scrollArea;
		QWidget *scrollWidget;
		QMap<QString, Node*> dictNode;
		ScBridge *mBridge;
		int testID;
	};
}

#endif // QUANT

