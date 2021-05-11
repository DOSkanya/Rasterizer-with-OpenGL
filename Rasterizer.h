#pragma once

#include <vector>
#include <Eigen/Dense>
class Scene;

class Rasterizer {
public:
	//Members
	int Screen_Width, Screen_Height;
	std::vector<Eigen::Vector3f> Pixel_Color;
	std::vector<float> Depth_Buffer;
	Eigen::Vector3f Background_Color;
	//Functions
	Rasterizer(int width, int height);
	void Set_Backgroud_Color(Eigen::Vector3f color = Eigen::Vector3f(0.0f, 0.0f, 0.0f));
	void Render_Frame(Scene* Scene_Layout);
	void Convert_To_Array(float*& target);
	void Clear_Color();
	void Clear_Depth();
};