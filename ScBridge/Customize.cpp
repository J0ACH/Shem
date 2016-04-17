#include "Customize.h"

namespace SupercolliderBridge
{
	Customize::Customize(QObject *parent) :
		QObject(parent),
		mBridge(NULL)
	{
		setObjectName("Customize");				
	}

	void Customize::setTargetBridge(ScBridge *target) 
	{ 
		mBridge = target; 
		connect(mBridge, SIGNAL(interpretBootDoneAct()), this, SLOT(onInterpretStart()));
	}

	void Customize::onInterpretStart() 
	{
		qDebug() << "Customize start";
	}

	Customize::~Customize() { }
}