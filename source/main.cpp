#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include "wrapper/irrLib.hpp"

int main() {
	context.device = createDevice(EDT_SOFTWARE, dimension2d<u32>(600, 400), 16, false, false, false, &context.receiver);

	if (!context.device)
		return 1; 

	context.driver = context.device->getVideoDriver();
	context.smgr = context.device->getSceneManager();

	context.smgr->getGUIEnvironment()->addStaticText(L"Press Space to hide occluder.", recti(10,10, 200,50));

	ISceneNode * node = context.smgr->addSphereSceneNode(10, 64);
	if (node) {
		node->setPosition(vector3df(0,0,60));
		node->setMaterialTexture(0, context.driver->getTexture("media/wall.bmp"));
		node->setMaterialFlag(EMF_LIGHTING, false);
	}

	ISceneNode* plane = context.smgr->addMeshSceneNode(context.smgr->addHillPlaneMesh(
		"plane", dimension2df(10,10), dimension2du(2,2)), 0, -1,
		vector3df(0,0,20), vector3df(270,0,0));

	if (plane) {
		plane->setMaterialTexture(0, context.driver->getTexture("media/t351sml.jpg"));
		plane->setMaterialFlag(EMF_LIGHTING, false);
		plane->setMaterialFlag(EMF_BACK_FACE_CULLING, true);
	}

	context.driver->addOcclusionQuery(node, ((IMeshSceneNode*)node)->getMesh());

	context.smgr->addCameraSceneNode();
	int lastFPS = -1;
	u32 timeNow = context.device->getTimer()->getTime();
	bool nodeVisible=true;

	while(context.device->run()) {
		plane->setVisible(!context.receiver.IsKeyDown(KEY_SPACE));

		context.driver->beginScene(true, true, SColor(255,113,113,133));
	
		node->setVisible(nodeVisible);
		context.smgr->drawAll();
		context.smgr->getGUIEnvironment()->drawAll();

		if (context.device->getTimer()->getTime()-timeNow>100) {
			context.driver->runAllOcclusionQueries(false);
			context.driver->updateAllOcclusionQueries();
			nodeVisible=context.driver->getOcclusionQueryResult(node)>0;
			timeNow=context.device->getTimer()->getTime();
		}

		context.driver->endScene();

		int fps = context.driver->getFPS();

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

