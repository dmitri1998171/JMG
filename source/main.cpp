#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <iostream>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class MyEventReceiver : public IEventReceiver {
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	
	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

int main() {
	MyEventReceiver receiver;

	IrrlichtDevice* device = createDevice(EDT_SOFTWARE, dimension2d<u32>(600, 400), 16, false, false, false, &receiver);

	if (device == 0)
		return 1; 

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	smgr->getGUIEnvironment()->addStaticText(L"Press Space to hide occluder.", recti(10,10, 200,50));

	ISceneNode * node = smgr->addSphereSceneNode(10, 64);
	if (node) {
		node->setPosition(vector3df(0,0,60));
		node->setMaterialTexture(0, driver->getTexture("media/wall.bmp"));
		node->setMaterialFlag(EMF_LIGHTING, false);
	}

	ISceneNode* plane = smgr->addMeshSceneNode(smgr->addHillPlaneMesh(
		"plane", dimension2df(10,10), dimension2du(2,2)), 0, -1,
		vector3df(0,0,20), vector3df(270,0,0));

	if (plane) {
		plane->setMaterialTexture(0, driver->getTexture("media/t351sml.jpg"));
		plane->setMaterialFlag(EMF_LIGHTING, false);
		plane->setMaterialFlag(EMF_BACK_FACE_CULLING, true);
	}

	driver->addOcclusionQuery(node, ((IMeshSceneNode*)node)->getMesh());

	smgr->addCameraSceneNode();
	int lastFPS = -1;
	u32 timeNow = device->getTimer()->getTime();
	bool nodeVisible=true;

	while(device->run()) {
		plane->setVisible(!receiver.IsKeyDown(KEY_SPACE));

		driver->beginScene(true, true, SColor(255,113,113,133));
	
		node->setVisible(nodeVisible);
		smgr->drawAll();
		smgr->getGUIEnvironment()->drawAll();

		if (device->getTimer()->getTime()-timeNow>100) {
			driver->runAllOcclusionQueries(false);
			driver->updateAllOcclusionQueries();
			nodeVisible=driver->getOcclusionQueryResult(node)>0;
			timeNow=device->getTimer()->getTime();
		}

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps) {
			stringw tmp(L"OcclusionQuery Example [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}

	device->drop();
	
	return 0;
}

