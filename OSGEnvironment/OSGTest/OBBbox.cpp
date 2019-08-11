#include "stdafx.h"
#include "OBBbox.h"

#define ROTATE(a,i,j,k,l) g=a(i,j); h=a(k, l); a(i, j)=(float)(g-s*(h+g*tau)); a(k, l)=(float)(h+s*(g-h*tau));

namespace render_system
{

	static osg::Matrix transpose(const osg::Matrix& b)
	{
		osg::Matrix mat;
		//mat.inverse(b);
		//mat.invert(b);
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				mat(i, j) = b(j, i);
			}
		}
 		return mat;
	}

	
	osg::ref_ptr<osg::Vec3Array> getPointsPolarCorner(const osg::ref_ptr<osg::Vec3Array>& vertPos, osg::Vec3& ptStart, osg::Vec3& ptEnd)
	{
		osg::ref_ptr<osg::Vec3Array> ptPollar = new osg::Vec3Array;
		if (!vertPos.valid() || vertPos->size() < 1)
		{
			return ptPollar;
		}
		//0、1 x: max/min
		//2、3 y: max/min
		//4、5 z: max/min
		ptPollar->push_back(vertPos->at(0));
		ptPollar->push_back(vertPos->at(0));
		ptPollar->push_back(vertPos->at(0));
		ptPollar->push_back(vertPos->at(0));
		ptPollar->push_back(vertPos->at(0));
		ptPollar->push_back(vertPos->at(0));

		for (osg::Vec3Array::size_type i=1; i<vertPos->size(); i++)
		{
			osg::Vec3& ptcur = vertPos->at(i);
			if (ptcur.x() > ptPollar->at(0).x())  ptPollar->at(0) = ptcur;
			if (ptcur.x() < ptPollar->at(1).x())  ptPollar->at(1) = ptcur;

			if (ptcur.y() > ptPollar->at(2).y())  ptPollar->at(2) = ptcur;
			if (ptcur.y() < ptPollar->at(3).y())  ptPollar->at(3) = ptcur;

			if (ptcur.z() > ptPollar->at(4).z())  ptPollar->at(4) = ptcur;
			if (ptcur.z() < ptPollar->at(5).z())  ptPollar->at(5) = ptcur;
		}

		double dDis = 0.0;

		ptStart = ptPollar->at(0);
		ptEnd = ptStart;
		for (int i=0; i<6; i++)
		{
			for (int j=0; j<6; j++)
			{
				float ssss = (ptPollar->at(i)-ptPollar->at(j)).length();
				if ( ssss > dDis)
				{
					dDis = ssss;
					ptStart = ptPollar->at(i);
					ptEnd = ptPollar->at(j);
				}
			}
		}
		return ptPollar;
	}

	//中心点，以对角线为方向的平面
	void calcPlaneEquation(const osg::Vec3& ptStart, const osg::Vec3& ptEnd, float& A, float& B,float& C, float& D)
	{
		osg::Vec3 ptCenter = (ptEnd + ptStart)/2.0;
		osg::Vec3 vDir = ptEnd - ptStart;
		vDir.normalize();

		A = vDir.x();
		B = vDir.y();
		C = vDir.z();
		D = -(ptCenter.x()*A + ptCenter.y()*B + ptCenter.z()*C);
	}

	//点到平面上的投影
	osg::Vec3 pointProjectToPlane(const osg::Vec3 &pt, float A, float B,float C, float D)
	{
		float t = (A*pt.x() + B*pt.y() + C*pt.z() + D)/(A*A + B*B + C*C);
		return osg::Vec3(pt.x()-A*t, pt.y()-B*t, pt.z()-C*t);
	}

	//点到线的投影
	osg::Vec3 pointProjectToLine(const osg::Vec3 &pt, const osg::Vec3 &ptonline, const osg::Vec3 &linedir)
	{
		osg::Vec3 retpt;
		float cosangl = (pt - ptonline) * linedir;
		retpt = ptonline + (linedir * cosangl);
		return retpt;
	}

	//一组点的aabb极点
	bool getRangePoint(const osg::ref_ptr<osg::Vec3Array>& vertPos, osg::Vec3& ptmax, osg::Vec3& ptmin)
	{
		if (!vertPos.valid() || vertPos->size() < 1)
		{
			return false;
		}
		ptmax = ptmin = vertPos->at(0);
		for (osg::Vec3Array::size_type i=1; i<vertPos->size(); ++i)
		{
			if (vertPos->at(i).x() > ptmax.x())  ptmax.x() = vertPos->at(i).x();
			if (vertPos->at(i).x() > ptmax.x())  ptmax.x() = vertPos->at(i).x();

			if (vertPos->at(i).y() > ptmax.y())  ptmax.y() = vertPos->at(i).y();
			if (vertPos->at(i).y() > ptmax.y())  ptmax.y() = vertPos->at(i).y();

			if (vertPos->at(i).z() > ptmax.z())  ptmax.z() = vertPos->at(i).z();
			if (vertPos->at(i).z() > ptmax.z())  ptmax.z() = vertPos->at(i).z();
		}
		return true;
	}

	osg::ref_ptr<osg::Vec3Array> GetOBBCorner(osg::ref_ptr<osg::Vec3Array>& vertPos)
	{
		osg::ref_ptr<osg::Vec3Array> retArr = new osg::Vec3Array;
		osg::Vec3 ptStart, ptEnd;
		osg::ref_ptr<osg::Vec3Array> ptPollar;
		//std::vector<osg::Vec3> ptPollar;
		ptPollar = getPointsPolarCorner(vertPos, ptStart, ptEnd);
		if (ptPollar->size() < 1)
		{
			return retArr;
		}
		float A, B, C, D;
		A= B = C = D = 0.0;
		calcPlaneEquation(ptStart, ptEnd, A, B, C, D);

		osg::ref_ptr<osg::Vec3Array> projectpts = new osg::Vec3Array;;
		for (osg::Vec3Array::size_type i = 0; i < ptPollar->size(); i++)
		{
			projectpts->push_back(pointProjectToPlane(ptPollar->at(i), A, B, C, D));
		}

		osg::Vec3 vHlfDir = (ptEnd - ptStart)/2.0;
		osg::Vec3 ptymax, ptymin;
		getPointsPolarCorner(projectpts, ptymax, ptymin);
	

		osg::Vec3 vnor = ptymax - ptymin;
		osg::Vec3 ptycenter = (ptymax+ptymin)/2.0;
		vnor = vnor^vHlfDir;
		vnor.normalize();
		//calcPlaneEquation(ptymax, ptymin, A, B, C, D);

		osg::ref_ptr<osg::Vec3Array> project2d1 = new osg::Vec3Array;;
		for (osg::Vec3Array::size_type i = 0; i < projectpts->size(); i++)
		{
			project2d1->push_back(pointProjectToLine(projectpts->at(i), ptycenter, vnor));
		}

		osg::Vec3 ptzmax, ptzmin;
		getPointsPolarCorner(project2d1, ptzmax, ptzmin);
		osg::Vec3 ptzcenter = (ptzmax+ptzmin)/2.0;


		retArr->push_back(ptymax + ptzcenter - ptzmax + vHlfDir);
		retArr->push_back(ptymin + ptzcenter - ptzmax + vHlfDir);
		retArr->push_back(ptzmax + ptycenter - ptymax + vHlfDir);
		retArr->push_back(ptymax + ptzmax - ptzcenter + vHlfDir);
			
		retArr->push_back(ptymax + ptzmax - ptzcenter - vHlfDir);
		retArr->push_back(ptzmax + ptycenter - ptymax - vHlfDir);
		retArr->push_back(ptymin + ptzcenter - ptzmax - vHlfDir);
		retArr->push_back(ptymax + ptzcenter - ptzmax - vHlfDir);

// 		retArr->push_back(ptymin - vHlfDir);
// 		retArr->push_back(ptzmin - vHlfDir);
// 		retArr->push_back(ptymax - vHlfDir);
// 		retArr->push_back(ptzmax - vHlfDir);

		return retArr;
	}
	
	static osg::Matrix _getConvarianceMatrix(const osg::ref_ptr<osg::Vec3Array> vertPos)
	{
		int i;
		osg::Matrix Cov;
		
		double S1[3];
		double S2[3][3];
		
		S1[0] = S1[1] = S1[2] = 0.0;
		S2[0][0] = S2[1][0] = S2[2][0] = 0.0;
		S2[0][1] = S2[1][1] = S2[2][1] = 0.0;
		S2[0][2] = S2[1][2] = S2[2][2] = 0.0;
		
		// get center of mass
		int vertCount = vertPos->size();
		for (i = 0; i < vertCount; i++)
		{
			S1[0] += (*vertPos)[i].x();
			S1[1] += (*vertPos)[i].y();
			S1[2] += (*vertPos)[i].z();
			
			S2[0][0] += (*vertPos)[i].x() * (*vertPos)[i].x();
			S2[1][1] += (*vertPos)[i].y() * (*vertPos)[i].y();
			S2[2][2] += (*vertPos)[i].z() * (*vertPos)[i].z();
			S2[0][1] += (*vertPos)[i].x() * (*vertPos)[i].y();
			S2[0][2] += (*vertPos)[i].x() * (*vertPos)[i].z();
			S2[1][2] += (*vertPos)[i].y() * (*vertPos)[i].z();
		}
		
		float n = (float)vertCount;
		// now get covariances
		Cov(0, 0) = (float)(S2[0][0] - S1[0] * S1[0] / n) / n;
		Cov(1, 1) = (float)(S2[1][1] - S1[1] * S1[1] / n) / n;
		Cov(2, 2) = (float)(S2[2][2] - S1[2] * S1[2] / n) / n;
		Cov(0, 1) = (float)(S2[0][1] - S1[0] * S1[1] / n) / n;
		Cov(1, 2) = (float)(S2[1][2] - S1[1] * S1[2] / n) / n;
		Cov(0, 2) = (float)(S2[0][2] - S1[0] * S1[2] / n) / n;
		Cov(1, 0) = Cov(0, 1);
		Cov(2, 0) = Cov(0, 2);
		Cov(2, 1) = Cov(1, 2);

		return Cov;
	}
	
	static float& _getElement(osg::Vec3& point, int index)
	{
		if (index == 0)
			return point.x();
		if (index == 1)
			return point.y();
		if (index == 2)
			return point.z();
		
		return point.x();
	}
	
	static void _getEigenVectors(osg::Matrix* vout, osg::Vec3* dout, osg::Matrix a)
	{
		int n = 3;
		int j, iq, ip, i;
		double tresh, theta, tau, t, sm, s, h, g, c;
		int nrot;
		osg::Vec3 b;
		osg::Vec3 z;
		osg::Matrix v;
		osg::Vec3 d;
		
		v = osg::Matrix::identity();
		for (ip = 0; ip < n; ip++)
		{
			_getElement(b, ip) = a(ip, ip);// a.m[ip + 4 * ip];
			_getElement(d, ip) = a(ip, ip);;// a.m[ip + 4 * ip];
			_getElement(z, ip) = 0.0;
		}
		
			nrot = 0;

		for (i = 0; i < 50; i++)
		{
			sm = 0.0;
			for (ip = 0; ip < n; ip++) for (iq = ip + 1; iq < n; iq++) sm += fabs((a(ip ,iq)));
			if (false/*fabs(sm) < FLT_EPSILON*/)
			{
				v = transpose(v);
				//v.transpose();
				*vout = v;
				*dout = d;
				return;
			}
			if (i < 3)
				tresh = 0.2 * sm / (n*n);
			else
				tresh = 0.0;
			
			for (ip = 0; ip < n; ip++)
			{
				for (iq = ip + 1; iq < n; iq++)
				{
					g = 100.0 * fabs(a(ip, iq)/*a.m[ip + iq * 4]*/);
					float dmip = _getElement(d, ip);
					float dmiq = _getElement(d, iq);
					
					if (i>3 && fabs(dmip) + g == fabs(dmip) && fabs(dmiq) + g == fabs(dmiq))
					{
						/*a.m[ip + 4 * iq]*/ a(ip, iq)= 0.0;
					}
					else if (fabs(a(ip, iq)) > tresh)
					{
						h = dmiq - dmip;
						if (fabs(h) + g == fabs(h))
						{
							t = (a(ip, iq)) / h;
						}
						else
						{
							theta = 0.5 * h / (a(ip, iq));
							t = 1.0 / (fabs(theta) + sqrt(1.0 + theta * theta));
							if (theta < 0.0) t = -t;
						}
						c = 1.0 / sqrt(1 + t*t);
						s = t*c;
						tau = s / (1.0 + c);
						h = t * a(ip, iq);
						_getElement(z, ip) -= (float)h;
						_getElement(z, iq) += (float)h;
						_getElement(d, ip) -= (float)h;
						_getElement(d, iq) += (float)h;
						a(ip, iq) = 0.0;
						for (j = 0; j < ip; j++) { ROTATE(a, j, ip, j, iq); }
						for (j = ip + 1; j < iq; j++) { ROTATE(a, ip, j, j, iq); }
						for (j = iq + 1; j < n; j++) { ROTATE(a, ip, j, iq, j); }
						for (j = 0; j < n; j++) { ROTATE(v, j, ip, j, iq); }
						nrot++;
					}
				}
			}

			for (ip = 0; ip < n; ip++)
			{
				_getElement(b, ip) += _getElement(z, ip);
				_getElement(d, ip) = _getElement(b, ip);
				_getElement(z, ip) = 0.0f;
			}
		}

		v = transpose(v);
		//v.transpose();
		*vout = v;
		*dout = d;
		return;
	}
	
	static osg::Matrix _getOBBOrientation(osg::ref_ptr<osg::Vec3Array> vertPos)
	{
		osg::Matrix Cov;
		
		if (vertPos->size() <= 0)
			return osg::Matrix::identity();
		
		Cov = _getConvarianceMatrix(vertPos);
		
	    // now get eigenvectors
		osg::Matrix Evecs;
		osg::Vec3 Evals;
		_getEigenVectors(&Evecs, &Evals, Cov);
		
		Evecs = transpose(Evecs);
		//Evecs.transpose();
		
		return Evecs;
	}
	
	OBB::OBB()
	{
		reset();
	}
	
	OBB::OBB(const osg::ref_ptr<osg::Vec3Array>& verts)
	{
		if (!verts.valid()) return;
		
		reset();
		
		int num = verts->size();
		osg::Matrix matTransform = _getOBBOrientation(verts);
		
		matTransform = transpose(matTransform);
		//matTransform.inverse(matTransform);
		
		osg::Vec3 vecMax = matTransform * osg::Vec3(verts->at(0).x(), verts->at(0).y(), verts->at(0).z());
		
		osg::Vec3 vecMin = vecMax;
		
		for (int i = 1; i < num; i++)
		{
			osg::Vec3 vect = matTransform * osg::Vec3(verts->at(i).x(), verts->at(i).y(), verts->at(i).z());
			
			vecMax.x() = vecMax.x() > vect.x() ? vecMax.x() : vect.x();
			vecMax.y() = vecMax.y() > vect.y() ? vecMax.y() : vect.y();
			vecMax.z() = vecMax.z() > vect.z() ? vecMax.z() : vect.z();
			
			vecMin.x() = vecMin.x() < vect.x() ? vecMin.x() : vect.x();
			vecMin.y() = vecMin.y() < vect.y() ? vecMin.y() : vect.y();
			vecMin.z() = vecMin.z() < vect.z() ? vecMin.z() : vect.z();
		}
	
		osg::Matrix mattemp = transpose(matTransform);
		//matTransform.inverse(matTransform);

		_xAxis = osg::Vec3(mattemp(0, 0), mattemp(1, 0), mattemp(2, 0));
		_yAxis = osg::Vec3(mattemp(0, 1), mattemp(1, 1), mattemp(2, 1));
		_zAxis = osg::Vec3(mattemp(0, 2), mattemp(1, 2), mattemp(2, 2));
		
		_center = (vecMax + vecMin)/2;
		matTransform.inverse(matTransform);
		_center = _center * matTransform;
		
		_xAxis.normalize();
		_yAxis.normalize();
		_zAxis.normalize();
		
		_extents = (vecMax - vecMin)/2;
		
		computeExtAxis();
	}

	bool OBB::containPoint(const osg::Vec3& point) const
	 {
		 osg::Vec3 vd = point - _center;
	
		float d = vd * (_xAxis);
		if (d > _extents.x() || d < -_extents.x())
		return false;
	
		d = vd * (_yAxis);
		if (d > _extents.y() || d < -_extents.y())
		return false;
	
		d = vd * (_zAxis);
		if (d > _extents.z() || d < -_extents.z())
		return false;
	
		return true;
	}

	void OBB::set(const osg::Vec3& center, const osg::Vec3& xAxis, const osg::Vec3& yAxis, const osg::Vec3& zAxis, const osg::Vec3& extents)
	{
		_center = center;
		_xAxis = xAxis;
		_yAxis = yAxis;
		_zAxis = zAxis;
		_extents = extents;
	}
	
	void OBB::reset()
	{
		memset(this, 0, sizeof(OBB));
	}
	
	void OBB::getCorners(osg::ref_ptr<osg::Vec3Array>& verts) const
	{
		if (!verts.valid())
		{
			verts = new osg::Vec3Array;
		}
		verts->clear();
		verts->push_back(_center - _extentX + _extentY + _extentZ);		// left top front   
		verts->push_back(_center - _extentX - _extentY + _extentZ);     // left bottom front
		verts->push_back(_center + _extentX - _extentY + _extentZ);     // right bottom front
		verts->push_back(_center + _extentX + _extentY + _extentZ);     // right top front
	
		verts->push_back(_center + _extentX + _extentY - _extentZ);     // right top back
		verts->push_back(_center + _extentX - _extentY - _extentZ);     // right bottom back
		verts->push_back(_center - _extentX - _extentY - _extentZ);     // left bottom back
		verts->push_back(_center - _extentX + _extentY - _extentZ);     // left top back
	}
	
	float OBB::projectPoint(const osg::Vec3& point, const osg::Vec3& axis)const
	{
		float dot = axis * point;
		float ret = dot * point.length();
		return ret;
	}
	
	void OBB::getInterval(const OBB& box, const osg::Vec3& axis, float &min, float &max)const
	{
		osg::ref_ptr<osg::Vec3Array> corners = new osg::Vec3Array;
		box.getCorners(corners);
		float value;
		min = max = projectPoint(axis, (*corners)[0]);
		for (int i = 1; i < 8; i++)
		{
			value = projectPoint(axis, (*corners)[i]);
			min = min > value ? value : min;
			max = max < value ? value : max;
		}
	}
	
	osg::Vec3 OBB::getEdgeDirection(int index)const
	{
		osg::ref_ptr<osg::Vec3Array> corners = new osg::Vec3Array;
		getCorners(corners);

		osg::Vec3 tmpLine;
		switch (index)
		{
		case 0:// edge with x axis
			tmpLine = (*corners)[5] - (*corners)[6];
			tmpLine.normalize();
			break;
		case 1:// edge with y axis
			tmpLine = (*corners)[7] - (*corners)[6];
			tmpLine.normalize();
			break;
		case 2:// edge with z axis
			tmpLine = (*corners)[1] - (*corners)[6];
			tmpLine.normalize();
			break;
		default:
			break;
		}
		return tmpLine;
	}
	
	osg::Vec3 OBB::getFaceDirection(int index) const
	{
		osg::ref_ptr<osg::Vec3Array> corners = new osg::Vec3Array;
		getCorners(corners);
	
		osg::Vec3 faceDirection, v0, v1;
		switch (index)
		{
		case 0:// front and back
			v0 = (*corners)[2] - (*corners)[1];
			v1 = (*corners)[0] - (*corners)[1];
			faceDirection = v0 ^ v1;
			faceDirection.normalize();
			break;
		case 1:// left and right
			v0 = (*corners)[5] - (*corners)[2];
			v1 = (*corners)[3] - (*corners)[2];
			faceDirection = v0 ^ v1;
			faceDirection.normalize();
			break;
		case 2:// top and bottom
			v0 = (*corners)[1] - (*corners)[2];
			v1 = (*corners)[5] - (*corners)[2];
			faceDirection = v0 ^ v1;
			faceDirection.normalize();
			break;
		default:
			break;
		}
		return faceDirection;
	}
	
		
	void OBB::transform(const osg::Matrix& mat)
	{
		osg::Vec4 newcenter = mat * osg::Vec4(_center.x(), _center.y(), _center.z(), 1.0f);// center;
		_center.x() = newcenter.x();
		_center.y() = newcenter.y();
		_center.z() = newcenter.z();
	
		_xAxis = mat * _xAxis;
		_yAxis = mat * _yAxis;
		_zAxis = mat * _zAxis;
	
		_xAxis.normalize();
		_yAxis.normalize();
		_zAxis.normalize();
	
		osg::Vec3 scale, trans;
		osg::Quat quat1, quat2;
		mat.decompose(scale, quat1, trans, quat2);
		
		_extents.x() *= scale.x();
		_extents.y() *= scale.y();
		_extents.z() *= scale.z();
		
		computeExtAxis();
	}

}