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
		void addText(QString text, QColor color, bool newLine);
		void fitGeometry();

	private:
		QTextEdit *text;
	};

}
#endif // CONSOLE_H
