/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:�����࣬�洢�����λ�ã��������꣬����������Ϣ
**************************************************/
#pragma once

#include <osg/Vec3>
#include <osg/Vec2>
#include <osg/Vec4>
#include <vector>
using namespace std;

/**
 * �����࣬��¼ģ���ж���ĸ�����Ϣ�������������ꡢ�����������������
 */
class Vertex
{
public:
	osg::Vec3 coor;//��������
	osg::Vec3 normal;//������
	osg::Vec2 texCoor;//��������
	int index;//�ö����������е��±�
	vector<int> neighborTriangle;//�������ڵ�������
	Vertex();
	~Vertex();
};

