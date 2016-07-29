#include <osg/Group>
#include <tchar.h>
#include <osgLib.h>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/StateSet>
#include <osg/Shader>
#include <osg/Uniform>
#include <osg/Program>
#include <osgGA/EventHandler>
#include <osg/MatrixTransform>

class UniformHandler : public osgGA::GUIEventHandler
{
public:
	UniformHandler(osg::StateSet* stateSet)
	{
		m_pStateSet = stateSet;
	}
	~UniformHandler(){};

	virtual bool handle(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa)
	{
		osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (viewer == NULL)
		{
			return false;
		}
		if (osgGA::GUIEventAdapter::FRAME == ea.getEventType())
		{
			osg::Uniform* uniform = m_pStateSet->getUniform("eyePos");
			if (uniform != NULL)
			{
				osg::Vec3 cameraPos = viewer->getCamera()->getBound().center();
				uniform->set(cameraPos);
			}
			return false;
		}
		return false;
	}
private:
	osg::StateSet* m_pStateSet;
};

void main()
{
	osg::Group* root = new osg::Group;
	osgViewer::Viewer* viewer = new osgViewer::Viewer;
	viewer->setSceneData(root);
	osg::Node* node = osgDB::readNodeFile("../Win32/Models/FixedCannon.ive");
	osg::MatrixTransform* mtNode = new osg::MatrixTransform;
	mtNode->addChild(node);
	mtNode->setMatrix(osg::Matrix::translate(1, 0, 0));
	root->addChild(mtNode);
	root->addChild(node);
	
	{
		osg::StateSet* stateSet = mtNode->getOrCreateStateSet();
		osg::Program* profram = new osg::Program;
		
		osg::Shader* vertShader = osgDB::readShaderFile(osg::Shader::VERTEX, "phong.vert");
		osg::Shader* fragShader = osgDB::readShaderFile(osg::Shader::FRAGMENT, "phong.frag");
		profram->addShader(vertShader);
		profram->addShader(fragShader);

		osg::Vec3 eyesPos = osg::Vec3(0, 300, 100);
		osg::Vec3 lightPos = osg::Vec3(100, 100, 100);
		osg::Vec4 diffus = osg::Vec4(0.58, 0.58, 0.58, 0.0);
		osg::Vec4 ambient = osg::Vec4(0.5, 0.5, 0.5, 0.);
		osg::Vec4 specular = osg::Vec4(0.6, 0.4, 0.4,1.0);
		
		osg::Uniform* ltIntensity = new osg::Uniform("ltIntensity", 10);
		osg::Uniform* ltPos = new osg::Uniform("ltPos", lightPos);
		osg::Uniform* eyePos = new osg::Uniform("eyePos", eyesPos);
		osg::Uniform* ltDiffus = new osg::Uniform("ltDiffus", diffus);
		osg::Uniform* ltAmbient = new osg::Uniform("ltAmbient", ambient);
		osg::Uniform* ltSpecular = new osg::Uniform("ltSpecular", specular);
		stateSet->addUniform(ltPos);
		stateSet->addUniform(eyePos);
		stateSet->addUniform(ltDiffus);
		stateSet->addUniform(ltAmbient);
		stateSet->addUniform(ltSpecular);
		stateSet->addUniform(ltIntensity);
		stateSet->setAttributeAndModes(profram);
		UniformHandler* handler = new UniformHandler(stateSet);
		viewer->addEventHandler(handler);

	}
	
	viewer->run();
}