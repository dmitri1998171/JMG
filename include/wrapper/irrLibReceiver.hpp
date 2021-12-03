#ifndef IRR_LIB_RECEIVER_HPP
#define IRR_LIB_RECEIVER_HPP

#include <irrlicht.h>
#include <iostream>

using namespace std;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class MyEventReceiver : public IEventReceiver {
	private:
		bool KeyIsDown[KEY_KEY_CODES_COUNT];
		bool KeyIsPressed[KEY_KEY_CODES_COUNT];
		bool KeyAcceptPressed[KEY_KEY_CODES_COUNT];
		bool anyKey;
	
	public:
		MyEventReceiver();
		virtual bool OnEvent(const SEvent& event);
		virtual bool IsKeyDown(EKEY_CODE keyCode) const;
		virtual bool KeyIsPress(EKEY_CODE keyCode);
		virtual bool IsAnyKeyDown() const;
};
#endif