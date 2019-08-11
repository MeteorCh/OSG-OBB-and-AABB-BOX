#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/WriteFile>
#include <regex>
#include <windows.h>
#include "PositionVisitor.h"
#include "Utility.h"
#include "OBBbox.h"
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osg/ComputeBoundsVisitor>
#include <osg/Material>
using namespace std;

osg::ref_ptr<osg::Geode> createBoundingBox(osg::Vec3 center,float lenx,float leny, float lenz)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;

	osg::ref_ptr<osg::ShapeDrawable>  drawable = new osg::ShapeDrawable(new osg::Box(center, lenx, leny, lenz));
	drawable->setColor(osg::Vec4(0.0, 1.0, 1.0, 1.0));
	osg::ref_ptr<osg::StateSet> stateset=drawable->getOrCreateStateSet();
	osg::ref_ptr<osg::PolygonMode> polygon = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	stateset->setAttributeAndModes(polygon);
	//设置线宽
	osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth(1.0);
	stateset->setAttribute(linewidth);
	geode->addDrawable(drawable);
	return geode;
}

osg::ref_ptr<osg::Geode> createOBBBox(render_system::OBB obb)
{
	osg::ref_ptr<osg::Vec3Array> point;
	obb.getCorners(point);
	vector<osg::Vec3> obbPnt = point->asVector();
	osg::ref_ptr<osg::Geode> box = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> vertexes = new osg::Vec3Array;
	for (int i = 0; i < 3; i++)
		for (size_t j = 0; j < obbPnt.size(); j++)
			vertexes->push_back(obbPnt[j]);
	geometry->setVertexArray(vertexes);
	//设置索引
	GLuint elements[]={
		//front
		0,1,2,
		0,2,3,
		//back
		6,4,7,
		6,5,4,
		//left
		8,9,14,
		8,14,15,
		//right
		11,12,13,
		11,13,10,
		//top
		16,20,19,
		16,23,20,
		//bottom
		17,22,21,
		17,21,18
	};
	//设置法向量
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	for (int i = 0; i < 4; i++)
		normals->push_back(obb._zAxis);
	for (int i = 0; i < 4; i++)
		normals->push_back(-obb._zAxis);

	normals->push_back(-obb._xAxis);
	normals->push_back(-obb._xAxis);
	for(int i=0;i<4;i++)
		normals->push_back(obb._xAxis);
	normals->push_back(-obb._xAxis);
	normals->push_back(-obb._xAxis);

	normals->push_back(obb._yAxis);
	normals->push_back(-obb._yAxis);
	normals->push_back(-obb._yAxis);
	normals->push_back(obb._yAxis);
	normals->push_back(obb._yAxis);
	normals->push_back(-obb._yAxis);
	normals->push_back(-obb._yAxis);
	normals->push_back(obb._yAxis);

	geometry->setNormalArray(normals,osg::Array::BIND_PER_VERTEX);
	osg::ref_ptr<osg::Vec4Array> colorArray = new osg::Vec4Array;
	colorArray->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	geometry->setColorArray(colorArray, osg::Array::BIND_OVERALL);
	osg::ref_ptr<osg::StateSet> stateset= geometry->getOrCreateStateSet();
	osg::ref_ptr<osg::PolygonMode> polygon = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	stateset->setAttributeAndModes(polygon);
	//设置线宽
	osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth(1.0);
	stateset->setAttribute(linewidth);
	int pntNum = sizeof(elements) / sizeof(GLuint);
	osg::ref_ptr<osg::DrawElementsUInt> elementsArray = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, pntNum,elements);
	geometry->addPrimitiveSet(elementsArray);
	box->addDrawable(geometry);
	return box;
}

int main()
{
	string filePath="Model/31_6.obj";
	osgViewer::Viewer viewer;
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<osgDB::Options> options = new osgDB::Options("noRotation");
	osg::ref_ptr<osg::Group> node =(osg::Group*)osgDB::readNodeFile(filePath, options);
	string modelName = Utility::getFileNameFromPath(filePath);
	PositionVisitor visitor = PositionVisitor(modelName);
	node->accept(visitor);
	//OBB包围盒
	for (Geom* geom:visitor.getAllGeom())
	{
		osg::ref_ptr<osg::Vec3Array> array = new osg::Vec3Array;
		for (Vertex* vertex : geom->vertices)
			array->push_back(vertex->coor);
		render_system::OBB obb = render_system::OBB(array);
		root->addChild(createOBBBox(obb));
	}
	//AABB包围盒
	for (size_t i=0;i<node->getNumChildren();i++)
	{
		osg::ref_ptr<osg::Node> child = node->getChild(i);
		osg::ComputeBoundsVisitor boundvisitor;
		child->accept(boundvisitor);
		osg::BoundingBox box = boundvisitor.getBoundingBox();
		root->addChild(createBoundingBox(box.center(), box.xMax() - box.xMin(), box.yMax() - box.yMin(), box.zMax() - box.zMin()));
	}
	osg::ref_ptr<osg::StateSet> stateset=node->getOrCreateStateSet();
	osg::ref_ptr<osg::Material> mat = new osg::Material();
	mat->setColorMode(osg::Material::ColorMode::DIFFUSE);
	stateset->setAttribute(mat);
	root->addChild(node);
	viewer.setSceneData(root);
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.setUpViewOnSingleScreen(1);//这里是单屏幕显示
	viewer.run();
	return 0;
}
