#ifndef QUANT_H
#define QUANT_H

#include <QWidget>
#include <QDebug>

#include "Customize.h"
#include "Canvan.h"
#include "Console.h"
#include "ScBridge.h"
#include "UDPServer.h"
#include "QuantCore.h"
#include "Panel.h"
#include "Edges.h"
#include "CodeEditor.h"
#include "CustomizePanel.h"
#include "ListWidget.h"
//#include "File.h"

#include "Node.h"

using namespace SupercolliderBridge;
using namespace Jui;

namespace QuantIDE
{
  class Quant : public QObject
  {
    Q_OBJECT

  public:
    Quant(QObject *parent);
    Quant(QObject *parent, QString userName, int sendPort, int listenPort, int appPosY);
    ~Quant();

    void initObjects();

    public slots:
    void onCustomize(DataCustomize);

  private:
    Customize *customize;
    CanvanNEW *canvanNEW;
    Console *console;
    CustomizePanel *customizePanel;
    CustomizePanelNEW *customizePanelNEW;
    SnippetPanel *snippedPanelNEW;
    Panel *snippedPanel;

    QuantCore *core;

    CanvanNEW_ToolBar *controlsBar, *panelsBar;
  };
}
#endif // QUANT

