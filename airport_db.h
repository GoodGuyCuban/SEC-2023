#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

class Gate
{

public:
    int time_slots[1440];
    bool fully_booked = false;
    bool partially_booked = false;
};

vector<Gate> schedule(vector<int> arvCities, vector<int> depCities, vector<int> departureTimes, int processingTime);