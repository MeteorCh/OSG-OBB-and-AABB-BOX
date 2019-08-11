/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:顶点类，存储顶点的位置，纹理坐标，法向量等信息
**************************************************/
#pragma once

#include <osg/Vec3>
#include <osg/Vec2>
#include <osg/Vec4>
#include <vector>
using namespace std;

/**
 * 顶点类，记录模型中顶点的各类信息，包括顶点坐标、法向量、纹理坐标等
 */
class Vertex
{
public:
	osg::Vec3 coor;//顶点坐标
	osg::Vec3 normal;//法向量
	osg::Vec2 texCoor;//纹理坐标
	int index;//该顶点在数组中的下标
	vector<int> neighborTriangle;//顶点相邻的三角形
	Vertex();
	~Vertex();
};

