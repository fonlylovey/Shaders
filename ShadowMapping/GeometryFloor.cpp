#include "GeometryFloor.h"
#include <osg/Material>
#include <osg/PolygonOffset>
#include <osg/PolygonMode>

GeometryFloor::GeometryFloor(void)
{
	createFloor();
}

GeometryFloor::~GeometryFloor(void)
{
}

void GeometryFloor::createFloor()
{
	setName("Plane");
	osg::Geometry* plane = createPlane(50, 50);
	addDrawable(plane);

	plane->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	
	//ÉèÖÃ¿ÌÏßäÖÈ¾×´Ì¬
	osg::Geometry* gird = createPlane(50, 50);
	osg::StateSet* stateset = gird->getOrCreateStateSet();  
	osg::ref_ptr<osg::PolygonOffset> polyoffset = new osg::PolygonOffset;  
	polyoffset->setFactor(-1.0f);  
	polyoffset->setUnits(-1.0f);  
	osg::ref_ptr<osg::PolygonMode> polymode = new osg::PolygonMode;  
	polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);  
	stateset->setAttributeAndModes(polyoffset,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);  
	stateset->setAttributeAndModes(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);  

	osg::ref_ptr<osg::Material> material = new osg::Material; 
	stateset->setAttributeAndModes(material,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);  
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);  
	stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);  
	addDrawable(gird);
}

osg::Geometry*  GeometryFloor::createPlane(size_t lenght, size_t width)
{
		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1, 1, 1, 1.0));
		geom->setColorArray(colors, osg::Array::BIND_OVERALL);
		for (size_t row = 0; row < lenght - 1; row++)
		{
			for (size_t col = 0; col < width - 1; col++)
			{
				vertices->push_back(osg::Vec3(row, col, 0));
				vertices->push_back(osg::Vec3(row + 1, col, 0));
				vertices->push_back(osg::Vec3(row + 1, col + 1, 0));
				vertices->push_back(osg::Vec3(row, col + 1, 0));
				normals->push_back(osg::Z_AXIS);
			}
		}
		size_t count = vertices->size();
		for (size_t i = 0; i < count; i++)
		{
			vertices->at(i).x() -= lenght / 2;
			vertices->at(i).y() -= width / 2;
		}


		geom->setVertexArray(vertices);
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertices->size()));

		return geom.release();
}