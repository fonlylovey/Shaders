#include <osgLib.h>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/StateSet>
#include <osg/MatrixTransform>
#include <osg/Image>
#include <osg/Texture>
#include "HUDCamera.h"

void main()
{
	osg::Group* root = new osg::Group;
	osgViewer::Viewer* viewer = new osgViewer::Viewer;
	viewer->setSceneData(root);


	osg::Image* image = osgDB::readImageFile("123.jpg");

	HUDCamera* hudCamera = new HUDCamera(50., 220., 50., 220.);
	hudCamera->setTexture(image);
	root->addChild(hudCamera);


	viewer->run();
}