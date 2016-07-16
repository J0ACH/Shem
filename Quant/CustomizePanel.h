#ifndef CUSTOMIZEPANEL_H
#define CUSTOMIZEPANEL_H

#include "Customize.h"
#include "Button.h"
#include "Panel.h"
#include "ControlBox.h"

#include <QWidget>
#include <QPainter>
#include <QMap>
#include <QDebug>
#include <QScrollArea>


using namespace Jui;
using namespace SupercolliderBridge;

namespace QuantIDE
{

  class CustomziePanel : public PanelNEW
  {
    Q_OBJECT

  public:
    CustomziePanel(QWidget *parent = 0);
    ~CustomziePanel();

    public slots:
    void onCustomize(Customize*);
    void onData(Data);

  signals:
    void actChanged();

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    void initControl();
    QString color2String(QColor);
    QColor string2Color(QString);

    Button *buttonConfirm;

    ControlBox *boxName;
    ControlBox *boxColorBackground;

  };
}

#endif // CUSTOMIZEPANEL_H

