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
    void onCustomize(DataCustomize);
    void onChangePressed();
    void onSavePressed();

  signals:
    void actChangeConfirmed(DataCustomize);
    void actSaveConfirmed(DataCustomize);
    void actUserNameChanged(DataCustomize);

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    void initControl();
    QString color2String(QColor);
    QString font2String(QFont);
    QColor string2Color(QString);
    QFont string2Font(QString);

    QScrollArea *scrollArea;
    QWidget *scrollWidget;

    Text *textApp, *textColor, *textFont;
    Text *subTextCanvan, *subTextPanels, *subTextMsg;
    ControlBox *boxName, *boxBootNetwork, *boxBootInterpret, *boxBootServer;
    ControlBox *boxColorNormal, *boxColorOver, *boxColorActive, *boxColorText;
    ControlBox *boxColorAppHeader, *boxColorAppBackground;
    ControlBox *boxColorPanelHeader, *boxColorPanelBackground;
    ControlBox *boxFontAntialias, *boxFontBig, *boxFontSmall, *boxFontConsole, *boxFontCode;
    // ControlBox *boxMsgNormal, *boxMsgStatus, *boxMsgEvaluate, *boxMsgAnswer, *boxMsgError, *boxMsgWarninig, *boxMsgBundle;
    Button *buttonSave;

    DataCustomize customizeData;
  };
}

#endif // CUSTOMIZEPANEL_H
