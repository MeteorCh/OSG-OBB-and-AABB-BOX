/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:�������������洢һ�������ε���Ϣ�����������������ڵ�����������������Ϣ
**************************************************/
#pragma once
#include <osg/Vec3>
#include <vector>
#include <osg/Vec4>

class Trianngle
{
public:
	void init();
	Trianngle();
	~Trianngle();
	int vertexIndexs[3];//��������
	osg::Vec3 normal;//������
	int index;//���������������е�����
	std::vector<int> neighborTriangles;//���ڵ������ε�����
};

