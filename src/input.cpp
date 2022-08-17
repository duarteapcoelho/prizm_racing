#include "input.h"

#include "time.h"

int time = 0;
int currentRepeatInterval = 0;
int repeatTimer = 0;

namespace Input {
	int holdDelay = 0;
	int minRepeatDelay = 0;

	void update(){
		time = Time::time;
		_updateKeys();
	}

	bool keyPressed(int key){
		return keyDown(key) && !keyDownLast(key);
	}

	bool keyPressedRepeat(int key){
		if(keyPressed(key)){
			currentRepeatInterval = holdDelay;
			repeatTimer = time;
			return true;
		} else if(keyDownLast(key) && time > repeatTimer + currentRepeatInterval){
			repeatTimer = time;
			currentRepeatInterval = currentRepeatInterval / 2;
			if(currentRepeatInterval < minRepeatDelay)
				currentRepeatInterval = minRepeatDelay;
			return true;
		}
		return false;
	}

	int pressedNumber(){
		int numberKeys[10] = { KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9 };
		for(int i = 0; i < 10; i++){
			if(keyPressed(numberKeys[i])) return i;
		}
		return -1;
	}

	bool pressedDirection(int *x, int *y){
		bool pressed = false;
		*x = 0;
		*y = 0;
		if(keyPressedRepeat(KEY_UP)){
			(*y)--;
			pressed = true;
		}
		if(keyPressedRepeat(KEY_DOWN)){
			(*y)++;
			pressed = true;
		}
		if(keyPressedRepeat(KEY_LEFT)){
			(*x)--;
			pressed = true;
		}
		if(keyPressedRepeat(KEY_RIGHT)){
			(*x)++;
			pressed = true;
		}
		return pressed;
	}

	char pressedCharacter(){
		int letterKeys[26] = { KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z};
		for(int i = 0; i < 26; i++){
			if(keyPressed(letterKeys[i])){
				if(shiftDown()){
					return 'A'+i;
				} else {
					return 'a'+i;
				}
			}
		}
		return '\0';
	}

	int pressedFunctionKey(){
		int functionKeys[6] = { KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6 };
		for(int i = 0; i < 6; i++){
			if(keyPressed(functionKeys[i]))
				return i;
		}
		return -1;
	}

	bool shiftDown(){
		return keyDown(KEY_SHIFT);
	}

	bool alphaDown(){
		return false; // TODO
	}
};
