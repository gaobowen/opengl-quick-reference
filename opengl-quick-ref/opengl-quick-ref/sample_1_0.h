#pragma once

#include<Windows.h>
#include "GL/glew.h"

//���õ����������������
inline void sample_1_0()
{

	/*OpenGL����3D������3���ᣨx��y��z���϶�Ϊ [-1.0, 1.0] �ķ�Χ��ʱ�Ŵ�������
	��������ν�ı�׼���豸����(Normalized Device Coordinates)��Χ�ڵ�����Ż����ճ�������Ļ�ϣ��������Χ��������궼������ʾ����*/
	glPushMatrix();
	glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 1.0f); //x=0.0 Ϊ��Ļˮƽ����1/2��, y=1.0 Ϊ��Ļ������ ����Ļ����1/2����
	glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(0.87f, -0.5f);//����
	glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(-0.87f, -0.5f);//����
	glEnd();
	glPopMatrix();

}