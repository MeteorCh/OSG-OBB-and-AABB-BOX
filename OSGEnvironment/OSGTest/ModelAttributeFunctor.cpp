#include "stdafx.h"
#include "ModelAttributeFunctor.h"
#include <iostream>
using namespace std;


ModelAttributeFunctor::ModelAttributeFunctor()
{
	vertexList = new osg::Vec3Array;
	normalList = new osg::Vec3Array;
	textCoordList = new osg::Vec2Array;
}


ModelAttributeFunctor::~ModelAttributeFunctor()
{
}

void ModelAttributeFunctor::apply(osg::Drawable::AttributeType type, unsigned size, osg::Vec2* front)
{
	if (type==osg::Drawable::TEXTURE_COORDS_0)
	{
		for (unsigned i=0;i<size;i++)
		{
			textCoordList->push_back(*(front + i));
		}
	}
}

void ModelAttributeFunctor::apply(osg::Drawable::AttributeType type, unsigned size, osg::Vec3* front)
{
	if (type == osg::Drawable::VERTICES)
	{
		for (unsigned i = 0; i<size; i++)
		{
			vertexList->push_back(*(front + i));
		}
	}
	else if (type == osg::Drawable::NORMALS)
	{
		for (unsigned i = 0; i<size; i++)
		{
			normalList->push_back(*(front + i));
		}
	}
}

