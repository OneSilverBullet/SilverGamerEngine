#include "Application.h"

using namespace Renderer;






int main(int argc, char** argv) {
	SGApplication::GetInstance().Init(argc, argv);
	SGApplication::GetInstance().Run();
	// ��ô�ã� CEPHEI_LOGXXX��Щ���ùܣ����������

	return 0;
}

