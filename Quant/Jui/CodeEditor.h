#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "Panel.h"
#include <QWidget>
#include <QTextEdit>
#include <QList>
#include <QDebug>

namespace Jui
{
	enum class HighLights {CLASS, CONTROL, NODE, DIGIT};

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

		void classesAct(QStringList);
		void symbolsAct(QStringList);
		void floatsAct(QStringList);

	protected:
		virtual bool eventFilter(QObject * watched, QEvent * event);
		void paintEvent(QPaintEvent *paintEvent);

	private:

		int fadeTimeOut;
		int fps;
		int backgroundAlpha;
		QTimer *timer;

		QColor normalColor, overColor, activeColor;

		QStringList regexpText(const QString &, QString regexpRule);
		QStringList regexpText2(HighLights type);
		void highlightText(QString, QColor);

		bool prEdit;
	};

}
#endif // CONSOLE_H
