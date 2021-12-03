#ifndef IRRLIB_HPP
#define IRRLIB_HPP

#include "irrLibReceiver.hpp"

struct SAppContext
{
	IrrlichtDevice* device;
	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;
	MyEventReceiver receiver;
};

inline SAppContext context;		// inline следит чтобы был только один экземпляр во всех файлах

IMeshSceneNode* oLoadStatModel(const char* objPath, const char* texturePath, vector3df objPosition, bool lighting);
IAnimatedMeshSceneNode* oLoadAnimModel(const char* objPath, const char* texturePath, vector3df objPosition, bool lighting);
ILightSceneNode* addLight(vector3df objPosition, SColorf lightColor, f32 radius);
void oTranslation(ISceneNode* o, vector3df objPosition);
void oMove(ISceneNode* o, vector3df objPosition);
void oTurn(ISceneNode* o, vector3df objPosition);
void addCollision(IMeshSceneNode* obj, IMeshSceneNode* to, int gravity);
bool checkCollision(ISceneNode* one, ISceneNode* two);

#endif
