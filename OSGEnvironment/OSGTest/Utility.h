/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:工具类，进行一些与其他类无关的额外的基础工具操作
**************************************************/
#pragma once
#include <osg/Vec3>
#include <string>
using namespace std;

class Utility
{
public:
	static bool isVec3Same(osg::Vec3 v1, osg::Vec3 v2);//比较两个三维向量是否相等
	static string getFileNameFromPath(string path);//从模型路径中获取明名称
	static void string_replace(std::string &strBig, const std::string &strsrc, const std::string &strdst);
	Utility();
	~Utility();
};

