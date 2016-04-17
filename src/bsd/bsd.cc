#include <node.h>
#include "bsd.h"



void shadowgrid::testFunc(){
	//std::out << "i m here " << std::endl;
	printf("i m here \n");
}

void shadowgrid::ct::test(){
	printf("i m here in ct\n");	
	testFunc();
}

