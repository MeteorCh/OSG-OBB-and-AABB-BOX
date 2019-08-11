/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:访问模型节点的访问器
**************************************************/
#pragma once
#include <osg/NodeVisitor>
#include <vector>
#include "ModelAttributeFunctor.h"
#include <osg/TriangleIndexFunctor>
#include "TriangleIndex.h"
#include "Geom.h"
#include <osgText/Text>
using namespace std;
class PositionVisitor
	:public osg::NodeVisitor
{
protected:
	vector<Geom*> allGeom;//所有的geom
	osg::Vec4 geomColor;//geom的颜色
	string modelName;//模型名称
	osg::BoundingBox boundingBox;//包围盒
public:
	virtual  void apply(osg::Geode& node) override;
	void dealTriangleInfo(ModelAttributeFunctor attributeFunctor,osg::TriangleIndexFunctor<TriangleIndex> indexFunctor);//处理访问器得到的信息，构建三角形关系
	osg::ref_ptr<osg::Node> createOsgNode(osg::Vec4 color,int order);//根据指定的颜色，将geom中的数据创建成osg节点
	osg::ref_ptr<osg::Node> createRandomColorOsgNode(int order);//将geom中的数据创建成osg节点，颜色随机
	osg::ref_ptr<osgText::Text> createTipText(short direction);//创建提示文字
	PositionVisitor(string ModelName);
	vector<Geom*> getAllGeom();
	~PositionVisitor();
};

