#include <osg/Group>
#include <tchar.h>
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

void main()
{
	osg::Group* root = new osg::Group;
	osgViewer::Viewer* viewer = new osgViewer::Viewer;
	viewer->setSceneData(root);
	GeometryFloor* floor = new GeometryFloor;
	root->addChild(floor);

	osg::MatrixTransform* renMT = new osg::MatrixTransform;
	osg::Node* ren = osgDB::readNodeFile("ren.fbx");
	renMT->addChild(ren);
	renMT->setMatrix(osg::Matrix::rotate(-osg::PI_2, osg::X_AXIS, osg::PI, osg::Z_AXIS, 0, osg::Y_AXIS)
		* osg::Matrix::translate(0, 0, 1));
	root->addChild(renMT);

	osg::Image* image = new osg::Image;
	int noPixels = 1;
	image->allocateImage(1024, 1024, 1, GL_DEPTH_COMPONENT, GL_FLOAT);
	image->setInternalTextureFormat(GL_DEPTH_COMPONENT);

	osg::Texture2D* depthTex = new osg::Texture2D;
	depthTex->setImage(image);
	depthTex->setTextureSize(1024, 1024);
	depthTex->setInternalFormat(GL_DEPTH_COMPONENT);
	depthTex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
	depthTex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
	depthTex->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::CLAMP_TO_BORDER);
	depthTex->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::CLAMP_TO_BORDER);
	osg::Vec3 eyesPos = osg::Vec3(0, 300, 100);
	osg::Vec3 lightPos = osg::Vec3(100, 100, 100);

	osg::Camera* lightCamera = new osg::Camera;
 	float radius = root->getBound().radius();
 	lightCamera->setViewport(0, 0, 1440, 900);
 	lightCamera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
 	lightCamera->setClearDepth(1.0);
 	lightCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
 	lightCamera->setProjectionMatrixAsOrtho(-radius, radius, -radius, radius, -2, 1);
 	lightCamera->setRenderOrder(osg::Camera::PRE_RENDER);
 	lightCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
 	lightCamera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
 	lightCamera->setNearFarRatio(0.001);
 	//lightCamera->setReferenceFrame(osg::Camera::ABSOLUTE_RF_INHERIT_VIEWPOINT);
	lightCamera->addChild(renMT);
 	lightCamera->setViewMatrixAsLookAt(osg::Vec3d(0, 2, 0), osg::Vec3d(0, 0, 0), osg::Vec3d(0, 0, 1));
	//osg::StateSet* stateset = lightCamera->getOrCreateStateSet();

	root->addChild(lightCamera);
	lightCamera->attach(osg::Camera::DEPTH_BUFFER, image);

	{
		osg::StateSet* stateSet = root->getOrCreateStateSet();
		osg::Program* profram = new osg::Program;

		osg::Shader* vertShader = osgDB::readShaderFile(osg::Shader::VERTEX, "shadow.vert");
		osg::Shader* fragShader = osgDB::readShaderFile(osg::Shader::FRAGMENT, "shadow.frag");
		profram->addShader(vertShader);
		profram->addShader(fragShader);

		osg::Uniform* ltPos = new osg::Uniform("ltPos", lightPos);
		osg::Uniform* eyePos = new osg::Uniform("eyePos", eyesPos);
		osg::Uniform* sampler0 = new osg::Uniform("sampler0", 0);
		osg::Uniform* sampler3 = new osg::Uniform("sampler3", 3);

		stateSet->addUniform(ltPos);
		stateSet->addUniform(eyePos);
		stateSet->addUniform(sampler0);
		stateSet->addUniform(sampler3);
		stateSet->setAttributeAndModes(profram);
		stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateSet->setTextureAttribute(3, depthTex, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		UniformHandler* handler = new UniformHandler(stateSet);
		handler->setLightPos(lightPos);
		handler->setLightCamera(lightCamera);
		handler->setTexture(depthTex);
		viewer->addEventHandler(handler);

	}

	viewer->run();
}