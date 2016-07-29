#include "UniformHandler.h"
#include <osgViewer/Viewer>
#include <osgDB/WriteFile>

UniformHandler::UniformHandler(osg::StateSet* stateSet)
{
	m_pStateSet = stateSet;
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
			osg::Vec3d mEyes, mCenter, mUp;
			m_pLightCamera->getViewMatrixAsLookAt(mEyes, mCenter, mUp);
// 			osg::Vec3 theUp = mCenter - m_lightPos;
// 			theUp = theUp ^ osg::Y_AXIS;
// 			theUp.normalize();
			//m_pLightCamera->setViewMatrixAsLookAt(m_lightPos, mCenter, mUp);
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
