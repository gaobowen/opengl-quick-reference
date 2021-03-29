#pragma once

#include <iostream>
#include<Windows.h>
#include "GL/glew.h"

//��ɫ�����ƾ���
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

	//����1������6�����㣬2����������ɾ���
	//����2������4�����㣬ͨ��������������2����ͬ��2����������ɾ���
	float vertices[] = {
	0.5f, 0.5f, 0.0f,   // ���Ͻ� index = 0
	0.5f, -0.5f, 0.0f,  // ���½� index = 1
	-0.5f, -0.5f, 0.0f, // ���½� index = 2
	-0.5f, 0.5f, 0.0f   // ���Ͻ� index = 3
	};

	unsigned int indices[] = {
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//�������
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//�����������
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//GL_ELEMENT_ARRAY_BUFFER ������ GL_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//�趨��������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//���
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//�����߶�
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//loop
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	//glDrawElementsǰ����glBindBuffer(GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
	//ʹ��glDrawElements��������
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	

}