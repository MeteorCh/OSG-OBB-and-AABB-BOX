/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:三角形类用来存储一个三角形的信息，包括三个顶点所在的索引，法向量等信息
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
	int vertexIndexs[3];//顶点索引
	osg::Vec3 normal;//法向量
	int index;//该三角形在数组中的索引
	std::vector<int> neighborTriangles;//相邻的三角形的索引
};

