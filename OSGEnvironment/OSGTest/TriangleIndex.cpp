#include "stdafx.h"
#include "TriangleIndex.h"
#include <iostream>


TriangleIndex::TriangleIndex()
{
	indexs = new osg::UIntArray;
	triangleNum = 0;
}


TriangleIndex::~TriangleIndex()
{
}

void TriangleIndex::operator()(const unsigned& v1, const unsigned& v2, const unsigned& v3)
{
	if (v1 == v2 || v1 == v3 || v2 == v3)
		return;
	indexs->push_back(v1);
	indexs->push_back(v2);
	indexs->push_back(v3);
	triangleNum++;
}

