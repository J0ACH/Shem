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
    NetworkPanel(QWidget *parent = 0);
    ~NetworkPanel();

    void addProfile(QuantUser *profile);
    void removeProfile(QString);

    public slots:
    void onConnectPressed();

  signals:
    void actNetworkConnect();

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    void initControl();
    void updateProfilesPosition();

    QScrollArea *scrollArea;
    QWidget *scrollWidget;

    Button *buttonConnect;


    QMap<QString, QuantUser*> profiles;
    //QList<QuantUser*> profiles;
  };
}

#endif // NETWORKPANEL_H

