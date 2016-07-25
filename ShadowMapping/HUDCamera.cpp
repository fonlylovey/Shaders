#include "HUDCamera.h"
#include <osg/Array>
#include <osg/Geometry>
#include <osgText/Text>
#include <osgViewer/GraphicsWindow>

HUDCamera::HUDCamera()
{
	m_pTexture = NULL;
	m_pGeode = new osg::Geode;
	addChild(m_pGeode);
}

HUDCamera::HUDCamera(double left, double right, double bottom, double top)
{
	m_pTexture = NULL;
	m_pGeode = new osg::Geode;
	m_pGeode->setDataVariance(osg::Object::DYNAMIC);
	addChild(m_pGeode);
	setProjectionMatrixAsOrtho2D(left, right, bottom, top);
	setViewport(left, bottom, right, top);
	init();
}

HUDCamera::~HUDCamera()
{
}

void HUDCamera::init()
{
	setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	setViewMatrix(osg::Matrix::identity());
	setClearMask(GL_DEPTH_BUFFER_BIT);
	setClearColor(osg::Vec4(0.2f, 0.2f, 0.4f, 0.5f));
	setRenderOrder(osg::Camera::POST_RENDER);
	setAllowEventFocus(false);
	getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | 
		osg::StateAttribute::OVERRIDE);
}

void HUDCamera::setTexture(osg::Texture2D* texture)
{
	m_pTexture = texture;
	m_pGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, m_pTexture);
	createQuad();
}

void HUDCamera::createQuad()
{
	osg::Viewport* viewPort = getViewport();
	osg::Geometry* geom = new osg::Geometry;
	osg::Vec3Array* vertices = new osg::Vec3Array;
	double depth = 0;
	vertices->push_back(osg::Vec3(0,depth, viewPort->y()));
	vertices->push_back(osg::Vec3(viewPort->x() + viewPort->width(),
		depth, 0));
	vertices->push_back(osg::Vec3(viewPort->x() + viewPort->width(), 
		viewPort->y() + viewPort->height(), depth));
	vertices->push_back(osg::Vec3(viewPort->x(), viewPort->y() + viewPort->height(), depth));
	geom->setVertexArray(vertices);

	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
	geom->setNormalArray(normals, osg::Array::BIND_OVERALL);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0, 1.0, 0.0, 1.0));
	//geom->setColorArray(colors, osg::Array::BIND_OVERALL);

	osg::Vec2Array* texcoord = new osg::Vec2Array;
	texcoord->push_back(osg::Vec2(0.0, 0.0));
	texcoord->push_back(osg::Vec2(1.0, 0.0));
	texcoord->push_back(osg::Vec2(1.0, 1.0));
	texcoord->push_back(osg::Vec2(0.0, 1.0));
	geom->setTexCoordArray(0, texcoord, osg::Array::BIND_PER_VERTEX);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::QUADS, 0, 4));

	osg::StateSet* stateset = m_pGeode->getOrCreateStateSet();
	stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
	stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	m_pGeode->addDrawable(geom);
}

