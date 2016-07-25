#ifndef HUDCAMERA_H
#define HUDCAMERA_H

#include <osg/Image>
#include <osg/Camera>
#include <osg/Geode>
#include <osg/Texture2D>

class HUDCamera : public osg::Camera
{
public:
	HUDCamera();
	HUDCamera(double left, double right, double bottom, double top);
	virtual ~HUDCamera();

	void setTexture(osg::Image* image);
	void setTexture(osg::Texture2D* texture);
private:
	void init();
	void createQuad();
public:
	osg::ref_ptr<osg::Geode>	m_pGeode;
	osg::ref_ptr<osg::Texture2D> m_pTexture;
};

#endif //!HUDCAMERA_H