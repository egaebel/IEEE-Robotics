#include "common.h"

class cam {
	public:
		bool inZone () {
			return true;
		}
		bool inbetweenZones() {
			return true;
		}
		bColour getBayColor() {
			return RED;
		}
                bSize detectSize() {
			return LARGE;
		}
};