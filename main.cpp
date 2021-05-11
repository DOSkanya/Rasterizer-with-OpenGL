#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Rasterizer.h"
#include "Scene.h"
#include "Shader.h"

void ProcessInput(GLFWwindow* window, Scene& layout);

static float angle = 0.0f;

int main() {
	Rasterizer rst(1000, 1000);
	Scene layout;
	Model HumanFace("resource/african_head/african_head.obj");
	Model HumanFace_Eye("resource/african_head/african_head_eye_inner.obj");
	HumanFace.Load_Texture("resource/african_head/african_head_diffuse.bmp");
	HumanFace_Eye.Load_Texture("resource/african_head/african_head_eye_inner_diffuse.bmp");
	HumanFace.Set_Model_Matrix(1.0f, angle, Eigen::Vector3f(0.0f, 0.0f, -2.0f));
	HumanFace_Eye.Set_Model_Matrix(1.0f, angle, Eigen::Vector3f(0.0f, 0.0f, -2.0f));
	layout.ModelList.push_back(HumanFace);
	layout.ModelList.push_back(HumanFace_Eye);

	Light light;
	layout.LightList.push_back(light);

	layout.Set_Perspective_Matrix(45.0f, (float)rst.Screen_Width / (float)rst.Screen_Height, -0.1f, -100.0f);
	layout.Set_Viewport_Matrix(rst.Screen_Width, rst.Screen_Height);

	rst.Set_Backgroud_Color();
	rst.Render_Frame(&layout);
	float *Rendering_Result;
	rst.Convert_To_Array(Rendering_Result);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(rst.Screen_Width, rst.Screen_Height, "Output", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create OpenGL window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load GLAD." << std::endl;
		return -1;
	}

	unsigned VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, rst.Screen_Width * rst.Screen_Height * 5 * sizeof(float), Rendering_Result, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader CPU_Shader("D:/CG Tool/Library and Headfile/Shader/CPU_Shader.vs", "D:/CG Tool/Library and Headfile/Shader/CPU_Shader.fs");

	//glViewport(0, 0, rst.Screen_Width, rst.Screen_Height);
	while (!glfwWindowShouldClose(window)) {
		ProcessInput(window, layout);
		rst.Render_Frame(&layout);
		rst.Convert_To_Array(Rendering_Result);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, rst.Screen_Width * rst.Screen_Height * 5 * sizeof(float), Rendering_Result, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		delete[] Rendering_Result;//Prevent memory leak


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		CPU_Shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, rst.Screen_Width * rst.Screen_Height);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void ProcessInput(GLFWwindow* window, Scene& layout) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		angle = angle + 1.0f;
		layout.ModelList[0].Set_Model_Matrix(1.0f, angle, Eigen::Vector3f(0.0f, 0.0f, -2.0f));
		layout.ModelList[1].Set_Model_Matrix(1.0f, angle, Eigen::Vector3f(0.0f, 0.0f, -2.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		angle = angle - 1.0f;
		layout.ModelList[0].Set_Model_Matrix(1.0f, angle, Eigen::Vector3f(0.0f, 0.0f, -2.0f));
		layout.ModelList[1].Set_Model_Matrix(1.0f, angle, Eigen::Vector3f(0.0f, 0.0f, -2.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		angle = 0.0f;
		layout.ModelList[0].Set_Model_Matrix(1.0f, angle, Eigen::Vector3f(0.0f, 0.0f, -2.0f));
		layout.ModelList[1].Set_Model_Matrix(1.0f, angle, Eigen::Vector3f(0.0f, 0.0f, -2.0f));
	}
}