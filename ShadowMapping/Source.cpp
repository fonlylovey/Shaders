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
#include <osg/LightSource>
#include <osg/Light>
#include <osg/CullFace>
#include <osg/PolygonOffset>
#include <osg/TexMat>

osg::Camera* createLightCamera()
{
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setAllowEventFocus(false);
	camera->setViewport(0, 0, 1400, 900);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	camera->setClearColor(osg::Vec4());

	camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
	camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

	osg::StateSet* ss = camera->getOrCreateStateSet();
	ss->setMode(GL_ALPHA_TEST, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

	return camera.release();
};

void main()
{
	osg::Group* root = new osg::Group;
	osg::Group* scene = new osg::Group;

	osgViewer::Viewer* viewer = new osgViewer::Viewer;
	viewer->setSceneData(root);

	GeometryFloor* floor = new GeometryFloor;
	floor->setTexture("Grid.bmp");
	osg::ref_ptr<osg::Node> lz = osgDB::readNodeFile("lz.osg");

	osg::Node* truck = osgDB::readNodeFile("cessna.osg");
	osg::MatrixTransform* renMT = new osg::MatrixTransform;
	renMT->addChild(truck);
	renMT->setMatrix(osg::Matrix::translate(osg::Vec3(0, 0, 100)));
	scene->addChild(renMT);
	scene->addChild(lz);
	root->addChild(lz);
	root->addChild(renMT);
	osg::ref_ptr<osg::Texture2D> depthTex = new osg::Texture2D;
	depthTex->setShadowComparison(false);
	depthTex->setTextureSize(1400, 900);
	depthTex->setSourceType(GL_FLOAT);
	depthTex->setSourceFormat(GL_DEPTH_COMPONENT);
	depthTex->setInternalFormat(GL_DEPTH_COMPONENT);
	
	depthTex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::NEAREST);
	depthTex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::NEAREST);
	depthTex->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::CLAMP_TO_BORDER);
	depthTex->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::CLAMP_TO_BORDER);
	depthTex->setBorderColor(osg::Vec4(1.0, 0.0, 0.0, 0.0));

	float radius = scene->getBound().radius();
	osg::Camera* lightCamera = createLightCamera();
	lightCamera->setProjectionMatrixAsOrtho(-radius, radius, -radius, radius, 0, 1000);
	lightCamera->attach(osg::Camera::DEPTH_BUFFER, depthTex.get());
	osg::Vec3 lightCameraEye = osg::Vec3(0.0, 0.0, 220);
	osg::Vec3 lightCameraUp = osg::Vec3(0, 1.0, 0);
	osg::Vec3 lightCameraCenter = osg::Vec3(0.0, 0.0, 0.0);
	lightCamera->setViewMatrixAsLookAt(lightCameraEye, lightCameraCenter, lightCameraUp);
	lightCamera->addChild(scene);
	root->addChild(lightCamera);

 	HUDCamera* hudCamera = new HUDCamera(2, 220, 2, 200);
 	hudCamera->setTexture(depthTex);
 	root->addChild(hudCamera);
	
	osg::Vec3 lightPos = osg::Vec3(0, 0, 30);
	osg::StateSet* stateSet = scene->getOrCreateStateSet();
	{
		osg::Program* profram = new osg::Program;

		osg::Shader* vertShader = osgDB::readShaderFile(osg::Shader::VERTEX, "shadow.vert");
		osg::Shader* fragShader = osgDB::readShaderFile(osg::Shader::FRAGMENT, "shadow.frag");
		profram->addShader(vertShader);
		profram->addShader(fragShader);
		stateSet->setAttributeAndModes(profram, osg::StateAttribute::ON);

		osg::Matrix shadowVMat = lightCamera->getViewMatrix();
		osg::Matrix shadowPMat = lightCamera->getProjectionMatrix();
		osg::Matrix bias(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0);
		osg::Matrix texMatrix = shadowVMat*shadowPMat * bias;
		stateSet->setTextureAttributeAndModes(4, depthTex);
		stateSet->addUniform(new osg::Uniform("baseMap", 0));
		stateSet->addUniform(new osg::Uniform("shadowMap", 4));
		stateSet->addUniform(new osg::Uniform("ShadowVP", texMatrix));
		stateSet->setTextureAttributeAndModes(4, new osg::TexMat(texMatrix), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	}
	viewer->run();
}