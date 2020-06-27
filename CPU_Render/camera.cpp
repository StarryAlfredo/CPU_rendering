#include "camera.h"

Camera::Camera(){
	isDirty = false;
}
Camera::~Camera(){}
bool Camera::GetIsDirty() const
{
	return isDirty;
}
void Camera::SetDirty(bool is)
{
	isDirty = is;
}
Vec3f Camera::GetPosition() const{
	return mPosition;
}

Vec3f Camera::GetRight() const{
	return mRight;
}

Vec3f Camera::GetUp() const{
	return mUp;
}

Vec3f Camera::GetLook() const{
	return mLook;
}

Matrix Camera::GetView() const{
	return mView;
}

Matrix Camera::GetProjection() const{
	return mProjrct;
}

void Camera::LookAtLH(Vec3f EyePosition, Vec3f FocusPosition, Vec3f UpDirection){
	mPosition = EyePosition;
	
	mLook =   FocusPosition  - EyePosition;
	mLook.normalize();
	mRight =mLook.crossProduct(UpDirection);
	mRight.normalize();
	mUp = mRight.crossProduct(mLook);
	mUp.normalize();

//	mView[0] = std::vector<float>{ mRight.x,  mRight.y, mRight.z, -(EyePosition * mRight) };
//	mView[1] = std::vector<float>{ mUp.x,  mUp.y,	mUp.z, -(EyePosition * mUp) };
//	mView[2] = std::vector<float>{ mLook.x,  mLook.y,	mLook.z, -(EyePosition * mLook) };
//	mView[3] = std::vector<float>{ 0,	 0,	  0, 1 };
}

void Camera::PerspectiveFovLH(float FovAngle, float Aspect, float NearZ, float FarZ){
	mAspect = Aspect;
	mFOV    = FovAngle;
	mNearZ  = NearZ;
	mFarZ   = FarZ;


	float t = tan(FovAngle * 0.5f * PI / 180.f) * NearZ;	 // width / height top = tan / 2  * near
	float b = -t;
	float r = t * Aspect;
	float l = -r;
	mProjrct[0] = std::vector<float>{ 2 * NearZ / (r - l),	0,				(r + l) / (r - l),													0 };
	mProjrct[1] = std::vector<float>{ 0,					2 * NearZ / (t - b),	(t + b) / (t - b),										 0 };
	mProjrct[2] = std::vector<float>{ 0,					0,				-(FarZ + NearZ) / (FarZ - NearZ),-2.0f *FarZ*NearZ / (FarZ - NearZ) };
	mProjrct[3] = std::vector<float>{ 0,					0,				1 ,															0 };
}

FirstPersonCamera::FirstPersonCamera(){
}

FirstPersonCamera::~FirstPersonCamera(){
}

void FirstPersonCamera::SetPosition(Vec3f & v){
	mPosition = v;
}

void FirstPersonCamera::Strafe(float d){
	mPosition.x = d * mRight.x + mPosition.x;
	mPosition.y = d * mRight.y + mPosition.y;
	mPosition.z = d * mRight.z + mPosition.z;
}

void FirstPersonCamera::Walk(float d)	{

	mPosition.x = d * mLook.x + mPosition.x;
	mPosition.y = d * mLook.y + mPosition.y;
	mPosition.z = d * mLook.z + mPosition.z;


}

void FirstPersonCamera::Pitch(float rad){
	Matrix r = MatrixRotationAxis(mRight,rad);

	Vec4f temp = r * Vec4f(mUp, 1.0f);
	Vec3f temp2 = Vec3f(temp.x, temp.y, temp.z);
	float cosPhi = VectorGetY(temp2);

	if (fabs(cosPhi) > cosf(PI / 8))
		return;
	
	mUp = temp2;

	temp = r * Vec4f(mLook, 1.0f);
	mLook = Vec3f(temp.x, temp.y, temp.z);

	mUp.normalize();
	mLook.normalize();
	
}

void FirstPersonCamera::RotateY(float rad){
	
	Matrix r = MatrixRotationY(rad);
	
	Vec4f temp = r * Vec4f(mRight,1.0f);
	mRight = Vec3f(temp.x,temp.y,temp.z);

	temp = r * Vec4f(mUp, 1.0f);
	mUp = Vec3f(temp.x, temp.y, temp.z);
	
	temp = r * Vec4f(mLook, 1.0f);
	mLook = Vec3f(temp.x, temp.y, temp.z);
	
	mLook.normalize();
	mRight.normalize();
	mUp.normalize();

}	




void FirstPersonCamera::UpdateViewMatrix(){


	mLook.normalize();
	mRight.normalize();
	mUp = mRight.crossProduct(mLook);
	mUp.normalize();
	
	mView[0] = std::vector<float>{ mRight.x,  mRight.y,  mRight.z,	 -(mPosition * mRight) };
	mView[1] = std::vector<float>{ mUp.x,     mUp.y,	 mUp.z,		 -(mPosition * mUp) };
	mView[2] = std::vector<float>{ -mLook.x,   -mLook.y, -mLook.z, (mPosition * mLook) };
	mView[3] = std::vector<float>{ 0,	 0,	  0,   1 }; 
}
