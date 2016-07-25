#include <osg/Group>
#include <osgLib.h>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/StateSet>
#include <osg/Shader>
#include <osg/Uniform>
#include <osg/Program>

#include <osg/MatrixTransform>
#include "GeometryFloor.h"
#include <osg/Image>
#include <osg/Texture>
#include "UniformHandler.h"
#include <osg/Light>
#include <osg/LightSource>
#include <osgDB/WriteFile>
#include "HUDCamera.h"
#include <osgViewer/ViewerEventHandlers>

void main()
{
	osg::Group* root = new osg::Group;
	osgViewer::Viewer* viewer = new osgViewer::Viewer;
	viewer->setSceneData(root);
	GeometryFloor* floor = new GeometryFloor;
	root->addChild(floor);

	osg::MatrixTransform* renMT = new osg::MatrixTransform;
	osg::Node* ren = osgDB::readNodeFile("cow.osg");
	renMT->addChild(ren);
 	renMT->setMatrix(osg::Matrix::translate(0, 0, 3));
	root->addChild(renMT);

	osg::ref_ptr<osg::Texture2D> depthTex = new osg::Texture2D;
	depthTex->setTextureSize(1440, 900);
	depthTex->setInternalFormat(GL_DEPTH_COMPONENT);
	depthTex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
	depthTex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
	depthTex->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::CLAMP_TO_BORDER);
	depthTex->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::CLAMP_TO_BORDER);

	float radius = ren->getBound().radius();
	osg::Camera* lightCamera = new osg::Camera;
	root->addChild(lightCamera);
	lightCamera->addChild(ren);
	lightCamera->setClearDepth(1.0);
	lightCamera->setViewport(0, 0, 1440, 900);
	lightCamera->setRenderOrder(osg::Camera::PRE_RENDER);
	lightCamera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	lightCamera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	lightCamera->attach(osg::Camera::DEPTH_BUFFER, depthTex.get());
	lightCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lightCamera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
	lightCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	lightCamera->setProjectionMatrixAsOrtho(-radius, radius, -radius, radius, -10, 10);

 	HUDCamera* hudCamera = new HUDCamera(2, 220, 2, 220);
 	hudCamera->setTexture(depthTex);
 	root->addChild(hudCamera);
	
	osg::StateSet* stateSet = root->getOrCreateStateSet();
	{
		osg::Program* profram = new osg::Program;

		osg::Shader* vertShader = osgDB::readShaderFile(osg::Shader::VERTEX, "shadow.vert");
		osg::Shader* fragShader = osgDB::readShaderFile(osg::Shader::FRAGMENT, "shadow.frag");
		profram->addShader(vertShader);
		profram->addShader(fragShader);

		//osg::Uniform* ltPos = new osg::Uniform("ltPos", lightPos);
		//osg::Uniform* eyePos = new osg::Uniform("eyePos", eyesPos);
		osg::Uniform* sampler0 = new osg::Uniform("sampler0", 0);
		osg::Uniform* sampler3 = new osg::Uniform("sampler3", 3);

		//stateSet->addUniform(ltPos);
		//stateSet->addUniform(eyePos);
		stateSet->addUniform(sampler0);
		stateSet->addUniform(sampler3);
		stateSet->setAttributeAndModes(profram);
		stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateSet->setTextureAttribute(3, depthTex);

// 		UniformHandler* handler = new UniformHandler(stateSet);
// 		handler->setLightPos(lightPos);
// 		handler->setLightCamera(lightCamera);
// 		handler->setTexture(depthTex);
// 		viewer->addEventHandler(handler);

	}

	viewer->addEventHandler(new osgViewer::StatsHandler);
	viewer->run();
}