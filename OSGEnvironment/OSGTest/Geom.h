/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:Geom��һ���������У������ٷֵĵ�����������д洢������Ķ�����Ϣ����������Ϣ
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
	vector<Vertex*> vertices;//һ��geom�����еĶ�����Ϣ
	vector<Trianngle*> trianngles;//һ��geom�е�������������Ϣ
	osg::BoundingBox  boundingBox;//��Χ��
	bool isTwoTriangleNeighbor(int triangle1Index,int triangle2Index);//�����������Ƿ�����
	void createTriangleTopo();//����������֮������˹�ϵ
	void createVertexTopo();//��������֮�������
	osg::ref_ptr<osg::Geode> createOsgNode(osg::Vec4 color);
	Geom();
	~Geom();
};

