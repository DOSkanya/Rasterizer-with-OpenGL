#pragma once

#include <string>
#include <vector>
#include "Triangle.h"
#include "Texture.h"
#include "Rasterizer.h"

class Model {
public:
	//Member
	std::vector<Triangle*> TriangleList;
	Eigen::Matrix4f Mod;//Model transformation, shouldn't be the same name of class
	Eigen::Matrix4f View;//View transformation
	Eigen::Matrix4f Perspective;//Perspecitve transformation
	Eigen::Matrix4f Viewport;//Viewport transformation

	Texture Source_Image;
	//Function
	Model();
	Model(std::string path);
	void Load_From_File(std::string path);
	void Load_Texture(std::string path);
	void Set_Model_Matrix(float scale, float rotate, Eigen::Vector3f translate = Eigen::Vector3f(0.0f, 0.0f, 0.0f));
	void Set_Perspective_Matrix(Eigen::Matrix4f matrix);
	void Set_Viewport_Matrix(Eigen::Matrix4f matrix);
	std::vector<Triangle*> Apply_Transformation();

	void Draw(Rasterizer& rst, std::vector<Light> LightList);

	static float Radians(float rotate);
};