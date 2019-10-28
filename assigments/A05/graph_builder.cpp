///////////////////////////////////////////////////////////////////////////////
//
// Date:             28 October 2019
// Title:            Program 3 - Graphs Part 1
// Semester:         Fall 2019
// Course:           CMPS 3013 
// Assignment:       A05
// Author:           Carter Case
// Email:            ccase0412@my.msutexas.edu
// Files:            Geo.hpp, json.hpp, JsonFacade.hpp, cities.json
// Description:      This program will read in a list of cities from 
//                   a json file and calculate the distance between
//                   each city. The program will then print each city
//                   and the user's number of closest cities to that city.
//       
/////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include "Heap.hpp"
#include "JsonFacade.hpp"
#include "json.hpp"
#include "Geo.hpp"
using namespace std;
using json = nlohmann::json;


struct City
{
    int Population;
    string Name;
    double longitude;
    double latitude;
    Coordinate coord;
    int rank;
    string state;
    double Priority;


    City()
    {
        Population = 0;
        Name = "";
        longitude = 0.0;
        latitude = 0.0;
        Coordinate coord(0.0,0.0);
        rank = 0;
        state = " ";
        Priority = 0.0;
    }

    //json constructor
    City(json obj)
    {
        Name = obj["city"];
        Population = obj["population"];
        longitude = obj["longitude"];
        latitude = obj["latitude"];
        Coordinate coord(longitude, latitude);
        rank = obj["rank"];
        state = obj["state"];
        Priority = 0.0;
    }
};

 
int main()
{
    ofstream ofile;
    ofile.open("output.txt");
    json obj;
    City **Cities;
    City *temp;
    string filename = "cities.json";
    JsonFacade J(filename);
    Heap<City> H(1000, false);

    int size = J.getSize();
    Cities = new City *[size];// array of cities
    
    for (int i = 0; i < size; i++)
      {
        obj = J.getNext();
        Cities[i] = new City(obj);
      }
    
    double distance;
    int citycount = 0;
    int cityNum;
    cin >> cityNum;// user enters how many neighbors to print


    for (int i = 0; i < 1000; i++)
    {
        //first 10 and last 10
        if (i < 10 || i > size - 11)
        {
            ofile << i + 1 << ": " << Cities[i]->Name << endl;
        }
        
        for (int j = 0; j < 1000; j++)
        {
            //computes haversine distance
            distance = HaversineDistance(Cities[i]->coord, Cities[j]->coord);
            Cities[j]->Priority = distance;
            H.Insert(Cities[j]);
        }

        while (number < 1000)
        {
            temp = H.Extract();
            //first 10 and last 10
            if (i < 10 || i > size - 11)
            {
                if (citycount <= cityNum)
                {
                    if (temp->Name != Cities[i]->Name)
                        ofile << "\t" << citycount  << ")  " << temp->Name << " " << temp->Priority << endl;
                }
            }

            citycount++;
            temp = NULL;
        }
      
        citycount = 0;// resets counter
    }
    ofile.close();
    return 0;
}
