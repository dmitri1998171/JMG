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
	std::string base_path = "media/Converted/Main/";
	std::string path;
	int currentTile = 0, prev_tile = 0, turn = 0;
	f32 x, y, z;
	int currentTileArr[ARR_SIZE] = {0, 1, 1, 0, 0, 2};
	vector3df currPosition, currRotation;
	IMeshSceneNode* map_tile[ARR_SIZE];

	// for (int i = 0; i < ARR_SIZE; i++)
	// 	currentTileArr[i] = 0 + rand() % (sizeof(map_tiles_enum) - 2);

	for (int i = 0; i < ARR_SIZE; i++) {
		currentTile = currentTileArr[i];

		if(i == ARR_SIZE - 1) {
			currentTile = HOLE;
			currentTileArr[i] = currentTile;
		}
		
		if(!i) {
			currentTile = STRAIGHT;
			currentTileArr[i] = currentTile;
			currPosition = vector3df(0, 0, 0);
		}
		else {
			if(turn > 0) {
				x = map_tile[i - 1]->getPosition().X;

				if(prev_tile == ANGLE || currentTile == ANGLE || currentTile == HOLE)
					x += TILE_SIZE - (TILE_SIZE / 3);
				else
					x += TILE_SIZE;

				currRotation = vector3df(0, 90, 0);
			}
			else if(turn < 0) {
				x = map_tile[i - 1]->getPosition().X;

				if(prev_tile == ANGLE || currentTile == ANGLE || currentTile == HOLE)
					x -= TILE_SIZE - (TILE_SIZE / 3);
				else
					x -= TILE_SIZE;

				currRotation = vector3df(0, 90 * turn, 0);
			}
			else {
				z = map_tile[i - 1]->getPosition().Z;

				if(prev_tile == ANGLE || currentTile == ANGLE || currentTile == HOLE)
					z += TILE_SIZE - (TILE_SIZE / 3);
				else
					z += TILE_SIZE;
			}
		}

		if(currentTile == STRAIGHT) 
			path.append(base_path + "straight.obj");
			
		if(currentTile == HOLE) 
			path.append(base_path + "hole.obj");

		if(currentTile == ANGLE) {
			path.append(base_path + "angle.obj");

			// Два поворота подряд
			if(prev_tile == ANGLE) {
				if(turn > 0)
					x = (map_tile[i - 1]->getPosition().X + (TILE_SIZE / 2));
				else if(turn < 0) 
					x = (map_tile[i - 1]->getPosition().X + (TILE_SIZE / 2)) * (-1);
				else
					z = map_tile[i - 1]->getPosition().Z + (TILE_SIZE / 2);
			}

			if(0 + rand() % 2) {
				turn--;
				currRotation += vector3df(0, 90, 0);
			}else {
				turn++;
			}
			
			if(turn > 1) { 
				turn = 0;
				currRotation += vector3df(0, 90, 0);
			}
			if(turn < -1) { 
				turn = 0;
				currRotation = vector3df(0, -90, 0);
			}
		}

		currPosition = vector3df(x, y, z);
		map_tile[i] = oLoadStatModel(path.c_str(), "", currPosition, false);
		map_tile[i]->setRotation(currRotation);

		currRotation = vector3df(0, 0, 0);
		prev_tile = currentTile;
		path.clear();
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

