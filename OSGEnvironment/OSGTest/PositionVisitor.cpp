#include "stdafx.h"
#include "PositionVisitor.h"
#include <osg/Drawable>
#include <osg/Geode>
#include <iostream>
#include "Geom.h"
#include <valarray>
#include "Trianngle.h"
#include "Vertex.h"
#include <cstdlib>
#include <ctime>
#include <osg/ComputeBoundsVisitor>

PositionVisitor::PositionVisitor(string ModelName)
{
	this->modelName = ModelName;
	setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
}

PositionVisitor::~PositionVisitor()
{
	for (Geom* geom:allGeom)
	{
		delete geom;
	}
}

void PositionVisitor::apply(osg::Geode& node)
{
	for (size_t i=0;i<node.getNumDrawables();i++)
	{
		osg::ref_ptr<osg::Drawable> drawable = node.getDrawable(i);
		ModelAttributeFunctor functor;
		drawable->accept(functor);
		osg::TriangleIndexFunctor<TriangleIndex> triangleIndex;
		drawable->accept(triangleIndex);
		dealTriangleInfo(functor, triangleIndex);
	}
}

void PositionVisitor::dealTriangleInfo(ModelAttributeFunctor attributeFunctor, osg::TriangleIndexFunctor<TriangleIndex> indexFunctor)
{
	Geom *geom = new Geom;
	if (attributeFunctor.textCoordList->size()!=0
		&&attributeFunctor.textCoordList->size()!=attributeFunctor.vertexList->size())
	{
		cout << "纹理坐标和顶点数量不匹配" << endl;
		return;
	}
	//处理顶点信息
	for (size_t i=0;i<attributeFunctor.vertexList->size();i++)
	{
		Vertex* vertex=new Vertex;
		vertex->coor = attributeFunctor.vertexList->at(i);
		vertex->index = i;
		vertex->normal = attributeFunctor.normalList->at(i);
		if (i< attributeFunctor.textCoordList->size())
			vertex->texCoor = attributeFunctor.textCoordList->at(i);
		geom->vertices.push_back(vertex);
	}
	//处理三角形信息
	for (int i=0;i<indexFunctor.triangleNum;i++)
	{
		Trianngle* trianngle=new Trianngle;
		trianngle->index = i;
		trianngle->vertexIndexs[0] = indexFunctor.indexs->at(i * 3);
		trianngle->vertexIndexs[1] = indexFunctor.indexs->at(i * 3+1);
		trianngle->vertexIndexs[2] = indexFunctor.indexs->at(i * 3+2);
		//计算法向量
		osg::Vec3 edge1 = geom->vertices.at(trianngle->vertexIndexs[1])->coor - geom->vertices.at(trianngle->vertexIndexs[0])->coor;
		osg::Vec3 edge2 = geom->vertices.at(trianngle->vertexIndexs[2])->coor - geom->vertices.at(trianngle->vertexIndexs[0])->coor;
		osg::Vec3 triangleNormal = edge1^edge2;
		triangleNormal.normalize();
		trianngle->normal = triangleNormal;
		geom->trianngles.push_back(trianngle);
	}
	allGeom.push_back(geom);
}

osg::ref_ptr<osg::Node> PositionVisitor::createOsgNode(osg::Vec4 color, int order)
{
	this->geomColor = color;
	short direction = order % 4;
	osg::ref_ptr<osg::Group> result = new osg::Group;
	if (allGeom.size()>0&&allGeom.size()==1)
	{
		osg::ref_ptr<osg::Geode> geode= allGeom[0]->createOsgNode(color);
		this->boundingBox = geode->getBoundingBox();
		result->addChild(geode);
	}
	else
	{
		for (Geom* geom : allGeom)
			result->addChild(geom->createOsgNode(color));
		osg::ComputeBoundsVisitor boundsVisitor;
		result->accept(boundsVisitor);
		this->boundingBox = boundsVisitor.getBoundingBox();
	}
	result->addChild(createTipText(direction));
	return result;
}

osg::ref_ptr<osg::Node> PositionVisitor::createRandomColorOsgNode(int order)
{
	//创建一个随机颜色
	osg::Vec4 color = osg::Vec4(rand()%10*0.1, rand() % 10 * 0.1, rand() % 10 * 0.1, 1.0f);
	this->geomColor = color;
	return createOsgNode(color,order);
}

osg::ref_ptr<osgText::Text> PositionVisitor::createTipText(short direction)
{
	osg::ref_ptr<osgText::Font> font = osgText::readFontFile("fonts/simhei.ttf");
	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	text->setFont(font);//设置字体
	text->setCharacterSize(5);//字体大小

	//对每个组件设置不同的朝向，避免所有的提示文字都在一个朝向
	osg::Vec3 tipPosition;
	float halfX = (boundingBox.xMax() + boundingBox.xMin()) / 2;
	float halfY = (boundingBox.yMax() + boundingBox.yMin()) / 2;
	float halfZ = (boundingBox.zMax() + boundingBox.zMin()) / 2;
	switch (direction)
	{
	case 0://左
		tipPosition =osg::Vec3 (halfX, boundingBox.yMin()-1, halfZ);
		text->setAxisAlignment(osgText::Text::XZ_PLANE);//文字对称方式
		break;
	case 1://右
		tipPosition = osg::Vec3(halfX, boundingBox.yMax()+1, halfZ);
		text->setAxisAlignment(osgText::Text::REVERSED_XZ_PLANE);//文字对称方式
		break;
	case 2://前
		tipPosition = osg::Vec3(boundingBox.xMax()+1, halfY, halfZ);
		text->setAxisAlignment(osgText::Text::YZ_PLANE);//文字对称方式
		break;
	case 3://后
		tipPosition = osg::Vec3(boundingBox.xMin()-1, halfY, halfZ);
		text->setAxisAlignment(osgText::Text::REVERSED_YZ_PLANE);//文字对称方式
		break;
	}
	text->setPosition(tipPosition);//字体位置
	text->setColor(this->geomColor);//字体颜色
	text->setAutoRotateToScreen(false);//跟随视角不断变化，距离越远，文字越小
	text->setBackdropType(osgText::Text::OUTLINE);//文件描边
	text->setBackdropColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));//文字描边的颜色
	text->setDrawMode(osgText::Text::TEXT | osgText::Text::BOUNDINGBOX);//添加文字边框
	text->setText(modelName);
	return text;
}

vector<Geom*> PositionVisitor::getAllGeom()
{
	return this->allGeom;
}





