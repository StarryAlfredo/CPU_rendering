#ifndef OBJECT_H
#define OBJECT_H
#include"../shader/Pipeline.h"
#include"../tools/model.h"
#include"../shader/BlinnShader.h"

class Object{
 public:
	Object(std::string name_t, Model * model_t);
	~Object();
	void Draw(Pipeline& pipline, renderWindow& ren);

 private:
	std::string shaderName_;
	Model* mesh_;
};

#endif // !OBJECT_H
