#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include "wrapper/irrLib.hpp"

#define ARR_SIZE 6
#define BALL_SIZE 0.2
#define TILE_SIZE 11

enum map_tiles_enum {
	STRAIGHT,
	ANGLE,
	HOLE 
};

void mapGenerator() {
	char* path;
	int currentTile = 0, prev_tile = 0, turn = 0;
	f32 x, y, z;
	int currentTileArr[ARR_SIZE] = {0, 1, 1, 0, 0, 2};
	vector3df currPosition, currRotation;
	IMeshSceneNode* map_tile[ARR_SIZE];

	for (int i = 0; i < ARR_SIZE; i++)
		currentTileArr[i] = 0 + rand() % (sizeof(map_tiles_enum) - 2);

	for (int i = 0; i < ARR_SIZE; i++) {
		currentTile = currentTileArr[i];

		if(i == ARR_SIZE - 1) {
			currentTile = HOLE;
		}
		
		if(!i) {
			currentTile = STRAIGHT;
			currPosition = vector3df(0, 0, 0);
		}
		else {
			if(turn) {
				if(prev_tile == ANGLE || currentTile == ANGLE || currentTile == HOLE)
					x = map_tile[i - 1]->getPosition().X + TILE_SIZE - (TILE_SIZE / 3);
				else
					x = map_tile[i - 1]->getPosition().X + TILE_SIZE;
				currRotation = vector3df(0, 90, 0);
			}
			else
				if(prev_tile == ANGLE || currentTile == ANGLE || currentTile == HOLE)
					z = map_tile[i - 1]->getPosition().Z + TILE_SIZE - (TILE_SIZE / 3);
				else
					z = map_tile[i - 1]->getPosition().Z + TILE_SIZE;
		}

		if(currentTile == STRAIGHT) 
			path = "media/Converted/Main/straight.obj";
			
		if(currentTile == HOLE) 
			path = "media/Converted/Main/hole.obj";
			
		if(currentTile == ANGLE) {
			path = "media/Converted/Main/angle.obj";

			if(prev_tile == ANGLE) {
				if(turn)
					x = map_tile[i - 1]->getPosition().X + (TILE_SIZE / 2);
				else
					z = map_tile[i - 1]->getPosition().Z + (TILE_SIZE / 2);
			}

			turn++;
			
			if(turn > 1) { 
				turn = 0;
				currRotation += vector3df(0, 90, 0);
			}
		}

		currPosition = vector3df(x, y, z);
		map_tile[i] = oLoadStatModel(path, "", currPosition, false);
		map_tile[i]->setRotation(currRotation);

		prev_tile = currentTile;
	}

	for (int i = 0; i < ARR_SIZE; i++)
		cout << currentTileArr[i] << " ";

	cout << endl;
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
	camera->setPosition(ball->getPosition() + vector3df(0, 50, -15));
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

