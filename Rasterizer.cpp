#include "Rasterizer.h"
#include "Scene.h"

Rasterizer::Rasterizer(int width, int height) {
	Screen_Width = width;
	Screen_Height = height;
	Pixel_Color.resize(width * height);
	Depth_Buffer.resize(width * height);
	Set_Backgroud_Color();

	for (int i = 0; i < Pixel_Color.size(); i++) {
		Pixel_Color[i] = Eigen::Vector3f(0.0f, 0.0f, 0.0f);//Black is the default color
	}
	for (int j = 0; j < Depth_Buffer.size(); j++) {
		Depth_Buffer[j] = -std::numeric_limits<float>::infinity();
	}
}

void Rasterizer::Set_Backgroud_Color(Eigen::Vector3f color) {
	Background_Color = color;
	for (int i = 0; i < Pixel_Color.size(); i++) {
		Pixel_Color[i] = Background_Color;
	}
}

void Rasterizer::Render_Frame(Scene* Scene_Layout) {
	Clear_Color();
	Clear_Depth();
	Scene_Layout->Draw(*this);
}

/*OpenGL only accept normalized values,
* so we need to convert the pixel coordinates from screen space to canonical space*/
void Rasterizer::Convert_To_Array(float*& target) {
	//Screen_Width * Screen_Height is pixel numbers,
	//and 2 floats for 1 pixel coordinates, 3 for 1 pixel color
	target = new float[Screen_Width * Screen_Height * 5];
	for (int j = 0; j < Screen_Height; j++) {
		for (int i = 0; i < Screen_Width; i++) {
			int location = (i + j * Screen_Width) * 5;
			target[location] = ((float)i - (((float)Screen_Width - 1.0f) / 2.0f)) * 2.0f / (float)Screen_Width;
			//Because in OpenGL the y-axis of image is from top to bottom, so we need to flip the image
			target[location + 1] = ((float)(Screen_Height - j - 1) - (((float)Screen_Height - 1.0f) / 2.0f)) * 2.0f / (float)Screen_Height;
			target[location + 2] = Pixel_Color[i + j * Screen_Width].x();
			target[location + 3] = Pixel_Color[i + j * Screen_Width].y();
			target[location + 4] = Pixel_Color[i + j * Screen_Width].z();
		}
	}
}

void Rasterizer::Clear_Color() {
	for (int i = 0; i < Pixel_Color.size(); i++) {
		Pixel_Color[i] = Background_Color;
	}
}

void Rasterizer::Clear_Depth() {
	for (int i = 0; i < Depth_Buffer.size(); i++) {
		Depth_Buffer[i] = -std::numeric_limits<float>::infinity();
	}
}