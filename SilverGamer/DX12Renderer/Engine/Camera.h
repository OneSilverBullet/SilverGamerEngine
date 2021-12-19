#ifndef SG_DX_CAMERA
#define SG_DX_CAMERA

#include "../Common/d3dx12.h"
#include "../Common/d3dUtil.h"
#include <DirectXMath.h>

using namespace DirectX;

class ICamera
{
public:
	ICamera() : camera_fov(XM_PIDIV4), camera_aspect(1.33f), camera_nearZ(0.1f), camera_farZ(100.0f) {}
	virtual XMMATRIX GetViewMatrix() = 0;
	virtual XMMATRIX GetViewMatrixForSkybox() = 0;
	virtual ~ICamera() {}
	virtual void SetTarget(XMFLOAT3) {}
	virtual void AddDistance(float dt) {}
	virtual void DecDistance(float dt) {}
	virtual XMFLOAT3 GetPosition() = 0;

	XMMATRIX GetProjMatrix()
	{
		XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(camera_fov, camera_aspect, camera_nearZ, camera_farZ);
		return projMatrix;
	}
public:
	float GetFov() { return camera_fov; }
	float GetAspect() { return camera_aspect; }
	float GetNearZ() { return camera_nearZ; }
	float GetFarZ() { return camera_farZ; }

private:
	float camera_fov;
	float camera_aspect;
	float camera_nearZ;
	float camera_farZ;


};

class ThirdRoleCamera : public ICamera
{
public:
	ThirdRoleCamera();
	void InitializeThirdRoleCamera();
	void RotateCamera(float xoffset, float yoffset);
	XMFLOAT3 GetPosition();
	XMMATRIX GetViewMatrix();
	XMMATRIX GetViewMatrixForSkybox();
	void SetTarget(XMFLOAT3 tar) { thirdcamera_target = tar; }
public:
	XMFLOAT3 GetForward() {
		XMFLOAT3 position = GetPosition();
		XMVECTOR position_xm = XMLoadFloat3(&position);
		XMVECTOR target_xm = XMLoadFloat3(&thirdcamera_target);
		XMVECTOR forward_xm = XMVector3Normalize(target_xm - position_xm);
		XMFLOAT3 forward;
		XMStoreFloat3(&forward, forward_xm);
		return forward;
	}
	float GetCameraYaw() { return thirdcamera_yaw; }
	float GetCameraPitch() { return thirdcamera_pitch; }

	void MoveDistance(float dt) {
		thirdcamera_distance += dt;
		if (thirdcamera_distance > 10.0f) thirdcamera_distance = 10.0f;
		if (thirdcamera_distance < 2.0f) thirdcamera_distance = 2.0f;
	}

	void AddDistance(float dt) {
		thirdcamera_distance += dt;
		if (thirdcamera_distance > 10.0f) thirdcamera_distance = 10.0f;
	}
	void DecDistance(float dt) {
		thirdcamera_distance -= dt;
		if (thirdcamera_distance < 2.0f) thirdcamera_distance = 2.0f;
	}
private:
	XMFLOAT3 thirdcamera_target;
	XMFLOAT3 thirdcamera_up;
	float thirdcamera_sensity;
	float thirdcamera_distance;
	float thirdcamera_yaw;
	float thirdcamera_pitch;
};

class FirstRoleCamera : public ICamera
{
public:
	FirstRoleCamera() : ICamera() {}
	void InitializeFirstRoleCamera();
	void SetYawAndPitch(float yaw, float pitch);
	void RotateCamera(float xoffset, float yoffset);
	void UpdateForward();
	XMMATRIX GetViewMatrix();
	XMMATRIX GetViewMatrixForSkybox();

public:
	void SetTarget(XMFLOAT3 pos) { firstcamera_position = pos; }
	XMFLOAT3 GetPosition() { return firstcamera_position; }
	XMFLOAT3 GetForward() { return firstcamera_forward; }
private:
	float firstcamera_pitch;
	float firstcamera_yaw;
	float firstcamera_rotateSpeed;
	XMFLOAT3 firstcamera_position;
	XMFLOAT3 firstcamera_forward;
	XMFLOAT3 firstcamera_up;
};









#endif