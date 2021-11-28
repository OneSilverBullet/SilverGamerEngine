#pragma once

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <memory.h>
#include <assert.h>
#include <sys/stat.h>
#include <atlsimpstr.h>
#include  "MacroLib.h"

namespace SilverEngineLib
{

	class SG_API SGSystem {
	public:
		void SystemOutput();
		static bool IsFileExist(std::string fileDir);

	};

}