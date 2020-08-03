
#ifndef __CAMERA__
#define __CAMERA__
#include"../tools/Math.h"
class Camera {

public:
	Camera();
	virtual ~Camera() = 0;
	//��ȡ����Ƿ�ı�Ĳ���ֵ
	bool GetIsDirty() const;
	void SetDirty(bool is);
	//��ȡ���λ��
	Vec3f GetPosition() const;
	
	//��ȡ�����
	Vec3f GetRight() const;
	Vec3f GetUp()	 const;
	Vec3f GetLook()  const;

	//��ȡ����
	Matrix  GetView() const;
	Matrix  GetProjection() const;
	
	//������׶�� /�����ӿ�
	void LookAtLH(Vec3f EyePosition, Vec3f FocusPosition, Vec3f UpDirection);
	void PerspectiveFovLH(float FovAngle, float Aspect, float NearZ, float FarZ);

	//���¹۲����
	virtual void UpdateViewMatrix() = 0;
	
protected:
	//�۲�ռ�������������᷽��
	Vec3f mPosition;
	Vec3f mRight;
	Vec3f mUp;
	Vec3f mLook;
	//͸�Ӿ���,�۲����
	Matrix mView;
	Matrix mProjrct;
	
	//�ӿ���Ϣ
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

	//�����������λ��
	void SetPosition(Vec3f &v);

	// ƽ��
	void Strafe(float d);

	// ֱ��
	void Walk(float d);
	//������ת
	void  Pitch(float rad);
	//������ת
	void  RotateY(float rad);
	//������
	void UP(float d);

	//���¹۲����
	void UpdateViewMatrix() override;

};



#endif // !__CAMERA__
