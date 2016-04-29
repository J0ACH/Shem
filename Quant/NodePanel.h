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

		void setTargetBridge(ScBridge*);

		public slots:
		void fitNodesPosition();
		void onConfigData(QMap<QString, QVariant*> config);

		void addNode();
		void deleteNode(QString name);

	signals:
		void actConfigData(QMap<QString, QVariant*> config);

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

	private:
		void initControl();
		QMap<QString, QVariant*> configData;
		QColor colorAppHeaderBackground, colorPanelBackground, colorNormal, colorOver, colorActive, colorText;
		QFont fontTextSmall;
		Button *buttAddNode;
		QScrollArea *scrollArea;
		QWidget *scrollWidget;
		QMap<QString, Node*> dictNode;
		ScBridge *mBridge;

	};
}

#endif // QUANT

