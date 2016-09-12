#ifndef QUANTPANELS_H
#define QUANTPANELS_H

#include "Panel.h"
#include "Text.h"
#include "ListWidget.h"
#include "QuantObjects.h"

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QScrollArea>

using namespace Jui;

namespace QuantIDE
{
  class QuantPanel : public Panel
  {
    Q_OBJECT

  public:
    QuantPanel(QWidget *parent = 0);
    ~QuantPanel();

    void setScrollWidget(QWidget*);
    QWidget* getScrollWidget();

    void setScrollOffset(int);

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    QScrollArea *scrollArea;
    int scrollOffset;
  };

  // NodePanel //////////////////////////////////////////////////////

  class NodePanelNEW : public QuantPanel
  {
    Q_OBJECT

  public:
    NodePanelNEW(QWidget *parent = 0);
    ~NodePanelNEW();

    public slots:
    void onAddNodePressed();

  signals:
    void actAddNode(QString, bool);

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    Button *buttonAddNode;
  };

  // SnippedPanel //////////////////////////////////////////////////////

  class SnippedPanel_Item : public QWidget
  {
    Q_OBJECT

  public:
    SnippedPanel_Item(QWidget *parent = 0);
    ~SnippedPanel_Item();


  protected:
    bool eventFilter(QObject *watched, QEvent * event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

  private:
    ControlBox *snippetBox, *codeBox;
    CodeEditor *codeEditor;

    private slots:
    void onPreviousFocused(QWidget*);
    void onNextFocused(QWidget*);
    void onParentFocused(QWidget*);
    void onCodeHeightChanged();
  };


  class SnippetPanel : public Panel
  {
    Q_OBJECT

  public:
    SnippetPanel(QWidget *parent = 0);
    ~SnippetPanel();

  protected:
    void resizeEvent(QResizeEvent *event);
    
  private:
    ListWidget *listWidget;
    Button *buttonSave;
  };
}

#endif // QUANTPANELS

