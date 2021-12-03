#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include "wrapper/irrLib.hpp"

#define ARR_SIZE 10

enum map_tiles_enum
{
	STRAIGHT,
	ANGLE,
	HOLE 
};

int main() {
	int fps = 1;
	IMeshSceneNode* map_tile[ARR_SIZE];

	context.device = createDevice(EDT_SOFTWARE, dimension2d<u32>(640, 480), 16, false, false, false, &context.receiver);

	if (!context.device)
		return 1; 

	context.driver = context.device->getVideoDriver();
	context.smgr = context.device->getSceneManager();

	ICameraSceneNode* camera = context.smgr->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(0, 30, 0));
	camera->setTarget(vector3df(0, 0, 30));

	srand(time(0));
	int currentTile = 0;

	for (int i = 0; i < ARR_SIZE; i++) {
		currentTile = 0 + rand() % 2;
		
		cout << "currentTile: " << currentTile << endl;

		if(currentTile == STRAIGHT)
			map_tile[i] = oLoadStatModel("media/Converted/straight.obj", "", vector3df(0, 0, 30 + (i * 11)), false);
		
		if(currentTile == ANGLE)
			map_tile[i] = oLoadStatModel("media/Converted/angle.obj", "", vector3df(0, 0, 30 + (i * 11)), false);
	}
	
	int lastFPS = -1;
	u32 timeNow = context.device->getTimer()->getTime();

	while(context.device->run()) {
		context.driver->beginScene(true, true, SColor(255,113,113,133));
		
		context.smgr->drawAll();
		context.smgr->getGUIEnvironment()->drawAll();

		context.driver->endScene();

		fps = context.driver->getFPS();

		if (lastFPS != fps) {
			stringw tmp(L"OcclusionQuery Example [");
			tmp += context.driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			context.device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}

	context.device->drop();
	return 0;
}

