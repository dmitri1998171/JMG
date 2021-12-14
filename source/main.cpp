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
	int currentTile = 0, prev_tile = 0;
	int curr_x = 0, curr_z = 0;
	int turn = 0, turn_off = 0;
	int currentTileArr[ARR_SIZE] = {0, 1, 1, 0, 0, 2};
	vector3df currPosition, currRotation;
	IMeshSceneNode* map_tile[ARR_SIZE];

	// for (int i = 0; i < ARR_SIZE; i++) {
	// 	currentTile = 0 + rand() % (sizeof(map_tiles_enum) - 2);
	// 	currentTileArr[i] = currentTile;
	// 	cout << currentTile << endl;
	// }

	for (int i = 0; i < ARR_SIZE; i++) {

		currentTile = currentTileArr[i];

		cout << "curr_x: " << curr_x << "  curr_z: " << curr_z;
		// cout << "  curr_tile: " << currentTile << "  prev_tile: " << prev_tile << endl;

		if(i == 0) 					// Первый элемент - всегда прямая
			currentTile = STRAIGHT;

		if (i == ARR_SIZE - 1) 		// Последний элемент - всегда лунка
			currentTile = HOLE;

		if(currentTile == STRAIGHT) {
			path = "media/Converted/Main/straight.obj";

			if(turn) {		// Если трасса повернула
				currPosition = vector3df((curr_x * TILE_SIZE) - (TILE_SIZE / 3), 0, (curr_z * TILE_SIZE)  - (TILE_SIZE / 3));
				currRotation = vector3df(0, 90, 0);
				curr_x++;
			}
			else {
				if(turn_off) {
					currPosition = vector3df((curr_x * TILE_SIZE) - (TILE_SIZE / 2), 0, (curr_z * TILE_SIZE) - (TILE_SIZE / 2));
				}
				else {
					currPosition = vector3df((curr_x * TILE_SIZE), 0, (curr_z * TILE_SIZE));
				}

				curr_z++;
			}
		}

		if(currentTile == ANGLE) {
			path = "media/Converted/Main/angle.obj";

			if(turn) {	// Если трасса повернула (предыдущ эл-т был углом)
				currPosition = vector3df((curr_x * TILE_SIZE) - (TILE_SIZE / 2), 0, (curr_z * TILE_SIZE) - (TILE_SIZE / 3));
				currRotation = vector3df(0, 180, 0);
				turn = 0;
				turn_off = 1;
				curr_z++;
			}
			else {
				currPosition = vector3df((curr_x * TILE_SIZE), 0, (curr_z * TILE_SIZE) - (TILE_SIZE / 3));
				turn = 1;
				curr_x++;
			}
		}

		if(currentTile == HOLE) {
			path = "media/Converted/Main/hole.obj";

			if(turn)
				currPosition = vector3df((curr_x * TILE_SIZE) - (TILE_SIZE / 2) - 1, 0, (curr_z * TILE_SIZE)  - (TILE_SIZE / 3));
			else {
				if(turn_off)
					currPosition = vector3df((curr_x * TILE_SIZE) - (TILE_SIZE / 2), 0, (curr_z * TILE_SIZE) - TILE_SIZE);
				else
					currPosition = vector3df((curr_x * TILE_SIZE), 0, (curr_z * TILE_SIZE) - (TILE_SIZE / 3));
			}
		}
		
		
		// if(previousTile == ANGLE) {
		// 	if(currentTile == ANGLE) {
		// 		currPosition = vector3df((curr_z * TILE_SIZE) - (TILE_SIZE / 2), 0, ((i - 1) * TILE_SIZE) - (TILE_SIZE / 3));
		// 		currRotation = vector3df(0, 180, 0);
		// 		// turn = 0;
		// 	}

		// 	if(currentTile == STRAIGHT) {
		// 		currPosition = vector3df((curr_z * TILE_SIZE) - (TILE_SIZE / 3), 0, ((i - 1) * TILE_SIZE) - (TILE_SIZE / 3));
		// 		currRotation = vector3df(0, 90, 0);
		// 	}

		// 	if(currentTile == HOLE) {
		// 		currPosition = vector3df(((curr_z - 1) * TILE_SIZE) - (TILE_SIZE / 2), 0, ((i - 1) * TILE_SIZE) - (TILE_SIZE - (TILE_SIZE / 5)));
		// 	}

		// curr_z++;
		// 	turn = 1;
		// }
		// else 
		// 	currRotation = vector3df(0, 0, 0);

		map_tile[i] = oLoadStatModel(path, "", currPosition, false);
		map_tile[i]->setRotation(currRotation);
		
		prev_tile = currentTile;
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

