#include "IrrLib.hpp"

// Загрузка статической модели
IMeshSceneNode* oLoadStatModel(const char* objPath, const char* texturePath, vector3df objPosition, bool lighting) {
	IAnimatedMesh* mesh = context.smgr->getMesh(objPath);
	if(!mesh) {
		context.device->drop();
		exit(1);
	}
	
	IMeshSceneNode* obj = context.smgr->addOctreeSceneNode(mesh, 0, 0);

	obj->setMaterialTexture(0, context.driver->getTexture(texturePath));
	obj->setPosition(objPosition);
	obj->setMaterialFlag(EMF_LIGHTING, lighting);

	if(lighting)
		obj->addShadowVolumeSceneNode();
		
	return obj;
}

// Загрузка анимированной модели
IAnimatedMeshSceneNode* oLoadAnimModel(const char* objPath, const char* texturePath, vector3df objPosition, bool lighting) {
	IAnimatedMesh* mesh = context.smgr->getMesh(objPath);
	if(!mesh) {
		context.device->drop();
		exit(1);
	}

	IAnimatedMeshSceneNode* obj = context.smgr->addAnimatedMeshSceneNode(mesh);
	
	obj->setMaterialTexture(0, context.driver->getTexture(texturePath));
	obj->setPosition(objPosition);
	obj->setMaterialFlag(EMF_LIGHTING, lighting);

	if(lighting)
		obj->addShadowVolumeSceneNode();
		
	return obj;
}

// Добавить источник света
ILightSceneNode* addLight(vector3df objPosition, SColorf lightColor, f32 radius) {
	ILightSceneNode* light = context.smgr->addLightSceneNode(0, objPosition, lightColor);
	light->setRadius(radius);
	return light;
}

// Перемещение объекта
void oTranslation(ISceneNode* obj, vector3df objPosition) {
	obj->setPosition(obj->getPosition() + objPosition);
}

// Движение в направлении
void oMove(ISceneNode* obj, vector3df objPosition) {
	matrix4 mat;
	vector3df move = objPosition;
	mat.setRotationDegrees(obj->getRotation());
	mat.transformVect(move);
	obj->setPosition(obj->getPosition() + move);
}

// Вращение объекта
void oTurn(ISceneNode* obj, vector3df objPosition) {
	obj->setRotation(obj->getRotation() + objPosition);
}

// Добавление аниматора коллизий
void addCollision(IMeshSceneNode* obj, IMeshSceneNode* to, int gravity) {
	ITriangleSelector* selector = context.smgr->createOctreeTriangleSelector(obj->getMesh(), obj, 128);
		obj->setTriangleSelector(selector);
	
	if (selector) {
		ISceneNodeAnimator* anim = context.smgr->createCollisionResponseAnimator(selector, to, vector3df(30,30,30), vector3df(0,gravity,0), vector3df(0,0,0));
		selector->drop();
		to->addAnimator(anim);
		anim->drop();
	}
}

// Проверка на столкновения
bool checkCollision(ISceneNode* one, ISceneNode* two) {
	return (one->getTransformedBoundingBox().intersectsWithBox(two->getTransformedBoundingBox()));
}