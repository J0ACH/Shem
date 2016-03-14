#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include <QPainter>

namespace Jui
{
	class Panel : public QWidget
	{
		Q_OBJECT

	public:
		Panel(QWidget *parent = 0);


	public slots:
		void draw();

	protected:
		//void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

	private:

	};



}

#endif // PANEL