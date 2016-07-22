#include <osgLib.h>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

void main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer;
	osgDB::readNodeFile("../../Win32/Models/NFD_100000.ive");
	viewer->run();
}