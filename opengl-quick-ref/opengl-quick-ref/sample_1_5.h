#pragma once


#include <iostream>
#include<Windows.h>
#include "GL/glew.h"

inline void sample_1_5()
{
	/*
	һ������������άshared����ô���������浽�ض���λ�ã��Ӷ���ͬһ�����ع����������м�����ɫ���ɼ���

	*/

	const GLchar* Program = " \
    #version 430\n\
    layout (local_size_x = 16, local_size_y = 1) in;\n\
    layout (r32f, binding = 0) uniform image1D in_array; \n\
    layout (r32f, binding = 1) uniform image1D out_array; \n\
    \
    void main() \n\
    { \
        int pos = int(gl_GlobalInvocationID.x);\n\
        vec4 value = imageLoad(in_array, pos);\n\
        value.x += 1.0f;\n\
        imageStore(out_array, pos, value);\n\
    } \
";


}