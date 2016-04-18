#ifndef CONSOLE_H
#define CONSOLE_H

#include "Panel.h"
#include <QWidget>
#include <QTextEdit>
#include <QScrollBar>

namespace Jui
{
	class Console : public Panel
	{
		Q_OBJECT

	public:
		Console(QWidget *parent);
		~Console();

		void setFont(QFont);
		
	public slots:
		void addText(QString text, QColor color, bool newLine);
		void fitGeometry();

	private:
		QTextEdit *text;
	};

}
#endif // CONSOLE_H
