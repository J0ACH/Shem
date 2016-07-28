#ifndef NETWORKPANEL_H
#define NETWORKPANEL_H

#include "Customize.h"
#include "Button.h"
#include "Panel.h"
#include "ControlBox.h"
#include "Text.h"
#include "QuantObjects.h"

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QScrollArea>


using namespace Jui;
using namespace SupercolliderBridge;

namespace QuantIDE
{
  class NetworkPanel : public PanelNEW
  {
    Q_OBJECT

  public:
    NetworkPanel(QWidget *parent, QMap<QString, QuantUser*> *lib);
    ~NetworkPanel();

    void updateProfilesPosition();
    
    protected:
    void resizeEvent(QResizeEvent *event);

  private:
    void initControl();

    QScrollArea *scrollArea;
    QWidget *scrollWidget;

    QMap<QString, QuantUser*> *mProfiles;
  };
}

#endif // NETWORKPANEL_H

