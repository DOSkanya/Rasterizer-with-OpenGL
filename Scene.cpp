#include "Scene.h"

void Scene::Set_Perspective_Matrix(float FOV, float ratio, float n, float f) {
	Eigen::Matrix4f Perspective;
	Perspective << n, 0.0f, 0.0f, 0.0f,
				0.0f, n, 0.0f, 0.0f,
				0.0f, 0.0f, n + f, -n * f,
				0.0f, 0.0f, 1.0f, 0.0f;

	Eigen::Matrix4f Orthogonal;
	float fov = Model::Radians(FOV);
	Orthogonal << 1.0f / (n * ratio * tan(fov)), 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f / (n * tan(fov)), 0.0f, 0.0f,
				0.0f, 0.0f, 2.0f / (n - f), (n + f) / (f - n),
				0.0f, 0.0f, 0.0f, 1.0f;

	Eigen::Matrix4f Result = Orthogonal * Perspective;

	for (auto &model : ModelList) {
		model.Set_Perspective_Matrix(Result);
	}
}

void Scene::Set_Viewport_Matrix(int Screen_Width, int Screen_Height) {
	Eigen::Matrix4f Result;
	float width = (float)Screen_Width;
	float height = (float)Screen_Height;
	Result << width / 2.0f, 0.0f, 0.0f, (width - 1.0f) / 2.0f,
			0.0f, height / 2.0f, 0.0f, (height - 1.0f) / 2.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f;
	for (auto& model : ModelList) {
		model.Set_Viewport_Matrix(Result);
	}
}

void Scene::Draw(Rasterizer& rst) {
	for (auto& model : ModelList) {
		model.Draw(rst, LightList);
	}
}