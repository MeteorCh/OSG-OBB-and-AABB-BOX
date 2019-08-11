/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:Geom是一个建筑物中，不可再分的单独组件，其中存储了组件的顶点信息及三角形信息
**************************************************/
#pragma once
#include <vector>
#include "Vertex.h"
#include "Trianngle.h"
#include <osg/ref_ptr>
#include <osg/Geode>
using namespace std;


class Geom
{
public:
	vector<Vertex*> vertices;//一个geom中所有的顶点信息
	vector<Trianngle*> trianngles;//一个geom中的所有三角形信息
	osg::BoundingBox  boundingBox;//包围盒
	bool isTwoTriangleNeighbor(int triangle1Index,int triangle2Index);//两个三角形是否相邻
	void createTriangleTopo();//创建三角形之间的拓扑关系
	void createVertexTopo();//创建顶点之间的拓扑
	osg::ref_ptr<osg::Geode> createOsgNode(osg::Vec4 color);
	Geom();
	~Geom();
};

