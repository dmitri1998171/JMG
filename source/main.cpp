#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include "wrapper/irrLib.hpp"

#define ARR_SIZE 10
#define BALL_SIZE 0.2

enum map_tiles_enum {
	STRAIGHT,
	ANGLE,
	HOLE 
};

void mapGenerator() {
	int j = 1;
	int currentTile = 0;
	int previousTile = 0;
	vector3df currentVector;
	IMeshSceneNode* map_tile[ARR_SIZE];

	for (int i = 0; i < ARR_SIZE; i++) {
		currentTile = 0 + rand() % 2;
		
		cout << "currentTile: " << currentTile << endl;

		if(previousTile == ANGLE) {
			currentVector = vector3df(j * 11, 0, ((i - 1) * 11));
			j++;
		}
		else
			currentVector = vector3df(0, 0, (i * 11));

		if(currentTile == STRAIGHT)
			map_tile[i] = oLoadStatModel("media/Converted/Main/straight.obj", "", currentVector, false);
		
		if(currentTile == ANGLE)
			map_tile[i] = oLoadStatModel("media/Converted/Main/angle.obj", "", vector3df(0, 0, (i * 11)), false);
	
		previousTile = currentTile;
	}
}

int main() {
	int fps = 1;
	int lastFPS = -1;

	srand(time(0));
	context.device = createDevice(EDT_SOFTWARE, dimension2d<u32>(800, 600), 16, false, false, false, &context.receiver);
	u32 timeNow = context.device->getTimer()->getTime();

	if (!context.device)
		return 1; 

	context.driver = context.device->getVideoDriver();
	context.smgr = context.device->getSceneManager();

	// Map generator
	mapGenerator();
	// oLoadStatModel("media/Converted/Main/straight.obj", "", vector3df(0, 0, 0), false);
	
	// Ball and golf club
	ISceneNode* ball = context.smgr->addSphereSceneNode(BALL_SIZE);
	ball->setPosition(vector3df(0, 1, 0));

	// ISceneNode* golfClub = context.smgr->addCubeSceneNode(BALL_SIZE, 0, 0, vector3df(0, 0, -2));
	// golfClub->setScale(vector3df(1, 5, 1));

	// Camera and cursor
	ICameraSceneNode* camera = context.smgr->addCameraSceneNodeFPS();
	// camera->setParent(ball);
	camera->setPosition(ball->getPosition() + vector3df(0, 5, -15));
	camera->setTarget(ball->getPosition());

	context.device->getCursorControl()->setVisible(false);

	while(context.device->run()) {
		context.driver->beginScene(true, true, SColor(255,113,113,133));
		
		context.smgr->drawAll();
		context.smgr->getGUIEnvironment()->drawAll();

		context.driver->endScene();

		fps = context.driver->getFPS();

		if (lastFPS != fps) {
			stringw tmp(L"Just Mini Golf [");
			tmp += L"fps: ";
			tmp += fps;
			tmp += L"]";

			context.device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}

	context.device->drop();
	return 0;
}

