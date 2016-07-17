#ifndef CUSTOMIZEPANEL_H
#define CUSTOMIZEPANEL_H

#include "Customize.h"
#include "Button.h"
#include "Panel.h"
#include "ControlBox.h"

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
   // void onCustomize(Customize*);
    void onCustomize(Data);
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
    Button *buttonSave;

    ControlBox *boxName;
    ControlBox *boxColorBackground;

  };
}

#endif // CUSTOMIZEPANEL_H

