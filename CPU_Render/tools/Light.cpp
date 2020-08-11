#include "../tools/Light.h"

Light::Light(){
}

Light::~Light(){
}

void Light::SetPosition(Vec3f  v){
	Position = v;
}

void Light::SetLightOfDiffuse(Vec3f  v){
	LightOfDiffuse = v;
}

void Light::SetDirection(Vec3f  v){
	Direction = v;
	Direction.normalize();
}

void Light::SetEyePosition(Vec3f  v){
	eyePosition = v;
}

void Light::SetAmbientLight(Vec3f v){
	LightOfAmbient = v;
}


void Light::SetLightOfSpecular(Vec3f v)
{
	LightOfSpecular = v;
}

Vec3f Light::GetDirection() {
	return Direction;
}

PointLight::PointLight(){
}

PointLight::~PointLight(){

}


void PointLight::SetDirection(Vec3f v)
{
	Direction = Position - v;
	dis = Direction.length();
	Direction.normalize();
}

Vec3f PointLight::ColorShader(Vec3f &normal, Material& material, float visible) {
	
	
	float k = 1.f / (dis * dis);
	float  diffuseFactor = std::max(0.0f, normal.Dot(Direction));
	Vec3f Md = material.DiffuseAlbedo.rgb();
	const float m = (1.0f - material.Roughness.w) * 256.f;
	Vec3f ambient = Md * LightOfAmbient;
	Vec3f diffuse;
	Vec3f specular;

	Vec3f finalColor = ambient;
	if (diffuseFactor > 0.0f) {
		Vec3f diffuse = Md * LightOfDiffuse * diffuseFactor;

		Vec3f h = (Direction + eyePosition);
		h.normalize();
		Vec3f specular = material.Roughness.rgb() * LightOfSpecular * std::pow(std::max(0.f, normal.Dot(h)), m);
		finalColor = finalColor + diffuse * k + specular * k;
	}
	return finalColor;
}




DirectionalLight::DirectionalLight() {
}

DirectionalLight::~DirectionalLight() {
}

Vec3f DirectionalLight::ColorShader(Vec3f & normal, Material& material, float visible) {

	Vec3f Md = material.DiffuseAlbedo.rgb();
	Vec3f ambient  = Md * LightOfAmbient;
	Vec3f lightVec = Direction * -1.f;
	float diffuseFactor = std::max(0.0f, normal.Dot(lightVec));
	const float m = (1.0f - material.Roughness.w) * 256.f;

	Vec3f finalColor = ambient;
	
	if (diffuseFactor > 0.0f){
		Vec3f diffuse = Md *  LightOfDiffuse * diffuseFactor ;
		Vec3f h = (Direction + eyePosition);
		h.normalize();
		Vec3f specular = material.Roughness.rgb() * LightOfSpecular * std::pow(std::max(0.f, normal.Dot(h)), m);
		finalColor = finalColor + diffuse * visible  + specular * visible;
	}
	return finalColor;
}

SpotLight::SpotLight() {
}

SpotLight::~SpotLight() {
}

void SpotLight::SetDirection(Vec3f v) {
	Direction = Position - v;
	dis = Direction.length();
	Direction.normalize();
}

void SpotLight::SetSpot(float q) {
	spot = q;
}

void SpotLight::SetSpotDir(Vec3f d) {
	spotDir = d - Position;
	spotDir.normalize();
}

Vec3f SpotLight::ColorShader(Vec3f & normal, Material& material, float visible) {

	float k = std::pow(std::max(0.0f, spotDir.Dot(Direction)), spot);
	float kWithD = k / dis * dis;

	Vec3f Md = material.DiffuseAlbedo.rgb();
	const float m = (1.0f - material.Roughness.w) * 256.f;
	Vec3f diffuse = Md * LightOfDiffuse * std::max(0.0f, normal.Dot(Direction));
	Vec3f ambient = Md * LightOfAmbient;
	Vec3f h = (Direction + eyePosition);
	h.normalize();
	Vec3f specular = material.Roughness.rgb() * LightOfSpecular * std::pow(std::max(0.f, normal.Dot(h)), m);
	Vec3f finalColor = diffuse * kWithD + ambient * k + specular * kWithD;
	return finalColor;

}

Matrix GetLightMatrix(Vec3f EyePosition, Vec3f FocusPosition, Vec3f UpDirection, Camera *camera) {
	Vec3f mPosition = EyePosition;
	Vec3f mLook = FocusPosition;
	mLook.normalize();
	Vec3f mRight = mLook.CrossProduct(UpDirection);
	mRight.normalize();
	Vec3f mUp = mRight.CrossProduct(mLook);
	mUp.normalize();
	
	Matrix mView;
	
	mView[0] = std::vector<float>{ mRight.x,  mRight.y,  mRight.z,	 -(mPosition.Dot(mRight)) };
	mView[1] = std::vector<float>{ mUp.x,     mUp.y,	 mUp.z,		 -(mPosition.Dot(mUp)) };
	mView[2] = std::vector<float>{ -mLook.x,   -mLook.y, -mLook.z,    (mPosition.Dot(mLook)) };
	mView[3] = std::vector<float>{ 0,	 0,	  0,   1 };

	return camera->GetProjection() * mView;
}
