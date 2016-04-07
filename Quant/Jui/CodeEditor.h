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

		void setBackground(const QColor &);

	public slots:
		void fitTextFormat();
		void alphaUpdate();

	signals:
		void sendText(QString);

	protected:
		virtual bool eventFilter(QObject * watched, QEvent * event);
		void paintEvent(QPaintEvent *paintEvent);

	private:
		void highlightText(const QString &);

		int fadeTimeOut;
		int fps;
		int backgroundAlpha;
		QTimer *timer;
		
		QColor normalColor, overColor, activeColor;
	};

}
#endif // CONSOLE_H
