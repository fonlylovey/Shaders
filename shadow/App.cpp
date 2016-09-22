

#include <osgLib.h>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osg/TexMat>
#include <osg/Texture2D>

#include <osgUtil/CullVisitor>

#include <iostream>
#include <string>

#define AREA 50

osg::Camera* createRTTCamera( osg::Camera::BufferComponent buffer, osg::Texture* tex, bool bAbsolute = false)
{
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setClearColor( osg::Vec4() );
	camera->setAllowEventFocus(false);
	camera->setClearMask( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	camera->setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );
	camera->setRenderOrder( osg::Camera::POST_RENDER );
	camera->setProjectionMatrix(osg::Matrix::ortho(-AREA, AREA, -AREA, AREA, 0, 100*AREA));
	camera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
	camera->getOrCreateStateSet()->setMode(GL_ALPHA_TEST, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
	//camera->setProjectionMatrixAsFrustum(-AREA, AREA, -AREA, AREA, 1.0, 50*AREA);
	//camera->setProjectionMatrixAsPerspective(45.0, 1.0, 1.0, 100);
	camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	camera->setViewport( 0, 0, tex->getTextureWidth(), tex->getTextureHeight() );
	camera->attach( buffer, tex );
	return camera.release();
}

osg::Camera* createHUDForHeightMapDisplay(osg::Texture2D* tex)
{
	osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi) {
		osg::notify(osg::ALWAYS)<<"Error, no WindowSystemInterface available, cannot create windows."<<std::endl;
		return 0;
	}
	osg::Camera* hudCamera = new osg::Camera;

	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
	//osg::notify(osg::NOTICE)<<"Windows width: "<< width << ", height: " << height << std::endl;

	width = 1600;
	height = 1200;

	hudCamera->setProjectionMatrix(osg::Matrix::ortho2D(0,width,0,height));
	hudCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	hudCamera->setViewMatrix(osg::Matrix::identity());
	hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
	hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
	hudCamera->setAllowEventFocus(false);

	osg::Vec3Array* vertices = new osg::Vec3Array;
	vertices->push_back(osg::Vec3(100,100,-0.1));
	vertices->push_back(osg::Vec3(400,100,-0.1));
	vertices->push_back(osg::Vec3(400,400,-0.1));
	vertices->push_back(osg::Vec3(100,400,-0.1));

	osg::Geometry* geometry = new osg::Geometry;
	geometry->setVertexArray(vertices);

	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
	geometry->setNormalArray(normals);
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

	osg::Vec2Array* uvs = new osg::Vec2Array;
	uvs->push_back(osg::Vec2(0,0));
	uvs->push_back(osg::Vec2(1,0));
	uvs->push_back(osg::Vec2(1,1));
	uvs->push_back(osg::Vec2(0,1));
	geometry->setTexCoordArray(0,uvs);
	geometry->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.f,1.f,1.f,1.f));
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	geometry->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

	osg::Geode* geode = new osg::Geode;
	geode->addDrawable(geometry);

	hudCamera->addChild(geode);

	return hudCamera;
}


std::string readFileToString(const std::string& fileName)
{
	std::string content;

	std::ifstream file;
	file.open(fileName.c_str(),std::ios::in);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file,line))
		{
			content += line;
			content += std::string("\n");
			//std::cout<<line<<std::endl;
		}
		file.close();
	}
	return content;
}


osg::ref_ptr<osg::Texture2D>	_depthMap;
osg::ref_ptr<osg::Camera>		_depthCamera;

osg::ref_ptr<osgViewer::Viewer> viewer;
osg::ref_ptr<osg::Group> root;

int main()
{
	viewer = new osgViewer::Viewer;
	root = new osg::Group;
	/************************************************************************/
	/*                 scene init                                           */
	/************************************************************************/
	osg::ref_ptr<osg::Node> lz = osgDB::readNodeFile("lz.osg");
	root->addChild(lz);

	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::MatrixTransform> cowMat = new osg::MatrixTransform;
	cowMat->setMatrix(osg::Matrix::translate(0, 0, 100));
	cowMat->addChild(cow);
	//root->addChild(cowMat);

	viewer->setSceneData(root);
	/************************************************************************/
	/*               shadow map init                                        */
	/************************************************************************/
	_depthMap = new osg::Texture2D;
	_depthMap->setTextureSize(1400, 900);

	//_depthMap->setInternalFormat(GL_RGBA);
	_depthMap->setSourceFormat(GL_DEPTH_COMPONENT);
	_depthMap->setSourceType(GL_FLOAT);
	_depthMap->setInternalFormat(GL_DEPTH_COMPONENT);

	_depthMap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER);
	_depthMap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER);
	_depthMap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST);
	_depthMap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
	//_depthMap->setShadowTextureMode(osg::Texture::LUMINANCE);
	_depthMap->setBorderColor(osg::Vec4(1.0, 0.0, 0.0, 0.0));
	_depthCamera = createRTTCamera(osg::Camera::DEPTH_BUFFER, _depthMap, true);
	_depthCamera->addChild(cowMat);
	_depthCamera->addChild(lz);
	root->addChild(_depthCamera);


	osg::Vec3 heightMapCameraEye = osg::Vec3(0.0, 0.0, 220.0);
	osg::Vec3 heightMapCameraUp = osg::Vec3(0,1.0,0);
	osg::Vec3 heightMapCameraCenter = osg::Vec3(0.0, 0.0, 0.0);
	_depthCamera->setViewMatrixAsLookAt(heightMapCameraEye,
										heightMapCameraCenter,
										heightMapCameraUp);
	root->addChild(createHUDForHeightMapDisplay(_depthMap));
	/************************************************************************/
	/*                 shadow                                               */
	/************************************************************************/


	osg::StateSet* state = lz->getOrCreateStateSet();
	osg::ref_ptr<osg::Program> airportProgram = new osg::Program;
	std::string shaderSource = readFileToString("shadowmap_vs.glsl");	
	airportProgram->addShader(new osg::Shader(osg::Shader::VERTEX, shaderSource));

	shaderSource = readFileToString("shadowmap_fs.glsl");	
	airportProgram->addShader(new osg::Shader(osg::Shader::FRAGMENT, shaderSource));

	state->setAttributeAndModes(airportProgram.get(), osg::StateAttribute::ON);

	osg::Matrix bias(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0 );

	osg::Matrix tmat = _depthCamera->getViewMatrix() * _depthCamera->getProjectionMatrix() * bias;


	state->setTextureAttributeAndModes(4, new osg::TexMat(tmat), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	state->addUniform( new osg::Uniform("shadowMatrix", tmat) );

	state->setTextureAttributeAndModes( 4, _depthMap);
	state->addUniform( new osg::Uniform("depTex", 4) );
	state->addUniform( new osg::Uniform("sceneTex", 0) );

	return viewer->run();
}