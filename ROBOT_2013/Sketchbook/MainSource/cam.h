#include "common.h"

class cam {
	public:
		bool inZone () {
			return true;
		}
		bool inbetweenZones() {
			return true;
		}
		enum COLOR getBayColor() {
			return RED;
		}
};