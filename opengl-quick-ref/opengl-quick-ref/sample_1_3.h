#pragma once

#include <iostream>
#include<Windows.h>
#include "GL/glew.h"


//��ɫ����ϸ�÷�
inline void sample_1_3(float* change)
{
	/*
	��������
	vec2 someVec;
	vec4 differentVec = someVec.xyxx;
	vec3 anotherVec = differentVec.zyw;
	vec4 otherVec = someVec.xxxx + anotherVec.yxzy;

	vec2 vect = vec2(0.5, 0.7);
	vec4 result = vec4(vect, 0.0, 0.0);
	vec4 otherResult = vec4(result.xyz, 1.0);
	*/

	/*
	GLSL������in��out�ؼ���ר����ʵ�����Ŀ�ġ�
	ÿ����ɫ��ʹ���������ؼ����趨����������ֻҪһ�������������һ����ɫ���׶ε�����ƥ�䣬���ͻᴫ����ȥ��
	���ڶ����Ƭ����ɫ���л��е㲻ͬ��

	������ɫ�������յ���һ��������ʽ�����룬����ͻ�Ч�ʵ��¡�
	Ϊ�˶��嶥�����ݸ���ι�������ʹ��location��һԪ����ָ������������������ǲſ�����CPU�����ö������ԡ�
	������ɫ����ҪΪ���������ṩһ�������layout��ʶ���������ǲ��ܰ������ӵ��������ݡ�

	Ƭ����ɫ��������Ҫһ��vec4��ɫ�����������ΪƬ����ɫ����Ҫ����һ�������������ɫ��
	�������Ƭ����ɫ��û�ж��������ɫ��OpenGL������������ȾΪ��ɫ�����ɫ����

	������Ǵ����һ����ɫ������һ����ɫ���������ݣ����Ǳ����ڷ��ͷ���ɫ��������һ��������ڽ��շ���ɫ��������һ�����Ƶ����롣
	�����ͺ����ֶ�һ����ʱ��OpenGL�ͻ�������������ӵ�һ������֮����ܷ��������ˣ����������ӳ������ʱ��ɵģ���

	uniform����һ�ִ�CPU�е�Ӧ����GPU�е���ɫ���������ݵķ�ʽ����uniform�Ͷ���������Щ��ͬ��
	���ȣ�uniform��ȫ�ֵ�(Global)��ȫ����ζ��uniform����������ÿ����ɫ����������ж��Ƕ�һ�޶��ģ����������Ա���ɫ�������������ɫ��������׶η��ʡ�
	�ڶ����������uniformֵ���ó�ʲô��uniform��һֱ�������ǵ����ݣ�ֱ�����Ǳ����û���¡�
	*/

	const char *vertexShaderSource =
		"#version 430 core                                                                                            \n"
		"layout(location = 0) in vec3 aPos;   // λ�ñ���������λ��ֵΪ 0 					  \n"
		"layout(location = 1) in vec3 aColor; // ��ɫ����������λ��ֵΪ 1					  \n"
		"																														  \n"
		"out vec3 ourColor; // ��Ƭ����ɫ�����һ����ɫ											  \n"
		"uniform float xOffset;																					  \n"
		"void main()																										  \n"
		"{																														  \n"
		"	gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0);			    			  \n"
		"	ourColor = aColor; // ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ \n"
		"}																														  \0";

	const char * fragmentShaderSource =
		"#version 430 core								       \n"
		"out vec4 FragColor;							       \n"
		"in vec3 ourColor;                                      \n"
		"void main()												   \n"
		"{																	\n"
		"	FragColor = vec4(ourColor, 1.0f);	        \n"
		"} 														            \0";

	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//����λ������  layout(location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//������ɫ����  layout(location = 1)����ʼ����ɫ������3��floatλ������֮�����������������(void*)(3 * sizeof(float))
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	*change += 0.001;
	
	glUseProgram(shaderProgram);
	//����glUseProgram�����Ч
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "xOffset");
	glUniform1f(vertexColorLocation, *change);

	//loop
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

}
