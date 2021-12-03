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
	HILL,
	RISE_15,
	RISE_30,
	RISE_45,
	HOLE 
};

void mapGenerator() {
	std::string base_path = "media/Converted/";
	std::string path;
	int curr_tile = 0, prev_tile = 0, turn = 0;
	f32 x = 0, y = 0, z = 0;
	int currentTileArr[ARR_SIZE] = {STRAIGHT, RISE_45, STRAIGHT, STRAIGHT, STRAIGHT, HOLE};
	vector3df currPosition, currRotation;
	IMeshSceneNode* map_tile[ARR_SIZE];

	for (int i = 0; i < ARR_SIZE; i++)
		currentTileArr[i] = 0 + rand() % (sizeof(map_tiles_enum) - 2);

	for (int i = 0; i < ARR_SIZE; i++) {
		curr_tile = currentTileArr[i];

		if(i == ARR_SIZE - 1) {
			curr_tile = HOLE;
			currentTileArr[i] = curr_tile;
		}
		
		if(!i) {
			curr_tile = STRAIGHT;
			currentTileArr[i] = curr_tile;
			currPosition = vector3df(0, 0, 0);
		}
		else {
			if(turn > 0) {
				x = map_tile[i - 1]->getPosition().X;

				if(prev_tile == ANGLE || curr_tile == ANGLE || curr_tile == HOLE)
					x += TILE_SIZE - (TILE_SIZE / 3);
				else
					x += TILE_SIZE;

				currRotation = vector3df(0, 90, 0);
			}
			else if(turn < 0) {
				x = map_tile[i - 1]->getPosition().X;

				if(prev_tile == ANGLE || curr_tile == ANGLE || curr_tile == HOLE)
					x -= TILE_SIZE - (TILE_SIZE / 3);
				else
					x -= TILE_SIZE;

				currRotation = vector3df(0, 90 * turn, 0);
			}
			else {
				z = map_tile[i - 1]->getPosition().Z;

				if(prev_tile == ANGLE || curr_tile == ANGLE || curr_tile == HOLE)
					z += TILE_SIZE - (TILE_SIZE / 3);
				else
					z += TILE_SIZE;
			}
		}

		if(curr_tile == STRAIGHT) 
			path.append(base_path + "Main/" + "straight.obj");
			
		if(curr_tile == HOLE) 
			path.append(base_path + "Main/" + "hole.obj");

		if(curr_tile == HILL) 
			path.append(base_path + "Main/" + "hill.obj");

		if(curr_tile == ANGLE) {
			path.append(base_path + "Main/" + "angle.obj");

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

		// условие подьема проверяются после установки текущ. позиции т.к задает высоту для след. эл-та

		if(curr_tile == RISE_15) {
			path.append(base_path + "Rise/" + "rise_15.obj");
			y += 2.2;	// ?
		}
		
		if(curr_tile == RISE_30) {
			path.append(base_path + "Rise/" + "rise_30.obj");
			y += 4.7;	// ?
		}

		if(curr_tile == RISE_45) {
			path.append(base_path + "Rise/" + "rise_45.obj");
			y += 8.2;	// ?
		}

		map_tile[i] = oLoadStatModel(path.c_str(), "", currPosition, false);
		map_tile[i]->setRotation(currRotation);

		currRotation = vector3df(0, 0, 0);
		prev_tile = curr_tile;
		path.clear();
	}
	
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
	camera->setPosition(ball->getPosition() + vector3df(35, 0, 0));
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

