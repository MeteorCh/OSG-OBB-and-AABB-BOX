/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:����ģ�ͽڵ�ķ�����
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
	vector<Geom*> allGeom;//���е�geom
	osg::Vec4 geomColor;//geom����ɫ
	string modelName;//ģ������
	osg::BoundingBox boundingBox;//��Χ��
public:
	virtual  void apply(osg::Geode& node) override;
	void dealTriangleInfo(ModelAttributeFunctor attributeFunctor,osg::TriangleIndexFunctor<TriangleIndex> indexFunctor);//����������õ�����Ϣ�����������ι�ϵ
	osg::ref_ptr<osg::Node> createOsgNode(osg::Vec4 color,int order);//����ָ������ɫ����geom�е����ݴ�����osg�ڵ�
	osg::ref_ptr<osg::Node> createRandomColorOsgNode(int order);//��geom�е����ݴ�����osg�ڵ㣬��ɫ���
	osg::ref_ptr<osgText::Text> createTipText(short direction);//������ʾ����
	PositionVisitor(string ModelName);
	vector<Geom*> getAllGeom();
	~PositionVisitor();
};

