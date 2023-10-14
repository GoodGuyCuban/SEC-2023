#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "airport_db.h"

using namespace std;

vector<pair<string, vector<int>>> parseCSVcolumn(string filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "File not found" << endl;
        exit(0);
    }

    vector<pair<string, vector<int>>> parsedColumn;

    string line;
    string colname;
    int val;

    if (file.good())
    {
        getline(file, line);
        stringstream s(line);

        while (getline(s, colname, ','))
        {

            parsedColumn.push_back({colname, vector<int>{}});
        }
    }

    while (getline(file, line))
    {
        stringstream s(line);

        int i = 0;

        while (s >> val)
        {
            parsedColumn.at(i).second.push_back(val);

            if (s.peek() == ',')
                s.ignore();

            i++;
        }
    }

    file.close();

    return parsedColumn;
}

int main()
{
    Gate test;
    for (int i = 0; i < 1440; i++)
        cout << test.time_slots[i];

    vector<pair<string, vector<int>>> data = parseCSVcolumn("data.csv");
    cout << schedule((data[1]).second, data[0].second, data[2].second, 45);

    return 0;
}

int schedule(vector<int> arvCities, vector<int> depCities, vector<int> departureTimes, int processingTime)
{
    vector<Gate> Gates;
    Gates.resize(arvCities.size());

    for (int i = 0; i < Gates.size(); i++)
    {
        for (int j = 0; i < 1440; i++)
            Gates[i].time_slots[j] = 0;
    }

    int totalGates = 0;

    for (int flight = 0; flight < arvCities.size(); flight++) // For loop for looping over flights
    {

        for (int gate = 0; gate < Gates.size(); gate++) // For loop for looping over the gates
        {

            if (arvCities[flight] == 1) // Checking if the flight is arriving
            {
                for (int time = departureTimes[flight]; time < max(departureTimes[flight] + processingTime, 1440); time++)
                {
                    if (Gates[gate].time_slots[time])
                        continue;
                }

                for (int time = departureTimes[flight]; time < max(departureTimes[flight] + processingTime, 1440); time++)
                {
                    Gates[gate].time_slots[time] = 1;
                }
                if (!Gates[gate].partially_booked)
                    Gates[gate].partially_booked = true;
            }

            else if (depCities[flight] == 1)
            {
                for (int time = min(departureTimes[flight] - processingTime, 0); time < departureTimes[flight]; time++)
                {
                    if (Gates[gate].time_slots[time])
                        continue;
                }

                for (int time = min(departureTimes[flight] - processingTime, 0); time < departureTimes[flight]; time++)
                {
                    Gates[gate].time_slots[time] = 1;
                }
                if (!Gates[gate].partially_booked)
                    Gates[gate].partially_booked = true;
            }

            else
            {
                cout << "Incorrectly Registered Flight, #" << flight;
            }

            if (Gates[gate].partially_booked)
                totalGates += 1;
        }
    }

    return totalGates;
}