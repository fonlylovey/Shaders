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

void main()
{
	osg::Group* root = new osg::Group;
	osg::Group* scene = new osg::Group;
	root->addChild(scene);

	osgViewer::Viewer* viewer = new osgViewer::Viewer;
	viewer->setSceneData(root);
	GeometryFloor* floor = new GeometryFloor;
	//floor->setTexture("Grid.bmp");
	scene->addChild(floor);

	osg::Node* truck = osgDB::readNodeFile("cessna.osg");
	osg::MatrixTransform* renMT = new osg::MatrixTransform;
	renMT->addChild(truck);
	renMT->setMatrix(osg::Matrix::translate(osg::Vec3(0, 0, 8)));
	scene->addChild(renMT);

	osg::ref_ptr<osg::Texture2D> depthTex = new osg::Texture2D;
	depthTex->setTextureSize(1440, 900);
	depthTex->setShadowComparison(true);
	depthTex->setInternalFormat(GL_DEPTH_COMPONENT);
	depthTex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
	depthTex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
	depthTex->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::CLAMP_TO_BORDER);
	depthTex->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::CLAMP_TO_BORDER);

	float radius = scene->getBound().radius();
	osg::Camera* lightCamera = new osg::Camera;
	root->addChild(lightCamera);
	lightCamera->addChild(scene);
	lightCamera->setClearDepth(1.0);
	lightCamera->setViewport(0, 0, 1440, 900);
	lightCamera->setRenderOrder(osg::Camera::PRE_RENDER);
	lightCamera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	lightCamera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	lightCamera->attach(osg::Camera::DEPTH_BUFFER, depthTex.get());
	lightCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
	lightCamera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
	lightCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	//lightCamera->setProjectionMatrixAsOrtho(0, 1440, 0, 900, -10, 10);
	lightCamera->setProjectionMatrixAsOrtho(-radius, radius, -radius, radius, -10, 10);
	//lightCamera->setViewMatrixAsLookAt();
	/*osg::StateSet* ss = lightCamera->getOrCreateStateSet();
	osg::ref_ptr<osg::CullFace> cull_face = new osg::CullFace;
	cull_face->setMode(osg::CullFace::FRONT);
	ss->setAttribute(cull_face.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	ss->setMode(GL_CULL_FACE, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	osg::ref_ptr<osg::PolygonOffset> polygon_offset = new osg::PolygonOffset;
	polygon_offset->setFactor(-1.0);
	polygon_offset->setUnits(-1.0);
	ss->setAttribute(polygon_offset.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	ss->setMode(GL_POLYGON_OFFSET_FILL, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);*/
	osg::Matrix shadowVMat = lightCamera->getViewMatrix();
	osg::Matrix shadowPMat = lightCamera->getProjectionMatrix();

 	HUDCamera* hudCamera = new HUDCamera(2, 220, 2, 220);
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
		stateSet->setAttributeAndModes(profram);

		stateSet->setTextureAttributeAndModes(1, depthTex);
		stateSet->setTextureMode(1, GL_TEXTURE_GEN_S, osg::StateAttribute::ON);
		stateSet->setTextureMode(1, GL_TEXTURE_GEN_T, osg::StateAttribute::ON);
		stateSet->setTextureMode(1, GL_TEXTURE_GEN_R, osg::StateAttribute::ON);
		stateSet->setTextureMode(1, GL_TEXTURE_GEN_Q, osg::StateAttribute::ON);
		stateSet->addUniform(new osg::Uniform("baseMap", 0));
		stateSet->addUniform(new osg::Uniform("shadowMap", 1));
		stateSet->addUniform(new osg::Uniform("shadowColor", osg::Vec4(0., 0., 0., 1.)));
		stateSet->addUniform(new osg::Uniform("ShadowVP", shadowVMat*shadowPMat));
 		stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
 		stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
 		

 		UniformHandler* handler = new UniformHandler(stateSet);
 		handler->setLightCamera(lightCamera);
		handler->setLightPos(lightPos);
 		viewer->addEventHandler(handler);

	}

	viewer->addEventHandler(new osgViewer::StatsHandler);
	viewer->run();
}