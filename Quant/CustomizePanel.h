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

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    void initControl();
    QString color2String(QColor);
    QColor string2Color(QString);
    
    QScrollArea *scrollArea;
    QWidget *scrollWidget;

    Text *textApp, *textColor, *textFont;
    Text *subTextCanvan, *subTextPanels, *subTextMsg;
    ControlBox *boxName, *boxBootNetwork, *boxBootInterpret, *boxBootServer;
    ControlBox *boxColorNormal, *boxColorOver, *boxColorActive, *boxColorText;
    ControlBox *boxColorAppHeader, *boxColorAppBackground;
    ControlBox *boxColorPanelHeader, *boxColorPanelBackground;
    Button *buttonSave;

    DataCustomize customizeData;

    bool firstOnCustomizeFillBoxes;
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

dataCustomize.setValue(DataCustomize::BOOL_TEXT_ANTIALIASING, true);
dataCustomize.setValue(DataCustomize::FONT_BIG, QFont("Univers Condensed", 13));
dataCustomize.setValue(DataCustomize::FONT_SMALL, QFont("Univers Condensed", 10));
dataCustomize.setValue(DataCustomize::FONT_CODE, QFont("Consolas", 8));
dataCustomize.setValue(DataCustomize::FONT_CONSOLE, QFont("Univers 57 Condensed", 9));

dataCustomize.setValue(DataCustomize::BOOL_BOOT_INTERPRETR, false);
dataCustomize.setValue(DataCustomize::BOOL_BOOT_SERVER, false);*/