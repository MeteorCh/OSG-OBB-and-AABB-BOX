#include "stdafx.h"
#include "Geom.h"
#include <iostream>
#include <queue>
#include "Utility.h"
#include <osg/Geometry>
using namespace std;

Geom::Geom()
{
}

Geom::~Geom()
{
	for (Vertex* vertex : vertices)
		delete vertex;
	for (Trianngle* trianngle : trianngles)
		delete trianngle;
}

/**
 * 判断两个三角形是否相邻
 */
bool Geom::isTwoTriangleNeighbor(int triangle1Index, int triangle2Index)
{
	Trianngle* trianngle1 = trianngles.at(triangle1Index);
	Trianngle* trianngle2 = trianngles.at(triangle2Index);

	osg::Vec3 pnt11 = vertices.at(trianngle1->vertexIndexs[0])->coor;
	osg::Vec3 pnt12 = vertices.at(trianngle1->vertexIndexs[1])->coor;
	osg::Vec3 pnt13 = vertices.at(trianngle1->vertexIndexs[2])->coor;

	osg::Vec3 pnt21 = vertices.at(trianngle2->vertexIndexs[0])->coor;
	osg::Vec3 pnt22 = vertices.at(trianngle2->vertexIndexs[1])->coor;
	osg::Vec3 pnt23 = vertices.at(trianngle2->vertexIndexs[2])->coor;

	if ((Utility::isVec3Same(pnt11, pnt21) && Utility::isVec3Same(pnt12, pnt22))//第一条边
		|| (Utility::isVec3Same(pnt11, pnt22) && Utility::isVec3Same(pnt12, pnt21))
		|| (Utility::isVec3Same(pnt11, pnt22) && Utility::isVec3Same(pnt12, pnt23))//第二条边
		|| (Utility::isVec3Same(pnt11, pnt23) && Utility::isVec3Same(pnt12, pnt22))
		|| (Utility::isVec3Same(pnt11, pnt21) && Utility::isVec3Same(pnt12, pnt23))//第三条边
		|| (Utility::isVec3Same(pnt11, pnt23) && Utility::isVec3Same(pnt12, pnt21))

		|| (Utility::isVec3Same(pnt12, pnt21) && Utility::isVec3Same(pnt13, pnt22))//第一条边
		|| (Utility::isVec3Same(pnt12, pnt22) && Utility::isVec3Same(pnt13, pnt21))
		|| (Utility::isVec3Same(pnt12, pnt22) && Utility::isVec3Same(pnt13, pnt23))//第二条边
		|| (Utility::isVec3Same(pnt12, pnt23) && Utility::isVec3Same(pnt13, pnt22))
		|| (Utility::isVec3Same(pnt12, pnt21) && Utility::isVec3Same(pnt13, pnt23))//第三条边
		|| (Utility::isVec3Same(pnt12, pnt23) && Utility::isVec3Same(pnt13, pnt21))

		|| (Utility::isVec3Same(pnt11, pnt21) && Utility::isVec3Same(pnt13, pnt22))//第一条边
		|| (Utility::isVec3Same(pnt11, pnt22) && Utility::isVec3Same(pnt13, pnt21))
		|| (Utility::isVec3Same(pnt11, pnt22) && Utility::isVec3Same(pnt13, pnt23))//第二条边
		|| (Utility::isVec3Same(pnt11, pnt23) && Utility::isVec3Same(pnt13, pnt22))
		|| (Utility::isVec3Same(pnt11, pnt21) && Utility::isVec3Same(pnt13, pnt23))//第三条边
		|| (Utility::isVec3Same(pnt11, pnt23) && Utility::isVec3Same(pnt13, pnt21)))
		return true;
	return  false;
}

/**
 * 创建模型的三角形之间的拓扑
 */
void Geom::createTriangleTopo()
{
	cout << "开始创建三角形之间的拓扑关系：" << endl;
	for (size_t i = 0; i<trianngles.size(); ++i)
	{
		Trianngle* triannglei = trianngles.at(i);
		for (size_t j = i+1; j<trianngles.size(); ++j)
		{
			Trianngle* triannglej = trianngles.at(j);
			if (isTwoTriangleNeighbor(i, j))
			{
				triannglei->neighborTriangles.push_back(j);
				triannglej->neighborTriangles.push_back(i);
			}
		}
		cout << "\t当前进度" << int(i*100.0 / trianngles.size()) << "%\r";
	}
	cout << endl;
}

/**
 * 创建顶点之间的拓扑
 */
void Geom::createVertexTopo()
{
	//点周围的三角形
	for (size_t i=0;i<trianngles.size();++i)
	{
		Vertex *vertex1 =(Vertex*) vertices.at(trianngles.at(i)->vertexIndexs[0]);
		vertex1->neighborTriangle.push_back(i);
		Vertex *vertex2 = (Vertex*)vertices.at(trianngles.at(i)->vertexIndexs[1]);
		vertex2->neighborTriangle.push_back(i);
		Vertex *vertex3 = (Vertex*)vertices.at(trianngles.at(i)->vertexIndexs[2]);
		vertex3->neighborTriangle.push_back(i);
		cout << "	点周围的三角形：" << int(i*1.0 / trianngles.size() * 100) << "%\r";
	}
}

/**
 * 将Geom中的数据创建成osg节点
 */
osg::ref_ptr<osg::Geode> Geom::createOsgNode(osg::Vec4 color)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
	//顶点、法向量
	osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normalArray = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> colorArray = new osg::Vec4Array;
	for (Vertex* vertex : vertices)
	{
		vertexArray->push_back(vertex->coor);
		normalArray->push_back(vertex->normal);
	}
	//颜色
	colorArray->push_back(color);
	//索引
	osg::ref_ptr<osg::DrawElementsUInt> indexs = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	for (Trianngle* trianngle : trianngles)
	{
		indexs->push_back(trianngle->vertexIndexs[0]);
		indexs->push_back(trianngle->vertexIndexs[1]);
		indexs->push_back(trianngle->vertexIndexs[2]);
	}
	geometry->setVertexArray(vertexArray);
	geometry->setNormalArray(normalArray, osg::Array::BIND_PER_VERTEX);
	geometry->setColorArray(colorArray, osg::Array::BIND_OVERALL);
	geometry->addPrimitiveSet(indexs);
	geode->addDrawable(geometry);
	return geode;
}










