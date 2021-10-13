
#include "BasicGeomFactory.h"

void Renderer::BasicGeomFactory::BasicBoxGeom(float length, std::vector<SGModelVertex>& outVertexVec, std::vector<unsigned int>& outIndexVec)
{
	outVertexVec = {
		// 0 1 0
		{ glm::vec3(-length, length, -length), glm::vec3(0.0, 1.0, 0.0), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(length, length, -length), glm::vec3(0.0, 1.0, 0.0), glm::vec2(1.0f, 0.0f)  },
		{ glm::vec3(length, length, length), glm::vec3(0.0, 1.0, 0.0), glm::vec2(1.0f, 1.0f)  },
		{ glm::vec3(-length, length, length), glm::vec3(0.0, 1.0, 0.0), glm::vec2(0.0f, 1.0f)  },
		//0 -1 0
		{ glm::vec3(-length, -length, -length), glm::vec3(0.0, -1.0, 0.0), glm::vec2(0.0f, 0.0f)  },
		{ glm::vec3(length, -length, -length), glm::vec3(0.0, -1.0, 0.0), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(length, -length, length), glm::vec3(0.0, -1.0, 0.0), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-length, -length, length), glm::vec3(0.0, -1.0, 0.0), glm::vec2(0.0f, 1.0f) },
		//-1 0 0
		{ glm::vec3(-length, -length, length), glm::vec3(-1.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-length, -length, -length), glm::vec3(-1.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-length, length, -length), glm::vec3(-1.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-length, length, length), glm::vec3(-1.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f) },
		//1 0 0
		{ glm::vec3(length, -length, length), glm::vec3(1.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)  },
		{ glm::vec3(length, -length, -length), glm::vec3(1.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(length, length, -length), glm::vec3(1.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)  },
		{ glm::vec3(length, length, length), glm::vec3(1.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f) },
		//0 0 -1
		{ glm::vec3(-length, -length, -length), glm::vec3(0.0, 0.0, -1.0), glm::vec2(0.0f, 0.0f)  },
		{ glm::vec3(length, -length, -length), glm::vec3(0.0, 0.0, -1.0), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(length, length, -length), glm::vec3(0.0, 0.0, -1.0), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-length, length, -length), glm::vec3(0.0, 0.0, -1.0), glm::vec2(0.0f, 1.0f) },
		//0 0 1
		{ glm::vec3(-length, -length, length), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(length, -length, length), glm::vec3(0.0, 0.0, 1.0), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(length, length, length), glm::vec3(0.0, 0.0, 1.0), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-length, length, length), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0f, 1.0f) },
	};
	outIndexVec = { 3, 1, 0, 2, 1, 3, 6, 4, 5, 7, 4, 6, 11, 9, 8, 10, 9, 11, 14, 12, 13, 15, 12, 14, 19, 17, 16, 18, 17, 19, 22, 20, 21, 23, 20, 22 };
}

void Renderer::BasicGeomFactory::BasicPlaneGeom(float width, float height, std::vector<SGModelVertex>& outVertexVec, 
	std::vector<unsigned int>& outIndexVec, int textureLoopNum)
{
	float half_width = width / 2.0;
	float half_height = height / 2.0;
	//position uv  normal
	outVertexVec = {
	{ glm::vec3(-half_width, 0, -half_width),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
	{ glm::vec3(half_width, 0, -half_width),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(textureLoopNum, 0.0f) },
	{ glm::vec3(half_width, 0, half_width),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(textureLoopNum, textureLoopNum) },
	{ glm::vec3(-half_width, 0, half_width),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, textureLoopNum) }
	};
	outIndexVec = { 3, 1, 0, 2, 1, 3 };
}

//UV����������Geometry
void Renderer::BasicGeomFactory::BasicSphereGeom(float radius, std::vector<SGModelVertex>& outVertexVec, std::vector<unsigned int>& outIndexVec)
{
	int x_segment_num = 16;
	int y_segment_num = 16;

	//����vertex
	for (int y = 0; y <= y_segment_num; ++y)
	{
		for (int x = 0; x <= x_segment_num; ++x)
		{
			float x_segment = (float)x / (float)x_segment_num;
			float y_segment = (float)y / (float)y_segment_num;
			float yaw = x_segment * 2.0f * MATH_PI; //����γ��360
			float pitch = y_segment * MATH_PI; //���徫��180
			float xpos = cos(yaw) * sin(pitch) * radius;
			float ypos = cos(pitch) * radius;
			float zpos = sin(yaw) * sin(pitch) * radius;
			//position , uv, normal
			SGModelVertex pbrVertex(glm::vec3(xpos, ypos, zpos), glm::vec3(xpos, ypos, zpos), glm::vec2(x_segment, y_segment));
			outVertexVec.push_back(pbrVertex);
		}
	}
	//����indices
	bool odd = false;
	for (int y = 0; y < y_segment_num; ++y)
	{
		if (!odd) {
			for (int x = 0; x <= x_segment_num; ++x)
			{
				outIndexVec.push_back(y * (x_segment_num + 1) + x);
				outIndexVec.push_back((y + 1) * (x_segment_num + 1) + x);
			}
		}
		else {
			for (int x = x_segment_num; x >= 0; --x) {
				outIndexVec.push_back((y + 1) * (x_segment_num + 1) + x);
				outIndexVec.push_back(y * (x_segment_num + 1) + x);

			}
		}
		odd = !odd;
	}
}

