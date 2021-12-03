#include <irrLibReceiver.hpp>

MyEventReceiver::MyEventReceiver() {
	for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i) {
		KeyIsDown[i] = false;
		KeyIsPressed[i] = false;
		KeyAcceptPressed[i] = true;
	}
	anyKey = false;
}

bool MyEventReceiver::OnEvent(const SEvent& event) {
	if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		if((event.KeyInput.PressedDown) && (KeyAcceptPressed[event.KeyInput.Key])) {
			KeyIsPressed[event.KeyInput.Key] = true;
			KeyAcceptPressed[event.KeyInput.Key] = false;
			anyKey = true;
		}
		if(!event.KeyInput.PressedDown) {
			KeyIsPressed[event.KeyInput.Key] = false;
			KeyAcceptPressed[event.KeyInput.Key] = true;
			anyKey = false;
		}
	}
    
	return false;
}

// Если клавиша нажата и удерживается - срабатывает несколько раз
bool MyEventReceiver::IsKeyDown(EKEY_CODE keyCode) const {
	return KeyIsDown[keyCode];
}

// Если клавиша нажата - срабатывает один раз
bool MyEventReceiver::KeyIsPress(EKEY_CODE keyCode) {
	bool pressed = KeyIsPressed[keyCode];
	KeyIsPressed[keyCode] = false;
	return pressed;
}

// Если нажата любая клавиша
bool MyEventReceiver::IsAnyKeyDown() const {
	return anyKey;
}
