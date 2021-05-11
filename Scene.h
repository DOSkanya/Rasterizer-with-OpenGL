#pragma once

#include "Model.h"
#include "Light.h"
#include "Rasterizer.h"

class Scene {
public:
	std::vector<Model> ModelList;
	std::vector<Light> LightList;
	void Set_Perspective_Matrix(float FOV, float ratio, float n, float f);
	void Set_Viewport_Matrix(int Screen_Width, int Screen_Height);
	void Draw(Rasterizer& rst);
};