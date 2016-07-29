#ifndef HUDCAMERA_H
#define HUDCAMERA_H

#include <osg/Image>
#include <osg/Camera>
#include <osg/Geode>
#include <osg/Texture2D>

class TexCallBack : public osg::Drawable::DrawCallback
{
public:
	TexCallBack(osg::Texture2D * texture, unsigned stage = 0);
	~TexCallBack(){};

	virtual void drawImplementation
		(osg::RenderInfo & ri, const osg::Drawable* drawable) const;

private:
	osg::ref_ptr< osg::Texture2D > _texture;
	unsigned                       _stage;
};

class HUDCamera : public osg::Camera
{
public:
	HUDCamera();
	HUDCamera(double left, double right, double bottom, double top);
	virtual ~HUDCamera();

	void setTexture(osg::Texture2D* texture);
private:
	void init();
	void createQuad();
public:
	osg::ref_ptr<osg::Geode>	m_pGeode;
	osg::ref_ptr<osg::Texture2D> m_pTexture;
};

#endif //!HUDCAMERA_H