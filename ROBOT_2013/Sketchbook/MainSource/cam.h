#include "common.h"

class cam {
	public:
		bool inZone () {
			return true;
		}
		bool inbetweenZones() {
			return true;
		}
		bColour getBayColour() {
			return RED;
		}
        bSize detectSize() {
			return LARGE;
		}
};