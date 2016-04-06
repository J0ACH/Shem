#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "Panel.h"
#include <QWidget>
#include <QTextEdit>

namespace Jui
{
	class CodeEditor : public QTextEdit
	{
		Q_OBJECT

	public:
		CodeEditor(QWidget *parent);
		~CodeEditor();

	public slots:
	//	void addText(QString text, bool newLine);
	//	void fitGeometry();

	private:
		//QTextEdit *text;
	};

}
#endif // CONSOLE_H
