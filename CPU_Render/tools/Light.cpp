#include "../tools/Light.h"

Light::Light(){
}

Light::~Light(){
}

void Light::SetPosition(Vec3f  v){
	Position = v;
}

void Light::SetdiffuseK(Vec3f  v){
	diffuseK = v;
}

void Light::SetambientK(Vec3f  v){
	ambientK = v;
}

void Light::SetSpecularK(Vec3f  v){
	specularK = v;
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


void Light::SetM(float v){
	m = v;
}

void Light::SetLightOfSpecular(Vec3f v)
{
	LightOfSpecular = v;
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

Vec3f PointLight::ColorShader(Vec3f &normal){
	
	
	float k = 1.f / (dis * dis);
	float  diffuseFactor = std::max(0.0f, normal.Dot(Direction));
	
	Vec3f ambient = ambientK * LightOfAmbient;
	Vec3f diffuse;
	Vec3f specular;

	Vec3f finalColor = ambient;
	if (diffuseFactor > 0.0f) {
		Vec3f diffuse = diffuseK * LightOfDiffuse * diffuseFactor;

		Vec3f h = (Direction + eyePosition);
		h.normalize();
		Vec3f specular = specularK * LightOfSpecular * std::pow(std::max(0.f, normal.Dot(h)), m);
		finalColor = finalColor + diffuse * k + specular * k;
	}
	return finalColor;
}




DirectionalLight::DirectionalLight(){
}

DirectionalLight::~DirectionalLight(){
}

Vec3f DirectionalLight::ColorShader(Vec3f & normal){


	float  diffuseFactor = std::max(0.0f, normal.Dot(Direction));
	Vec3f ambient = ambientK * LightOfAmbient;
	Vec3f finalColor = ambient;
	if (diffuseFactor > 0.0f){
		Vec3f diffuse = diffuseK * LightOfDiffuse * diffuseFactor;
		Vec3f h = (Direction + eyePosition);
		h.normalize();
		Vec3f specular = specularK * LightOfSpecular * std::pow(std::max(0.f, normal.Dot(h)), m);
		finalColor =finalColor + diffuse  + specular;
	}
	return finalColor;
}

SpotLight::SpotLight()
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::SetDirection(Vec3f v)
{
	Direction = Position - v;
	dis = Direction.length();
	Direction.normalize();
}

void SpotLight::SetSpot(float q)
{
	spot = q;
}

void SpotLight::SetSpotDir(Vec3f d)
{
	spotDir = d - Position;
	spotDir.normalize();
}

Vec3f SpotLight::ColorShader(Vec3f & normal){	

	float k = std::pow(std::max(0.0f, spotDir.Dot(Direction)), spot);
	float kWithD = k / dis * dis;
	Vec3f diffuse = diffuseK * LightOfDiffuse * std::max(0.0f, normal.Dot(Direction));
	Vec3f ambient = ambientK * LightOfAmbient;
	Vec3f h = (Direction + eyePosition);
	h.normalize();
	Vec3f specular = specularK * LightOfSpecular * std::pow(std::max(0.f, normal.Dot(h)), m);
	Vec3f finalColor = diffuse * kWithD + ambient * k + specular * kWithD;
	return finalColor;
}
