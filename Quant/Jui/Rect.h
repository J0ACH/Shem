#include <QRect>
#include <QWidget>

namespace Jui
{

	class Rect
	{
	public:
		Rect();
		Rect(QWidget *);
		virtual ~Rect();

		QRect offset(int, int, int, int);

	private:
		QWidget *parent;
	};



}