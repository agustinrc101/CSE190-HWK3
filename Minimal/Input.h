#pragma once
#ifndef INPUT_H
#define INPUT_H

#define INPUT_STICK_SMOOTHING 0.01f

#include <glm/glm.hpp>

class Input {
public:
	//Setters
	static void setIndexTriggerL(bool b) { indexTriggerL = b; }
	static void setIndexTriggerR(bool b) { indexTriggerR = b; }
	static void setHandTriggerL(bool b) { handTriggerL = b; }
	static void setHandTriggerR(bool b) { handTriggerR = b; }
	static void setButtonA(bool b) { buttonA = b; }
	static void setButtonB(bool b) { buttonB = b; }
	static void setButtonX(bool b) { buttonX = b; }
	static void setButtonY(bool b) { buttonY = b; }
	static void setStickL(glm::vec2 v) { stickL.x = v.x * INPUT_STICK_SMOOTHING; stickL.y = v.y * INPUT_STICK_SMOOTHING; }
	static void setStickR(glm::vec2 v) { stickR.x = v.x * INPUT_STICK_SMOOTHING; stickR.y = v.y * INPUT_STICK_SMOOTHING; }
	static void setButtonStickL(bool b) { buttonStickL = b; }
	static void setButtonStickR(bool b) { buttonStickR = b; }
	//Getters
	static bool getIndexTriggerL() { return indexTriggerL; }
	static bool getIndexTriggerR() { return indexTriggerR; }
	static bool getHandTriggerL() { return handTriggerL; }
	static bool getHandTriggerR() { return handTriggerR; }
	static bool getButtonA() { return buttonA; }
	static bool getButtonB() { return buttonB; }
	static bool getButtonX() { return buttonX; }
	static bool getButtonY() { return buttonY; }
	static glm::vec2 getStickL() { return stickL; }
	static glm::vec2 getStickR() { return stickR; }
	static bool getButtonStickL() { return buttonStickL; }
	static bool getButtonStickR() { return buttonStickR; }

protected:
	//Index Trigger
	static bool indexTriggerL;
	static bool indexTriggerR;
	//Hand Trigger
	static bool handTriggerL;
	static bool handTriggerR;
	//Buttons
	static bool buttonA;
	static bool buttonB;
	static bool buttonX;
	static bool buttonY;
	//Sticks
	static glm::vec2 stickL;
	static glm::vec2 stickR;
	static bool buttonStickL;
	static bool buttonStickR;
};

#endif