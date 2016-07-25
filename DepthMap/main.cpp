#include <osgLib.h>
#include <osgViewer/Viewer>
#include <osg/Group>
#include <osg/Camera>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osgGA/GUIEventHandler>
#include <osgDB/WriteFile>

class KeyControl : public osgGA::GUIEventHandler
{
public:
	KeyControl(osg::MatrixTransform *mt,osg::Camera *camera,osg::Texture2D *texture)
		:_mt(mt)
		,_camera(camera)
		,_texture(texture)
	{

	}
protected:
	bool handle(const osgGA::GUIEventAdapter &ea,osgGA::GUIActionAdapter &aa)
	{
		if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
		{
			if (ea.getKey() == 'a')
			{
				if (_mt)
				{
					static float fAngle = 0;
					_mt->setMatrix(osg::Matrix::rotate(fAngle,osg::Z_AXIS));
					fAngle += 0.1;

					_camera->attach(osg::Camera::DEPTH_BUFFER,_texture);
					//osgDB::writeImageFile(*_texture->getImage(0), "image.png");
				}
			}

		}

		return false;
	}
private:
	osg::MatrixTransform *_mt;
	osg::Camera *_camera;
	osg::Texture2D *_texture;
};

void initRTTCamera(osgViewer::Viewer *viewer,osg::Group *group)
{
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	group->addChild(camera);

	int tex_width = 2048,tex_height = 2048;

	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");
	mt->addChild(node);

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setTextureSize(tex_width,tex_height);
	texture->setInternalFormat(GL_DEPTH_COMPONENT);
	texture->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
	texture->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
	texture->setWrap(osg::Texture2D::WRAP_S,osg::Texture2D::CLAMP_TO_BORDER);
	texture->setWrap(osg::Texture2D::WRAP_T,osg::Texture2D::CLAMP_TO_BORDER);

	float radius = node->getBound().radius();
	camera->addChild(mt.get());
	camera->setClearDepth(1.0);
	camera->setViewport(0,0,tex_width,tex_height);
	camera->setRenderOrder(osg::Camera::PRE_RENDER);
	camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	camera->setClearColor(osg::Vec4(0.0f,0.0f,0.0f,1.0f));
	camera->attach(osg::Camera::DEPTH_BUFFER, texture.get());
	camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	camera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
	camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	camera->setProjectionMatrixAsOrtho(-radius, radius, -radius, radius, -10, 10);
	

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> vertexs = new osg::Vec3Array;
	vertexs->push_back(osg::Vec3(-10,-10,0));
	vertexs->push_back(osg::Vec3(10,-10,0));
	vertexs->push_back(osg::Vec3(10,10,0));
	vertexs->push_back(osg::Vec3(-10,10,0));
	geometry->setVertexArray(vertexs);

	osg::ref_ptr<osg::Vec2Array> coords = new osg::Vec2Array;
	coords->push_back(osg::Vec2(0,0));
	coords->push_back(osg::Vec2(1,0));
	coords->push_back(osg::Vec2(1,1));
	coords->push_back(osg::Vec2(0,1));
	geometry->setTexCoordArray(0,coords);

	geometry->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture);
	geode->addDrawable(geometry);
	group->addChild(geode);

	viewer->addEventHandler(new KeyControl(mt,camera,texture));
}

int main()
{
	osgViewer::Viewer *viewer = new osgViewer::Viewer;

	osg::ref_ptr<osg::Group> root = new osg::Group;
	initRTTCamera(viewer,root);
	viewer->setSceneData(root);
	viewer->run();

	return 0;
}