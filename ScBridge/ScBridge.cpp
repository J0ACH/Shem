#include "ScBridge.h"
#include <SC_LanguageClient.h>



ScBridge::ScBridge()
{
    SC_LanguageClient * client = createLanguageClient("sclang");

	client->runMain();
	client->setCmdLine("s.boot;");

}

ScBridge::~ScBridge()
{

}

