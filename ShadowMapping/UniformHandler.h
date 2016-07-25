#ifndef UNIFORMHANDLER_H
#define UNIFORMHANDLER_H

#include <osgGA/GUIEventHandler>
#include <osg/StateSet>
#include <osg/Texture2D>
#include "HUDCamera.h"
class UniformHandler : public osgGA::GUIEventHandler
{
public:
	UniformHandler(osg::StateSet* stateSet);
	virtual ~UniformHandler();

	virtual bool handle(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);

	void setTexture(osg::Texture2D* texture);

	void setLightPos(osg::Vec3 lPos);

	void setLightCamera(osg::Camera* camera);

	void setHUD(HUDCamera* camera);
private:
	osg::Vec3 m_lightPos;
	HUDCamera* m_pHUDCamera;
	osg::ref_ptr<osg::Texture2D> m_pTexture;
	osg::ref_ptr<osg::StateSet> m_pStateSet;
	osg::ref_ptr<osg::Camera> m_pLightCamera;

};

#endif//UNIFORMHANDLER_H
