#pragma once

#include <iostream>
#include<Windows.h>
#include "GL/glew.h"

//着色器绘制矩形
inline void sample_1_2()
{

	const char *vertexShaderSource =
		"#version 430 core                                                 \n"
		"layout (location = 0) in vec3 aPos;                       \n"
		"void main()                                                            \n"
		"{                                                                             \n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);  \n"
		"}                                                                             \0";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	const char * fragmentShaderSource =
		"#version 430 core								       \n"
		"out vec4 FragColor;							       \n"
		"void main()												   \n"
		"{																	\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);	\n"
		"} 														            \0";
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//方案1，绘制6个顶点，2个三角形组成矩形
	//方案2，绘制4个顶点，通过索引复用其中2个，同样2个三角形组成矩形
	float vertices[] = {
	0.5f, 0.5f, 0.0f,   // 右上角 index = 0
	0.5f, -0.5f, 0.0f,  // 右下角 index = 1
	-0.5f, -0.5f, 0.0f, // 左下角 index = 2
	-0.5f, 0.5f, 0.0f   // 左上角 index = 3
	};

	unsigned int indices[] = {
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//顶点对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//索引缓冲对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//GL_ELEMENT_ARRAY_BUFFER 区别于 GL_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//设定顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//解绑
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//绘制线段
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//loop
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	//glDrawElements前必须glBindBuffer(GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
	//使用glDrawElements绘制索引
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	

}