#ifndef GEOMETRYFLOOR_H
#define GEOMETRYFLOOR_H

#include <osg/Geode>
#include <osg/Geometry>

class GeometryFloor : public osg::Geode
{
public:
	GeometryFloor(void);
	virtual ~GeometryFloor(void);

private:
	void createFloor();

	osg::Geometry* createPlane(size_t lenght, size_t width);
};

#endif//GEOMETRYFLOOR_H