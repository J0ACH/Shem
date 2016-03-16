
#include <QtWidgets>

#include "Console.h"
#include "Editor.h"
#include "ScBridge.h"

using namespace SupercolliderBridge;

int main(int argv, char **args) {
  QApplication app(argv, args);

  CodeEditor editor;

  editor.setWindowTitle(QObject::tr("Shem Editor"));
  editor.show();

  return app.exec();
}
