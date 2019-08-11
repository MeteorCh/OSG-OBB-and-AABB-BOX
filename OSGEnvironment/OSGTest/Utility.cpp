#include "stdafx.h"
#include "Utility.h"
#include <iostream>

Utility::Utility()
{
}


Utility::~Utility()
{
}

/**
 * �Ƚ�������ά�����Ƿ���ͬ
 */
bool Utility::isVec3Same(osg::Vec3 v1, osg::Vec3 v2)
{
	return (v1.x() == v2.x()) && (v1.y() == v2.y()) && (v1.z() == v2.z());
}

/**
 * ��һ���ַ��滻ԭ�ַ��е���һ���ַ�
 */
void Utility::string_replace(std::string& strBig, const std::string& strsrc, const std::string& strdst)
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}

/**
 * ��·���л�ȡ�ļ�������������׺����
 */
string Utility::getFileNameFromPath(string path)
{
	if(path.empty())
	{
		return "";
	}
	string_replace(path, "/", "\\");
	std::string::size_type iPos = path.find_last_of('\\') + 1;
	std::string::size_type dPos = path.find_last_of('.') + 1;
	if (dPos == 0)
		dPos = path.length();
	return path.substr(iPos, dPos - iPos-1);
}



