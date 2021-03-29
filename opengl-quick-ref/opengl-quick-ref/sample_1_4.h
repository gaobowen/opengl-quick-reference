#pragma once

#include <iostream>
#include<Windows.h>
#include "GL/glew.h"
#include "std_image.h"


inline void sample_1_4()
{
	/* s => x-axis, t => y-axis, r => z-axis */
	/*
		����������x��y���ϣ���ΧΪ0��1֮�䣨ע������ʹ�õ���2D����ͼ�񣩡�
		����������ʼ��(0, 0)��Ҳ��������ͼƬ�����½ǣ���ʼ��(1, 1)��������ͼƬ�����Ͻǡ�
		ʹ�����������ȡ������ɫ��������(Sampling)��

	*/

	//һ����������3������
	const char *vertexShaderSource =
		"#version 430 core                                                 \n"
		"layout (location = 0) in vec3 aPos;						\n"
		"layout (location = 1) in vec3 aColor;						\n"
		"layout (location = 2) in vec2 aTexCoord;				\n"
		"																				\n"
		"out vec3 ourColor;													\n"
		"out vec2 TexCoord;												\n"
		"																				\n"
		"void main()																\n"
		"{																				\n"
		"	gl_Position = vec4(aPos, 1.0);								\n"
		"	ourColor = aColor;												\n"
		"	TexCoord = vec2(aTexCoord.x, aTexCoord.y);	\n"
		"}                                                                              \0";

	/*
		����ܻ����Ϊʲôsampler2D�����Ǹ�uniform������ȴ����glUniform������ֵ��
	ʹ��glUniform1i�����ǿ��Ը��������������һ��λ��ֵ�������Ļ������ܹ���һ��Ƭ����ɫ�������ö������
	һ�������λ��ֵͨ����Ϊһ������Ԫ(Texture Unit)��һ�������Ĭ������Ԫ��0������Ĭ�ϵļ�������Ԫ�����Խ̳�ǰ�沿������û�з���һ��λ��ֵ��

	���ǻ����԰ѵõ���������ɫ�붥����ɫ��ϣ�����ø���Ȥ��Ч����
	����ֻ���������ɫ�붥����ɫ��Ƭ����ɫ�����������϶��ߵ���ɫ
	FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);

	mix ������ֵ  ��ʾ�ڶ���������Ȩ��

	*/
	//����������
	const char * fragmentShaderSource =
		" #version 430 core                                                                                                               \n"
		" out vec4 FragColor;																											  \n"
		" 																																			  \n"
		" in vec3 ourColor;																												  \n"
		" in vec2 TexCoord;																												  \n"
		" 																																			  \n"
		" 																																			  \n"
		" // texture samplers																											  \n"
		" uniform sampler2D texture1;																							  \n"
		" uniform sampler2D texture2;																							  \n"
		" uniform sampler2D texture3;                                                                                             \n"
		" 																																			  \n"
		" void main()																														  \n"
		" {																																			  \n"
		" 	// linearly interpolate between both textures																	  \n"
		"  // vec4 clr_tex1=texture2D(texture1, TextCoord);                                                             \n"
		" 	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);         \n"
		"   FragColor = texture(texture1, TexCoord) * texture(texture3, TexCoord).a;                     \n"
		" }																																			  \0";


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

	//texture coords ����0, 0 Ϊ���½�, 1, 1Ϊ���Ͻǡ����½�0, 0 ���ó� 0, 1 x�ᷭת180�ȡ�
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // top left 
	};
	//������������ɾ���
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	unsigned int VBO;
	unsigned int* pVAO = (unsigned int*)malloc(sizeof(unsigned int));
	unsigned int* pEBO = (unsigned int*)malloc(sizeof(unsigned int));
	glGenVertexArrays(1, pVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, pEBO);

	glBindVertexArray(*pVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *pEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���ݶ������ݽṹ����ƫ�ƺͲ���
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	
	//��������
	unsigned int texture1;
	glGenTextures(1, &texture1);
	//��������ǰ����������
	glBindTexture(GL_TEXTURE_2D, texture1);
	// ����ƽ��ģʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//��������ģʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int imgw, imgh, nrChannels;
	unsigned char *data = stbi_load("D:\\github\\opengl-quick-reference\\opengl-quick-ref\\resources\\textures\\container.jpg", &imgw, &imgh, &nrChannels, 0);
	/*
	//Ϊ��ǰ���� ������ͼ
	��һ������ָ��������Ŀ��(Target)������ΪGL_TEXTURE_2D��ζ�Ż������뵱ǰ�󶨵����������ͬһ��Ŀ���ϵ������κΰ󶨵�GL_TEXTURE_1D��GL_TEXTURE_3D���������ܵ�Ӱ�죩��
	�ڶ�������Ϊ����ָ���༶��Զ����ļ��������ϣ�������ֶ�����ÿ���༶��Զ����ļ���Ļ�������������0��Ҳ���ǻ�������
	��������������OpenGL����ϣ����������Ϊ���ָ�ʽ�����ǵ�ͼ��ֻ��RGBֵ���������Ҳ��������ΪRGBֵ��
	���ĸ��͵���������������յ�����Ŀ�Ⱥ͸߶ȡ�����֮ǰ����ͼ���ʱ�򴢴������ǣ���������ʹ�ö�Ӧ�ı�����
	����������Ӧ�����Ǳ���Ϊ0����ʷ���������⣩��
	���ߵڰ˸�����������Դͼ�ĸ�ʽ���������͡�ʹ��RGBֵ�������ͼ��( pngΪRBGA )���������Ǵ���Ϊchar(byte)���飬���ǽ��ᴫ���Ӧֵ��
	���һ��������������ͼ�����ݵ�ָ�롣
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgw, imgh, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	//����2 ͬ��
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("D:\\github\\opengl-quick-reference\\opengl-quick-ref\\resources\\textures\\awesomeface.png", &imgw, &imgh, &nrChannels, 0);
	//png��alphaͨ������Ҫע��
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgw, imgh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	unsigned char alpha[] = {
		0xff, 0xff, 0xff, 0xff,
		0xff, 0x00, 0x00, 0x88,
		0xff, 0x00, 0x00, 0x88,
		0xff, 0xff, 0xff, 0xff,
	};

	unsigned int texture3;
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 4, 4, 0, GL_ALPHA, GL_UNSIGNED_BYTE, alpha);
	glGenerateMipmap(GL_TEXTURE_2D);



	glUseProgram(shaderProgram);
	//��������0  ������16����Ԫ�ɹ�ѡ��
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//uniform sampler2D texture1; ����Ƭ����ɫ���� ��������texture1 Ϊ0�ŵ�Ԫ����GL_TEXTURE0��
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture3"), 2);

	//����
	glBindVertexArray(*pVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *pEBO); //glDrawElementsǰ����glBindBuffer(GL_ELEMENT_ARRAY_BUFFER
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//����
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	glDeleteVertexArrays(1, pVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, pEBO);

	/*
	�򵥵�ȫ�ֱ任

	#version 430 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
    
    out vec2 TexCoord;
    
    uniform mat4 transform;
    
    void main()
    {
    	gl_Position = transform * vec4(aPos, 1.0f);
    	TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
    }

		glm::mat4 trans = glm::mat4(1.0f)
		trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	*/

	/*
	glPixelStorei(GL_UNPACK_ALIGNMENT��4) ��֤ÿһ������4�ֽڶ���
	��һ�����ĸ��������һ������
	��������� GL_RGB,  GL_RGBA��GL_RED��ֻ��ȡ���صĺ�ɫ����.
	���������� GL_UNSIGNED_BYTE GL_FLOAT
	���߸�������ʾһ������ָ��
	glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
	
	*/
}


