#include "Application.h"

using namespace Renderer;






int main(int argc, char** argv) {
	SGApplication::GetInstance().Init(argc, argv);
	SGApplication::GetInstance().Run();
	// 怎么用， CEPHEI_LOGXXX那些不用管，拿来输出的

	return 0;
}

