


#include <QWidget>
#include <QPainter>
#include <QList>
//#include <list>

namespace Jui
{
	
	enum EdgePosition
	{
		LEFT,
		TOP,
		RIGHT,
		BOTTOM,
		ALL
	};
	
	class Edges : public QWidget
	{
		Q_OBJECT

	public:
		Edges(QWidget *parent);
		~Edges();

		//EdgeControler getAhoj(void);

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		//EdgeControler *ahoj;
		//QWidget *parent;		
	};

	class EdgeControler : public QWidget
	{
		Q_OBJECT

	public:
		EdgeControler(QWidget *parent);
		~EdgeControler();

	protected:
		void paintEvent(QPaintEvent *event);

	//private:
		//QWidget *parent;

	};

}



