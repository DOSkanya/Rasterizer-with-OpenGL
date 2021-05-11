#pragma once

#include <Eigen/Dense>
#include <vector>
#include "Light.h"
#include "Rasterizer.h"
#include "Texture.h"

class Triangle {
public:
	//Members
	Eigen::Vector4f Vertices[3];
	Eigen::Vector4f Normals[3];
	Eigen::Vector2f TexCoords[3];
	//Functions
	//Different approaches to set vertex data
	void Set_Vertex(int index, Eigen::Vector4f vertex);
	void Set_Vertex(int index, float x, float y, float z);
	void Set_Normal(int index, Eigen::Vector4f normal);
	void Set_Normal(int index, float x, float y, float z);
	void Set_TexCoord(int index, Eigen::Vector2f texcoord);
	void Set_TexCoord(int index, float u, float v);

	std::tuple<int, int, int, int> BoundingBox();
	std::tuple<float, float, float> Barycentric_Coord(float u, float v);
	void Draw(Rasterizer& rst, std::vector<Light> LightList, Texture& image);
	//void LightShading(std::vector<Eigen::Vector3f>& Pixel_Color, std::vector<Light> LightList, Eigen::Vector3f Position, Eigen::Vector3f Normal, int u, int v, int Screen_Width);
};