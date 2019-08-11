/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:遍历模型中三角形索引的访问器
**************************************************/
#pragma once
#include <osg/ref_ptr>
#include <osg/Array>
#include <osg/TriangleIndexFunctor>

class TriangleIndex
{
public:
	osg::ref_ptr<osg::UIntArray> indexs;//所有的索引
	int triangleNum;//三角形的数量
	TriangleIndex();
	~TriangleIndex();
	void operator()(const unsigned int& v1, const unsigned int& v2, const unsigned int& v3);
};