//����Բ����
void Renderer::BasicGeomFactory::BasicCylinderGeom(float radius, float thickness, std::vector<SGModelVertex>& outVertexVec, std::vector<unsigned int>& outIndexVec)
{
	int slices = 12;
	glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0); //��ʼ�㴦��Բ��
	//��һ��Բ��
	glm::vec3 origin_point1 = glm::vec3(thickness / 2.0, 0.0, 0.0);
	//�ڶ���Բ��
	glm::vec3 origin_point2 = glm::vec3(-thickness / 2.0, 0.0, 0.0);
	//�ȷ����һ��Բ��
	SGModelVertex origin_basevertex1(origin_point1, glm::vec3(1.0, 0.0, 0.0), glm::vec2(0.5f, 0.5f));
	outVertexVec.push_back(origin_basevertex1);
	float angleDelta = 360.0 / slices;
	float currentAngle = 0.0f;
	//��һ��Բ��
	for (int i = 0; i < slices; ++i) {
		float ypos = cos(currentAngle * MATH_RADIANCE) * radius;
		float zpos = sin(currentAngle * MATH_RADIANCE) * radius;
		float xpos = thickness / 2.0;
		float u = (zpos + radius) / (2 * radius);
		float v = (radius - ypos) / (2 * radius);
		SGModelVertex singleVertex(glm::vec3(xpos, ypos, zpos), glm::vec3(1.0, 0.0, 0.0), glm::vec2(u, v));
		outVertexVec.push_back(singleVertex);
		currentAngle += angleDelta;
	}
	//���õ�һ��Բ�������
	for (int i = 1; i <= slices; ++i) {
		int origin_index = 0;
		int first_index = i;
		int second_index = (i == slices) ? 1 : i + 1;
		outIndexVec.push_back(origin_index);
		outIndexVec.push_back(first_index);
		outIndexVec.push_back(second_index);
	}
	//�ڶ���Բ��
	SGModelVertex origin_basevertex2(origin_point2, glm::vec3(-1.0, 0.0, 0.0), glm::vec2(0.5f, 0.5f));
	outVertexVec.push_back(origin_basevertex2);
	//�ڶ���Բ
	for (int i = 0; i < slices; ++i) {
		float ypos = cos(currentAngle * MATH_RADIANCE) * radius;
		float zpos = sin(currentAngle * MATH_RADIANCE) * radius;
		float xpos = -thickness / 2.0;
		float u = (zpos + radius) / (2 * radius);
		float v = (radius - ypos) / (2 * radius);
		SGModelVertex singleVertex(glm::vec3(xpos, ypos, zpos), glm::vec3(-1.0, 0.0, 0.0), glm::vec2(u, v));
		outVertexVec.push_back(singleVertex);
		currentAngle += angleDelta;
	}
	//���õڶ���Բ�������
	for (int i = 1; i <= slices; ++i) {
		int origin_index = 0;
		int first_index = i;
		int second_index = (i == slices) ? 1 : i + 1;
		outIndexVec.push_back(origin_index + slices + 1);
		outIndexVec.push_back(first_index + slices + 1);
		outIndexVec.push_back(second_index + slices + 1);
	}
	//һ����2 * (slices+1) ������
	//��ȥԲ�ģ���Ч���㣺1  .. slices,   slices+2 .. 2*slices+1
	for (int i = 1; i <= slices; ++i) {
		//��һ��������
		outIndexVec.push_back(i);
		outIndexVec.push_back(i + slices + 1);
		outIndexVec.push_back((i == slices) ? (slices + 2) : (i + slices + 2));
		//�ڶ���������
		outIndexVec.push_back(i);
		outIndexVec.push_back((i == slices) ? (slices + 2) : (i + slices + 2));
		outIndexVec.push_back((i == slices) ? (1) : (i + 1));
	}
}
