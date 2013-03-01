#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

class DistanceSensor
{
    public:
        int findDistance();
    private:
        int microsecondsToCentimeters(long time);
};

#endif // DISTANCESENSOR_H
