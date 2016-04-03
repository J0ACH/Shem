#ifndef CONSOLE_H
#define CONSOLE_H

#include "Panel.h"
#include <QWidget>
#include <QTextEdit>

namespace Jui
{
	class Console : public Panel
	{
		Q_OBJECT

	public:
		Console(QWidget *parent);
		~Console();

	public slots:
		void addLine(QString newLine);
		void fitTextSize();

	private:
		QTextEdit *text;
	};

}
#endif // CONSOLE_H
