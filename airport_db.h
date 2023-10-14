#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

class Gate
{
private:
    bool fully_booked = false;

public:
    int time_slots[1440];
};

int schedule(int arvCities[], int depCities[], int departureTimes[], int totalGates);