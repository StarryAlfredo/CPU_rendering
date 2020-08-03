
#ifndef __CAMERA__
#define __CAMERA__
#include"../tools/Math.h"
class Camera {

public:
	Camera();
	virtual ~Camera() = 0;
	//获取相机是否改变的布尔值
	bool GetIsDirty() const;
	void SetDirty(bool is);
	//获取相机位置
	Vec3f GetPosition() const;
	
	//获取相机轴
	Vec3f GetRight() const;
	Vec3f GetUp()	 const;
	Vec3f GetLook()  const;

	//获取矩阵
	Matrix  GetView() const;
	Matrix  GetProjection() const;
	
	//设置视锥体 /设置视口
	void LookAtLH(Vec3f EyePosition, Vec3f FocusPosition, Vec3f UpDirection);
	void PerspectiveFovLH(float FovAngle, float Aspect, float NearZ, float FarZ);

	//更新观察矩阵
	virtual void UpdateViewMatrix() = 0;
	
protected:
	//观察空间最基本的三个轴方向
	Vec3f mPosition;
	Vec3f mRight;
	Vec3f mUp;
	Vec3f mLook;
	//透视矩阵,观察矩阵
	Matrix mView;
	Matrix mProjrct;
	
	//视口信息
	float mFarZ;
	float mFOV;
	float mNearZ;
	float mAspect;

	bool isDirty;
};

class FirstPersonCamera : public Camera {

public:
	FirstPersonCamera();
	~FirstPersonCamera() override;

	//设置摄像机的位置
	void SetPosition(Vec3f &v);

	// 平移
	void Strafe(float d);

	// 直行
	void Walk(float d);
	//上下旋转
	void  Pitch(float rad);
	//左右旋转
	void  RotateY(float rad);
	//向上走
	void UP(float d);

	//更新观察矩阵
	void UpdateViewMatrix() override;

};



#endif // !__CAMERA__
