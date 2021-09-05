

#include "Application.h"

using namespace Renderer;

int main(int argc, char** argv) {
	SGApplication::GetInstance().Init(argc, argv);
	SGApplication::GetInstance().Run();
	return 0;
}