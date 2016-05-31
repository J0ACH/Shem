#include "Edges.h"

namespace Jui
{

  Edges::Edges(QWidget *parentObject) : QObject(parentObject)
  {
    parent = parentObject;
    //qDebug("Edges init...");

    setObjectName("Edges");

    /*
    qDebug("Class(%s) -> parent(%s)",
    qPrintable(objectName()),
    qPrintable(parent->objectName())
    );
    */

    parent->installEventFilter(this);

    if (parent->objectName() == "Canvan")
    {
      this->parentType = WIN;
    }
    else
    {
      this->parentType = WIDGET;
    };

    this->setEdgeOffset(10);

    //this->addManipulator(EdgeControler::LEFT);
    //this->addManipulator(EdgeControler::TOP);
    //this->addManipulator(EdgeControler::RIGHT);
    //this->addManipulator(EdgeControler::BOTTOM);
  }

  void Edges::addManipulator(EdgeControler::Direction direction)
  {
    EdgeControler *manipulator = new EdgeControler(parent);
    manipulator->setParentObject(this);
    manipulator->setDirection(direction);
    manipulator->setEdgeOffset(edgeOffset);

    connect(parent, SIGNAL(resizeAct()), manipulator, SLOT(fitGeometry()));

    conteiner.append(manipulator);
  }

  void Edges::removeManipulator(EdgeControler::Direction direction)
  {

  }

  void Edges::hide()
  {
    foreach(EdgeControler *oneCnt, conteiner)  { oneCnt->isActive = false; }
  }
  void Edges::show()
  {
    foreach(EdgeControler *oneCnt, conteiner)  { oneCnt->isActive = true; }
  }

  void Edges::setEdgeOffset(int offset) { edgeOffset = offset; }

  void Edges::edgePressed()
  {
    mEdgePressActGrandParentOriginGlobal = parent->parentWidget()->mapToGlobal(QPoint(0, 0));
  }

  void Edges::edgeMoved(EdgeControler::Direction direction, int newValue)
  {
    QRect originalGeometry = parent->geometry();

    switch (direction)
    {
    case EdgeControler::LEFT:
      switch (parentType)
      {
      case WIDGET:
        originalGeometry.setLeft(newValue - mEdgePressActGrandParentOriginGlobal.x());
        parent->setGeometry(originalGeometry);
        break;
      case WIN:
        QRect winRect = parent->parentWidget()->geometry();
        winRect.setLeft(newValue);
        parent->parentWidget()->setGeometry(winRect);
        parent->setGeometry(0, 0, winRect.width(), winRect.height());
        break;
      }
      break;

    case EdgeControler::TOP:
      switch (parentType)
      {
      case WIDGET:
        originalGeometry.setTop(newValue - mEdgePressActGrandParentOriginGlobal.y());
        parent->setGeometry(originalGeometry);
        break;
      case WIN:
        QRect winRect = parent->parentWidget()->geometry();
        winRect.setTop(newValue);
        parent->parentWidget()->setGeometry(winRect);
        parent->setGeometry(0, 0, winRect.width(), winRect.height());
        break;
      }
      break;

    case EdgeControler::RIGHT:
      switch (parentType)
      {
      case WIDGET:
        originalGeometry.setRight(newValue - mEdgePressActGrandParentOriginGlobal.x());
        parent->setGeometry(originalGeometry);
        break;
      case WIN:
        QRect winRect = parent->parentWidget()->geometry();
        winRect.setRight(newValue);
        parent->parentWidget()->setGeometry(winRect);
        parent->setGeometry(0, 0, winRect.width(), winRect.height());
        break;
      }
      break;

    case EdgeControler::BOTTOM:
      switch (parentType)
      {
      case WIDGET:
        originalGeometry.setBottom(newValue - mEdgePressActGrandParentOriginGlobal.y());
        parent->setGeometry(originalGeometry);
        break;
      case WIN:
        QRect winRect = parent->parentWidget()->geometry();
        winRect.setBottom(newValue);
        parent->parentWidget()->setGeometry(winRect);
        parent->setGeometry(0, 0, winRect.width(), winRect.height());
        break;
      }
      break;

    default:
      break;
    }

  }

  Edges::~Edges()
  {

  }
}