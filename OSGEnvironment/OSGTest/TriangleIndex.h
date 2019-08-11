/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:����ģ���������������ķ�����
**************************************************/
#pragma once
#include <osg/ref_ptr>
#include <osg/Array>
#include <osg/TriangleIndexFunctor>

class TriangleIndex
{
public:
	osg::ref_ptr<osg::UIntArray> indexs;//���е�����
	int triangleNum;//�����ε�����
	TriangleIndex();
	~TriangleIndex();
	void operator()(const unsigned int& v1, const unsigned int& v2, const unsigned int& v3);
};

