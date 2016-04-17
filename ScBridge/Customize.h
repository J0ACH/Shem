#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H

#include "ScBridge.h"

#include <QWidget>
#include <QMap>
#include <QDebug>


namespace SupercolliderBridge
{

	class Customize : public QObject
	{
		Q_OBJECT

	public:
		Customize(QObject *parent = 0);
		~Customize();

		void setTargetBridge(ScBridge*);

	public slots:
		void onInterpretStart();

		
	private:
		ScBridge *mBridge;
		QMap<QString, QVariant*> dictNode;
		
	};
}

#endif // QUANT

