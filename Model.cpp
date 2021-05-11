#include "Model.h"
#include <OBJ_Loader.h>
#include <iostream>

Model::Model() {
	//Initialize transform matrices
	Mod = Eigen::Matrix4f::Identity();
	View = Eigen::Matrix4f::Identity();
	Perspective = Eigen::Matrix4f::Identity();
	Viewport = Eigen::Matrix4f::Identity();
}

Model::Model(std::string path) {
	objl::Loader obj_file;
	obj_file.LoadFile(path);

	for (auto mesh : obj_file.LoadedMeshes) {
		for (int i = 0; i < obj_file.LoadedVertices.size(); i += 3) {
			Triangle* t = new Triangle;
			for (int j = 0; j < 3; j++) {
				t->Set_Vertex(j, obj_file.LoadedVertices[i + j].Position.X, obj_file.LoadedVertices[i + j].Position.Y, obj_file.LoadedVertices[i + j].Position.Z);
				t->Set_Normal(j, obj_file.LoadedVertices[i + j].Normal.X, obj_file.LoadedVertices[i + j].Normal.Y, obj_file.LoadedVertices[i + j].Normal.Z);
				t->Set_TexCoord(j, obj_file.LoadedVertices[i + j].TextureCoordinate.X, obj_file.LoadedVertices[i + j].TextureCoordinate.Y);
			}
			TriangleList.push_back(t);
		}
	}

	//Initialize transform matrices
	Mod = Eigen::Matrix4f::Identity();
	View = Eigen::Matrix4f::Identity();
	Perspective = Eigen::Matrix4f::Identity();
	Viewport = Eigen::Matrix4f::Identity();
}

void Model::Load_From_File(std::string path) {
	objl::Loader obj_file;
	obj_file.LoadFile(path);

	for (auto mesh : obj_file.LoadedMeshes) {
		for (int i = 0; i < obj_file.LoadedVertices.size(); i += 3) {
			Triangle* t = new Triangle;
			for (int j = 0; j < 3; j++) {
				t->Set_Vertex(j, obj_file.LoadedVertices[i + j].Position.X, obj_file.LoadedVertices[i + j].Position.Y, obj_file.LoadedVertices[i + j].Position.Z);
				t->Set_Normal(j, obj_file.LoadedVertices[i + j].Normal.X, obj_file.LoadedVertices[i + j].Normal.Y, obj_file.LoadedVertices[i + j].Normal.Z);
				t->Set_TexCoord(j, obj_file.LoadedVertices[i + j].TextureCoordinate.X, obj_file.LoadedVertices[i + j].TextureCoordinate.Y);
			}
			TriangleList.push_back(t);
		}
	}
}

void Model::Load_Texture(std::string path) {
	Source_Image.Load_Texture(path);
}

void Model::Set_Model_Matrix(float scale, float rotate, Eigen::Vector3f translate) {
	Eigen::Matrix4f Translate;
	Translate << 1.0f, 0.0f, 0.0f, translate.x(),
				0.0f, 1.0f, 0.0f, translate.y(),
				0.0f, 0.0f, 1.0f, translate.z(),
				0.0f, 0.0f, 0.0f, 1.0f;

	Eigen::Matrix4f Rotate;
	float radians = Radians(rotate);
	Rotate << cos(radians), 0.0f, sin(radians), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sin(radians), 0.0f, cos(radians), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f;

	Eigen::Matrix4f Scale;
	Scale << scale, 0.0f, 0.0f, 0.0f,
			0.0f, scale, 0.0f, 0.0f,
			0.0f, 0.0f, scale, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f;

	Mod = Translate * Rotate * Scale;
}

void Model::Set_Perspective_Matrix(Eigen::Matrix4f matrix) {
	Perspective = matrix;
}

void Model::Set_Viewport_Matrix(Eigen::Matrix4f matrix) {
	Viewport = matrix;
}

/*Description:
* To make second use of original data, like we may change some matrices and do rendering again,
* we need to keep the original one, and create a copy of it,
* the transformation that we want to apply to TriangleList should be donw with TriangleList_Copy.
* That's why here in this function I first get a copy,
* then use our transformation matrix, and finally return it.
*/
std::vector<Triangle*> Model::Apply_Transformation() {
	std::vector<Triangle*> TriangleList_Copy;
	for (auto& triangle : TriangleList) {
		Triangle* t = new Triangle;
		for (int i = 0; i < 3; i++) {
			t->Set_Vertex(i, triangle->Vertices[i]);
			t->Set_Normal(i, triangle->Normals[i]);
			t->Set_TexCoord(i, triangle->TexCoords[i]);
		}
		TriangleList_Copy.push_back(t);
	}

	Eigen::Matrix4f Transformation = View * Mod;
	Eigen::Matrix4f Transformation_Inverse = Transformation.inverse().transpose();
	Transformation = Perspective * Transformation;
	for (auto& triangle : TriangleList_Copy) {
		for (int i = 0; i < 3; i++) {
			triangle->Vertices[i] = Transformation * triangle->Vertices[i];
			triangle->Vertices[i] = triangle->Vertices[i] / triangle->Vertices[i].w();//Don't forget to divide w, or error will occur in rendering
			triangle->Normals[i] = Transformation_Inverse * triangle->Normals[i];
			triangle->Normals[i] = triangle->Normals[i].normalized();
		}
	}

	Transformation = Viewport;
	for (auto& triangle : TriangleList_Copy) {
		for (int i = 0; i < 3; i++) {
			triangle->Vertices[i] = Transformation * triangle->Vertices[i];
		}
	}

	return TriangleList_Copy;
}

void Model::Draw(Rasterizer& rst, std::vector<Light> LightList) {
	std::vector<Triangle*> Transformed_Triangles = Apply_Transformation();
	for (auto& triangle : Transformed_Triangles) {
		triangle->Draw(rst, LightList, Source_Image);
		delete triangle;//Do remember to release the memory allocated
	}
}

float Model::Radians(float rotate) {
	return rotate * EIGEN_PI / 180.0f;
}