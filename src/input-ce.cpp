#ifdef CE
#include "input.h"
#include "util.h"
#include <keypadc.h>

namespace Input {
	uint8_t lastData[7];
	uint8_t data[7];
	void init(){
	}
	void _updateKeys(){
		kb_Scan();
		memcpy(lastData, data, 7*sizeof(uint8_t));
		for(int i = 0; i < 7; i++)
			data[i] = kb_Data[i];
	}
	bool keyDown(int key){
		if(key == -1)
			return false;
		return data[key / 8] & (1 << (key % 8));
	}
	bool keyDownLast(int key){
		if(key == -1)
			return false;
		return lastData[key / 8] & (1 << (key % 8));
	}
}
#endif
