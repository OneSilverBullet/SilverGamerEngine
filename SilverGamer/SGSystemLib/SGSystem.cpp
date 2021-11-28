#include "SGSystem.h"
#include <fstream>

void SilverEngineLib::SGSystem::SystemOutput()
{
	std::cout << "DLL Output" << std::endl;
}

bool SilverEngineLib::SGSystem::IsFileExist(std::string fileDir)
{
    std::fstream fileStream;
    fileStream.open(fileDir.c_str(), std::ios::in);
    if (!fileStream)
    {
        return false;
    }
    return true;
}
