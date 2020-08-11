#ifndef __Light__
#define __Light__
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include"camera.h"
#include"../tools/Math.h"
#include"Material.h"

class Light {
public:
	Light();
	~Light();
	
	void SetPosition(Vec3f v);
	void SetLightOfDiffuse(Vec3f v); 
	virtual void SetDirection(Vec3f v);
	void SetEyePosition(Vec3f v);
	void SetAmbientLight(Vec3f v);
	void SetLightOfSpecular(Vec3f v);
	Vec3f GetDirection();
	virtual Vec3f ColorShader(Vec3f& normal, Material& material, float visible) = 0;

protected:
	Vec3f Direction;
	Vec3f Position;
	Vec3f eyePosition;
	Vec3f LightOfDiffuse;
	Vec3f LightOfAmbient;
	Vec3f LightOfSpecular;
};

class PointLight : public Light {
public:
	PointLight();
	~PointLight();
	void SetDirection(Vec3f v) override;
	Vec3f ColorShader(Vec3f &normal, Material& material, float visible) override;
private:
	float dis;
};
class DirectionalLight : public Light {
public:
	DirectionalLight();
	~DirectionalLight();
	Vec3f ColorShader(Vec3f &normal, Material& material, float visible) override;
};

class SpotLight : public Light {
public:
	SpotLight();
	~SpotLight();
	void  SetDirection(Vec3f v) override;
	void  SetSpot(float q);
	void  SetSpotDir(Vec3f d);
	Vec3f ColorShader(Vec3f &normal, Material& material, float visible) override;

private:
	float spot;
	float dis;
	Vec3f spotDir;
};

Matrix GetLightMatrix(Vec3f EyePosition, Vec3f FocusPosition, Vec3f UpDirection, Camera *camera);
#endif // ! __Light__
