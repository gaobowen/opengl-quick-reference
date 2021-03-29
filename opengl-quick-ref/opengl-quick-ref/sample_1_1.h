#pragma once

#include <iostream>
#include<Windows.h>
#include "GL/glew.h"

//��ɫ������������
inline void sample_1_1()
{

	//������ɫ��
	//ʹ��in�ؼ��֣��ڶ�����ɫ�����������е����붥������(Input Vertex Attribute)
	//����ÿ�����㶼��һ��3D���꣬���Ǿʹ���һ��vec3�������aPos
	//layout (location = 0)�趨�����������λ��ֵ(Location)
	//Ϊ�����ö�����ɫ������������Ǳ����λ�����ݸ�ֵ��Ԥ�����gl_Position����������Ļ����vec4���͵ġ�
	//��main������������ǽ�gl_Position���õ�ֵ���Ϊ�ö�����ɫ���������
	//Ҳ���Ե���������ļ�������
	const char *vertexShaderSource =
		"#version 430 core                                                 \n"
		"layout (location = 0) in vec3 aPos;                       \n"
		"void main()                                                            \n"
		"{                                                                             \n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);  \n"
		"}                                                                             \0";

	//���ر�����ɫ��
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//�Ƿ����ɹ�
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Ƭ����ɫ��
	/*Ƭ����ɫ��ֻ��Ҫһ��������������������һ��4��������������ʾ�������յ������ɫ������Ӧ���Լ�������������
	���������������ʹ��out�ؼ��֣�������������ΪFragColor��
	���棬���ǽ�һ��AlphaֵΪ1.0(1.0������ȫ��͸��)���ٻ�ɫ��vec4��ֵ����ɫ�����*/
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

	//��ɫ������
	//��ɫ���������(Shader Program Object)�Ƕ����ɫ���ϲ�֮������������ɵİ汾��
	//������ɫ�����ڶ������ˣ�ʣ�µ������ǰ�������ɫ���������ӵ�һ��������Ⱦ����ɫ������(Shader Program)�С�
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//��������������
	glUseProgram(shaderProgram);
	//�ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//�����������
	/*
	�����������(Vertex Array Object, VAO)�����񶥵㻺������������󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO�С�
	�����ĺô����ǣ������ö�������ָ��ʱ����ֻ��Ҫ����Щ����ִ��һ�Σ�֮���ٻ��������ʱ��ֻ��Ҫ����Ӧ��VAO�����ˡ�
	��ʹ�ڲ�ͬ�������ݺ���������֮���л���÷ǳ��򵥣�ֻ��Ҫ�󶨲�ͬ��VAO�����ˡ�
	*/
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//���㻺�����(Vertex Buffer Objects, VBO)
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	/*���ڣ������Ѿ������붥�����ݷ��͸���GPU����ָʾ��GPU����ڶ����Ƭ����ɫ���д�������
	�Ϳ�Ҫ����ˣ�����û������OpenGL����֪��������ν����ڴ��еĶ������ݣ��Լ�������ν������������ӵ�������ɫ���������ϡ�
	������Ҫ����OpenGL��ô����
	*/
	//���Ӷ�������
	/*
	 ��һ������ָ������Ҫ���õĶ������ԡ����ǵ������ڶ�����ɫ����ʹ�� layout(location = 0), �Ѷ������Ե�λ��ֵ����Ϊ0��
			��Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
	 �ڶ�������ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
	 ����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��
	 ���ĸ����������Ƿ�ϣ�����ݱ���׼��(Normalize)�������������ΪGL_TRUE���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣���ǰ�������ΪGL_FALSE��
	 �����������������(Stride)�������������������Ķ���������֮��ļ���������¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)��
			Ҫע�������������֪����������ǽ������еģ���������������֮��û�п�϶������Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã���
			һ�������и���Ķ������ԣ����Ǿͱ����С�ĵض���ÿ����������֮��ļ���������ں���ῴ����������ӣ���ע: �����������˼��˵���Ǵ�������Եڶ��γ��ֵĵط�����������0λ��֮���ж����ֽڣ���
	 ������������������void*��������Ҫ���ǽ��������ֵ�ǿ������ת��������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0�����ο�sample_1_3.h��

	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//���VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//���VBO
	glBindVertexArray(0);


	//loop
	//ѭ������ֻ��Ҫ����3������
	//��������������
	glUseProgram(shaderProgram);
	//����VAO
	glBindVertexArray(VAO);
	//����Array
	//
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

