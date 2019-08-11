

#pragma once

#include <osg\Node>
#include <osg\Array>



namespace render_system
{

	osg::ref_ptr<osg::Vec3Array> GetOBBCorner(osg::ref_ptr<osg::Vec3Array>& vertPos);

	class  OBB
	{
	public:
		OBB();
			
			
		/*
		* Construct obb from points
		*/
		OBB(const osg::ref_ptr<osg::Vec3Array>& verts);
			
		/*
		 * Check point in
		*/
		bool containPoint(const osg::Vec3& point) const;
			
		/*
		* Specify obb values
		*/
		void set(const osg::Vec3& center, const osg::Vec3& _xAxis, const osg::Vec3& _yAxis, const osg::Vec3& _zAxis, const osg::Vec3& _extents);
			
		/*
		* Clear obb
		*/
		void reset();
			
		/* face to the obb's -z direction
		* verts[0] : left top front
		* verts[1] : left bottom front
		* verts[2] : right bottom front
		* verts[3] : right top front
		*
		* face to the obb's z direction
		* verts[4] : right top back
		* verts[5] : right bottom back
		* verts[6] : left bottom back
		* verts[7] : left top back
		*/
		void getCorners(osg::ref_ptr<osg::Vec3Array>& verts) const;

		/**
			 * Transforms the obb by the given transformation matrix.
			 */
		void transform(const osg::Matrix& mat);
			
	protected:
		/*
		* compute extX, extY, extZ
	     */
		void computeExtAxis()
		{
			_extentX = _xAxis * _extents.x();
			_extentY = _yAxis * _extents.y();
			_extentZ = _zAxis * _extents.z();
		}
					
		/*
		* Project point to the target axis
		*/
		float projectPoint(const osg::Vec3& point, const osg::Vec3& axis) const;
		
		/*
		* Calculate the min and max project value of through the box's corners
		*/
		void getInterval(const OBB& box, const osg::Vec3& axis, float &min, float &max) const;
		
		/*
		* Get the edege of x y z axis direction
		*/
		osg::Vec3 getEdgeDirection(int index) const;
	
	    /*
		 * Get the face of x y z axis direction
		*/
		osg::Vec3 getFaceDirection(int index) const;
	
	public:
		osg::Vec3 _center;   // obb center
		osg::Vec3 _xAxis;    // x axis of obb, unit vector
		osg::Vec3 _yAxis;    // y axis of obb, unit vecotr
		osg::Vec3 _zAxis;    // z axis of obb, unit vector
		osg::Vec3 _extentX;  // _xAxis * _extents.x
		osg::Vec3 _extentY;  // _yAxis * _extents.y
		osg::Vec3 _extentZ;  // _zAxis * _extents.z
		osg::Vec3 _extents;  // obb length along each axis
	};

}