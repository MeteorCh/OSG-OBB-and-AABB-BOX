/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:�����࣬����һЩ���������޹صĶ���Ļ������߲���
**************************************************/
#pragma once
#include <osg/Vec3>
#include <string>
using namespace std;

class Utility
{
public:
	static bool isVec3Same(osg::Vec3 v1, osg::Vec3 v2);//�Ƚ�������ά�����Ƿ����
	static string getFileNameFromPath(string path);//��ģ��·���л�ȡ������
	static void string_replace(std::string &strBig, const std::string &strsrc, const std::string &strdst);
	Utility();
	~Utility();
};

