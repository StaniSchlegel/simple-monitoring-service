#include<string>
#include<iostream>
#include<fstream>
#include "sys/types.h"
#include "sys/sysinfo.h"

class MemorySensor{
public:
	static unsigned long getTotalMemory();
	static unsigned long getAvailableMemory();
	static unsigned long getTotalSwapMemory();
    static unsigned long getFreeSwapMemory();

	static std::string generateJSONOutput();
};
