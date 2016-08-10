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
    Button *buttonSave;

    DataCustomize customizeData;
         };
}

#endif // CUSTOMIZEPANEL_H

/*
dataCustomize.setValue(DataCustomize::COLOR_MSG_NORMAL, QColor(70, 70, 70));
dataCustomize.setValue(DataCustomize::COLOR_MSG_STATUS, QColor(230, 230, 230));
dataCustomize.setValue(DataCustomize::COLOR_MSG_EVALUATE, QColor(170, 230, 230));
dataCustomize.setValue(DataCustomize::COLOR_MSG_ANSWER, QColor(170, 200, 160));
dataCustomize.setValue(DataCustomize::COLOR_MSG_ERROR, QColor(230, 30, 30));
dataCustomize.setValue(DataCustomize::COLOR_MSG_WARNINIG, QColor(230, 130, 30));
dataCustomize.setValue(DataCustomize::COLOR_MSG_BUNDLE, QColor(170, 160, 20));

*/