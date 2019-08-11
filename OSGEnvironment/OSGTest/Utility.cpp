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
 * 比较两个三维向量是否相同
 */
bool Utility::isVec3Same(osg::Vec3 v1, osg::Vec3 v2)
{
	return (v1.x() == v2.x()) && (v1.y() == v2.y()) && (v1.z() == v2.z());
}

/**
 * 用一个字符替换原字符中的另一个字符
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
 * 从路径中获取文件名（不包括后缀名）
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



