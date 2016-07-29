#include <osgLib.h>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/StateSet>
#include <osg/MatrixTransform>
#include <osg/Image>
#include <osg/TexGen>
#include <osg/Texture>

void main()
{
	osg::Group* root = new osg::Group;
	osgViewer::Viewer* viewer = new osgViewer::Viewer;
	viewer->setSceneData(root);
	osg::Node* cow = osgDB::readNodeFile("cow.osg");
	root->addChild(cow);

	osg::StateSet* stateSet = root->getOrCreateStateSet();
	
	osg::Program* profram = new osg::Program;

	osg::Shader* vertShader = osgDB::readShaderFile(osg::Shader::VERTEX, "test.vert");
	osg::Shader* fragShader = osgDB::readShaderFile(osg::Shader::FRAGMENT, "test.frag");
	profram->addShader(vertShader);
	profram->addShader(fragShader);
	osg::Uniform* sampler0 = new osg::Uniform("baseMap", 0);
	stateSet->addUniform(sampler0);
	stateSet->setAttributeAndModes(profram);
	stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	
	viewer->run();
}