#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "Panel.h"
#include <QWidget>
#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QDebug>

namespace Jui
{
	class CodeEditor : public QTextEdit
	{
		Q_OBJECT

	public:
		CodeEditor(QWidget *parent);
		~CodeEditor();

	public slots:
		void fitTextFormat();

	signals:
		void evaluate(QString);

	protected:
		virtual bool eventFilter(QObject * watched, QEvent * event);

	private:
		void highlightText(const QString &);
	};

}
#endif // CONSOLE_H
