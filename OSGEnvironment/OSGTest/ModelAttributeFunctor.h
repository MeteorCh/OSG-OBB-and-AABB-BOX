/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:访问模型中所有信息的访问器
**************************************************/
#pragma once
#include <osg/Drawable>

class ModelAttributeFunctor
	:public osg::Drawable::AttributeFunctor
{
public:
	osg::ref_ptr<osg::Vec3Array> vertexList;//存储顶点的数组
	osg::ref_ptr<osg::Vec3Array> normalList;//存储法向量
	osg::ref_ptr<osg::Vec2Array> textCoordList;//纹理坐标
	virtual void apply(osg::Drawable::AttributeType, unsigned, osg::Vec2*) override;
	virtual void apply(osg::Drawable::AttributeType, unsigned, osg::Vec3*) override;
	ModelAttributeFunctor();
	~ModelAttributeFunctor();
};

