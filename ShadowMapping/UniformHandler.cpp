#include "UniformHandler.h"
#include <osgViewer/Viewer>
#include <osgDB/WriteFile>

UniformHandler::UniformHandler(osg::StateSet* stateSet)
{
	m_pStateSet = stateSet;
	m_pTexture = nullptr;
	m_pLightCamera = nullptr;
}


UniformHandler::~UniformHandler()
{
}

bool UniformHandler::handle(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa)
{
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (viewer == NULL)
	{
		return false;
	}
	if (osgGA::GUIEventAdapter::FRAME == ea.getEventType())
	{
		if (m_pLightCamera != nullptr)
		{
			//m_pLightCamera->attach(osg::Camera::DEPTH_BUFFER, m_pTexture);
			//m_pHUDCamera->setTexture(m_pTexture);
			//osgDB::writeImageFile(*m_pTexture->getImage(0), "Debug/depth.jpg");
		}
		return false;
	}
	return false;
}

void UniformHandler::setLightPos(osg::Vec3 lPos)
{
	m_lightPos = lPos;
}

void UniformHandler::setLightCamera(osg::Camera* camera)
{
	m_pLightCamera = camera;
}


void UniformHandler::setTexture(osg::Texture2D* texture)
{
	m_pTexture = texture;
}


void UniformHandler::setHUD(HUDCamera* camera)
{
	m_pHUDCamera = camera;
}
