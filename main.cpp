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

    if (!file.is_open()) // check if file is open
    {
        cout << "File not found" << endl;
        exit(0);
    }

    vector<pair<string, vector<int>>> parsedColumn; // initialize return vector

    string line; // initialize variables
    string colname;
    int val;

    if (file.good()) // check if file is good
    {
        getline(file, line);
        stringstream s(line);

        while (getline(s, colname, ','))
        {

            parsedColumn.push_back({colname, vector<int>{}});
        }
    }

    while (getline(file, line)) // read data, line by line
    {
        stringstream s(line);

        int i = 0;

        while (s >> val) // read each row
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
    int input;
    vector<pair<string, vector<int>>> data;
    cout << "5000 (type 1) or 50000 (type 2): ";
    cin >> input;
    if (input == 1)
        data = parseCSVcolumn("data_5000.csv");
    if (input == 2)
        data = parseCSVcolumn("dataBig.csv");
    vector<Gate> gates = schedule((data[1]).second, data[0].second, data[2].second, 45);

    return 0;
}

vector<Gate> schedule(vector<int> arvCities, vector<int> depCities, vector<int> departureTimes, int processingTime)
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
        // Converting a city number into a mock travel time
        int travelTime = depCities[flight] / 3;

        if (departureTimes[flight] + travelTime > 1440)
        {
            cout << "Flight #" << flight << " must be booked for arrival on the next day\n";
            continue;
        }

        for (int gate = 0; gate < Gates.size(); gate++) // For loop for looping over the gates
        {

            bool breakOut = false;
            if (arvCities[flight] == 1) // Checking if the flight is arriving
            {
                bool edited = false;
                for (int time = departureTimes[flight] + travelTime; time < min(departureTimes[flight] + travelTime + processingTime, 1439); time++)
                {
                    if (Gates[gate].time_slots[time])
                        breakOut = true;
                }

                if (breakOut)
                    continue;

                edited = true;
                for (int time = departureTimes[flight] + travelTime; time < min(departureTimes[flight] + travelTime + processingTime, 1439); time++)
                {
                    Gates[gate].time_slots[time] = 1;
                }

                if (!Gates[gate].partially_booked)
                {
                    Gates[gate].partially_booked = true;
                }

                if (edited)
                    break;
                continue;
            }

            else if (depCities[flight] == 1)
            {
                bool edited = false;
                for (int time = max(departureTimes[flight] - processingTime, 0); time < departureTimes[flight]; time++)
                {
                    if (Gates[gate].time_slots[time])
                        breakOut = true;
                }

                if (breakOut)
                    continue;

                edited = true;
                for (int time = max(departureTimes[flight] - processingTime, 0); time < departureTimes[flight]; time++)
                {
                    Gates[gate].time_slots[time] = 1;
                }
                if (!Gates[gate].partially_booked)
                {
                    Gates[gate].partially_booked = true;
                }
                if (edited)
                    break;
            }

            else
            {
                cout << "Incorrectly Registered Flight, #" << flight;
            }
        }
    }

    for (int i = 0; i < Gates.size(); i++)
        if (Gates[i].partially_booked)
        {
            totalGates += 1;
        }

    cout << "Total Gates: " << totalGates << endl;
    return Gates;
}