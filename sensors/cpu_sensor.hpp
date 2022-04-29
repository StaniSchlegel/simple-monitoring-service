#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class CpuSensor{
public:
	static unsigned long getOverallCpuUsage();
	static unsigned long getCpuCoresUsages();
        	
	static std::string generateJSONOutput();
};
