#ifndef CUSTOMIZEPANEL_H
#define CUSTOMIZEPANEL_H

#include "Customize.h"
#include "Button.h"
#include "Panel.h"
#include "ControlBox.h"
#include "Text.h"

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QScrollArea>


using namespace Jui;
using namespace SupercolliderBridge;

namespace QuantIDE
{

  class CustomizePanel : public PanelNEW
  {
    Q_OBJECT

  public:
    CustomizePanel(QWidget *parent = 0);
    ~CustomizePanel();

    public slots:
    void onChangePressed();
    void onSavePressed();

  signals:
    void actChangeConfirmed(Data);
    void actSaveConfirmed(Data);

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    void initControl();
    QString color2String(QColor);
    QColor string2Color(QString);
    Data makeData();

    QScrollArea *scrollArea;
    QWidget *scrollWidget;

    Text *textApp, *textColor, *textFont;
    Text *subTextCanvan, *subTextPanels, *subTextMsg;
    ControlBox *boxName;
    ControlBox *boxColorNormal, *boxColorOver, *boxColorActive, *boxColorText;
    ControlBox *boxColorPanelBackground;
    Button *buttonSave;
  };
}

#endif // CUSTOMIZEPANEL_H

