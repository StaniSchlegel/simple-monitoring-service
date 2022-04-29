#include "memory_sensor.hpp"

std::string MemorySensor::generateJSONOutput(){

    std::string availableMem = "0";
    std::ifstream memInfoFile("/proc/meminfo");

    if (memInfoFile.is_open()) {

        std::string line;

        while (std::getline(memInfoFile, line)) {
            if(line.find("MemAvailable") == 0){
                auto last_whitespace = line.find_last_of(" ");
                availableMem = line.substr(17, last_whitespace - 17);
                memInfoFile.close();
                break;
            }
        }       

        memInfoFile.close();
    }

    struct sysinfo memInfo;

    sysinfo (&memInfo);

    auto total_ram = ((uint64_t) memInfo.totalram * memInfo.mem_unit)/1024;
    auto free_swap = ((uint64_t) memInfo.freeswap * memInfo.mem_unit)/1024;
    auto total_swap = ((uint64_t) memInfo.totalswap * memInfo.mem_unit)/1024;

    std::string jsonOutput = "{\"total memory\": " + std::to_string(total_ram) + 
                             ",\"available memory\": " + availableMem + 
                             ",\"total swap\": " + std::to_string(total_swap) +
                             ",\"free swap\": "  + std::to_string(free_swap) + "}";

    return jsonOutput;
}