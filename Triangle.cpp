#include "Triangle.h"
#include <iostream>

// Functions to set corresponding member
// Start here
void Triangle::Set_Vertex(int index, Eigen::Vector4f vertex) {
	Vertices[index] = vertex;
}

void Triangle::Set_Vertex(int index, float x, float y, float z) {
	Eigen::Vector4f vertex(x, y, z, 1.0f);
	Vertices[index] = vertex;
}

void Triangle::Set_Normal(int index, Eigen::Vector4f normal) {
	Normals[index] = normal;
}

void Triangle::Set_Normal(int index, float x, float y, float z) {
	Eigen::Vector4f normal(x, y, z, 0.0f);
	Normals[index] = normal;
}

void Triangle::Set_TexCoord(int index, Eigen::Vector2f texcoord) {
	TexCoords[index] = texcoord;
}

void Triangle::Set_TexCoord(int index, float u, float v) {
	Eigen::Vector2f texcoord(u, v);
	TexCoords[index] = texcoord;
}
// Functions to set correponding member
// End here

std::tuple<int, int, int, int> Triangle::BoundingBox() {
	int xMin, xMax, yMin, yMax;
	xMin = (int)std::floor(std::min(Vertices[0].x(), std::min(Vertices[1].x(), Vertices[2].x())));
	xMax = (int)std::ceil(std::max(Vertices[0].x(), std::max(Vertices[1].x(), Vertices[2].x())));
	yMin = (int)std::floor(std::min(Vertices[0].y(), std::min(Vertices[1].y(), Vertices[2].y())));
	yMax = (int)std::ceil(std::max(Vertices[0].y(), std::max(Vertices[1].y(), Vertices[2].y())));
	return { xMin, xMax, yMin, yMax };
}

std::tuple<float, float, float> Triangle::Barycentric_Coord(float u, float v) {
	float denominator = (Vertices[1].x() - Vertices[0].x()) * (Vertices[2].y() - Vertices[0].y()) -
		(Vertices[2].x() - Vertices[0].x()) * (Vertices[1].y() - Vertices[0].y());
	float beta_numerator = (u - Vertices[0].x()) * (Vertices[2].y() - Vertices[0].y()) -
		(Vertices[2].x() - Vertices[0].x()) * (v - Vertices[0].y());
	float gamma_numerator = (Vertices[1].x() - Vertices[0].x()) * (v - Vertices[0].y()) -
		(u - Vertices[0].x()) * (Vertices[1].y() - Vertices[0].y());
	float beta = beta_numerator / denominator;
	float gamma = gamma_numerator / denominator;
	float alpha = 1 - beta - gamma;
	return { alpha, beta, gamma };
}

void Triangle::Draw(Rasterizer& rst, std::vector<Light> LightList, Texture& image) {
	auto [xMin, xMax, yMin, yMax] = BoundingBox();
	for (int v = yMin; v <= yMax ; v++) {
		for (int u = xMin; u <= xMax; u++) {
			if (v >=0 && v < rst.Screen_Height && u >= 0 && u < rst.Screen_Width) {//Abort all the points outside screen
				auto [alpha, beta, gamma] = Barycentric_Coord((float)u, (float)v);
				if (alpha >= 0 && beta >= 0 && gamma >= 0) {//Be sure that the point we rendering is in the triangle
					float interpolated_z = alpha * Vertices[0].z() + beta * Vertices[1].z() + gamma * Vertices[2].z();
					if (interpolated_z > rst.Depth_Buffer[u + v * rst.Screen_Width]) {//Depth Testing
						rst.Depth_Buffer[u + v * rst.Screen_Width] = interpolated_z;

						Eigen::Vector4f Position = alpha * Vertices[0] + beta * Vertices[1] + gamma * Vertices[2];
						Eigen::Vector3f Normal = (alpha * Normals[0] + beta * Normals[1] + gamma * Normals[2]).segment(0, 3);
						Eigen::Vector2f TexCoord = alpha * TexCoords[0] + beta * TexCoords[1] + gamma * TexCoords[2];
						Normal.normalize();
						Eigen::Vector3f I = LightList[0].Position - Position.segment(0, 3);
						I.normalize();
						cv::Vec3b Color = image.Lookup_Color(TexCoord.x(), TexCoord.y());
						Eigen::Vector3f Texture_Color((float)Color[2] / 255.0f, (float)Color[1] / 255.0f, (float)Color[0] / 255.0f);
						rst.Pixel_Color[u + v * rst.Screen_Width] = std::max(I.dot(Normal), 0.0f) * LightList[0].Diffuse.cwiseProduct(Texture_Color);

						//LightShading(Pixel_Color, LightList, Position.segment(0, 3), Normal, u, v, Screen_Width);
					}
				}
			}
		}
	}
}
/*
void Triangle::LightShading(std::vector<Eigen::Vector3f>& Pixel_Color, std::vector<Light> LightList, Eigen::Vector3f Position, Eigen::Vector3f Normal, int u, int v, int Screen_Width) {
	Eigen::Vector3f Result = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	for (auto light : LightList) {
		Eigen::Vector3f LightDir = light.Position - Position;
		LightDir.normalize();
		Eigen::Vector3f ViewDir = Eigen::Vector3f(500.0f, 500.0f, 0.0f) - Position;
		ViewDir.normalize();
		Eigen::Vector3f ReflectDir = 2 * LightDir.dot(Normal) * Normal - LightDir;
		ReflectDir.normalize();
		Eigen::Vector3f Diffuse = std::max(LightDir.dot(Normal), 0.0f) * light.Diffuse;
		Eigen::Vector3f Specular = pow(std::max(ReflectDir.dot(ViewDir), 0.0f), light.Shininess) * light.Specular;
		Eigen::Vector3f Ambient = light.Ambient;
		Result += Diffuse + Specular + Ambient;
	}
	Pixel_Color[u + v * Screen_Width] = Result;
}
*/


