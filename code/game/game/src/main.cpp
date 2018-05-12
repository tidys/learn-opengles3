#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

unsigned int VBO, VAO;// vertex buffer object , vertex array object
unsigned int shaderProgram;

// 窗口大小改变回调
void frameSizeCallBack(GLFWwindow* window, int widht, int height) {
	glViewport(0, 0, widht, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
void buildAndCompileShaderProgram() {
	int success;
	char infoLog[512];

	// 顶点着色器
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location=0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "[vertexShader] error: " << infoLog << std::endl;
	}

	// 片段着色器
	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "[fragmentShader] error: " << infoLog << std::endl;
	}
	// 着色器程序
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// 检测错误
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "[shaderProgram] error: " << infoLog << std::endl;
	}

	// 删除着色器
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	*/

	float vertices[] = {
		// 第一个三角形
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, 0.5f, 0.0f,  // 左上角
							// 第二个三角形
							0.5f, -0.5f, 0.0f,  // 右下角
							-0.5f, -0.5f, 0.0f, // 左下角
							-0.5f, 0.5f, 0.0f   // 左上角
	};


	// 顶点着色器
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);// 绑定vao

	// 复制顶点数组到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 设置顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void render() {
	glClearColor(0.2, 0.3, 0.3, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT);

	// 使用着色器渲染物体
	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//核心模式


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "game", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// 注册回调
	glfwSetFramebufferSizeCallback(window, frameSizeCallBack);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to init GLAD!" << std::endl;
	}

	buildAndCompileShaderProgram();

	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		// 检测输入事件
		processInput(window);
		// render
		render();
		glfwPollEvents();
		//交换缓冲区
		glfwSwapBuffers(window);
	}
	// 释放窗口分配的所有资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}


