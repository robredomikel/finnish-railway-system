# Finnish Railway System
This project is based on the final project required in the subject Data Structures and Algorithms 1 at Tampere University. Shortly, my work covered the content appearing in **datastructures.hh** and **datastructures.c** in which I implemented the needed data structures and functions for the correct functionality of the given GUI.
  
**Note:** Final function of the project specified later on may not print correctly some of the test cases, but overall it works fine.

## Table of contents
- [Description](https://github.com/robredomikel/finnish-railway-system#description)
- [How to run](https://github.com/robredomikel/finnish-railway-system#how-to-run)
	- [In Qt](https://github.com/robredomikel/finnish-railway-system#in-qt)
- [Main features](https://github.com/robredomikel/finnish-railway-system#main-features)
	- [STL library](https://github.com/robredomikel/finnish-railway-system#stl-library)
		- [Containers](https://github.com/robredomikel/finnish-railway-system#containers)
		- [Algorithms](https://github.com/robredomikel/finnish-railway-system#algorithms)
	- [Struct based objects](https://github.com/robredomikel/finnish-railway-system#struct-based-objects)
- [Traversal algorithms](https://github.com/robredomikel/finnish-railway-system#traversal-algorithms)
	- [Breath First Search](https://github.com/robredomikel/finnish-railway-system#breath-first-search)
	- [Depth First Search](https://github.com/robredomikel/finnish-railway-system#depth-first-search)
	- [Dijkstra's Algorithm](https://github.com/robredomikel/finnish-railway-system#dijkstras-algorithm)
- [Further comments](https://github.com/robredomikel/finnish-railway-system#further-comments)
- [References](https://github.com/robredomikel/finnish-railway-system#references)

## Description
In this project I implemented the necessary data structures and operations so that train stations, train departures and administrative regions can be added to the program, and furthermore train connections between stations can be created and shown based on different preferences. The **goal of the project** is to run every single function of the project with less than $O(n^2)$ asymptotical efficiency.

During this  README file I will mostly **go over some of the most important features** in which the project is based on, rather than the description of each function and their asymtotical efficiency can be found on the definition of the function in the **datastructures.hh** file.
## How to run
### In Qt
If you want to get visual output of what the input commands print out, then it's preferable that Qt is used since the GUI of this project was created with Qt designer by the course staff. Other IDEs could be used but then the output would be only numerical. 
In order to start running the program, you would need to open with Qt creator the pro file. Then, the same interface guides the user when clicking in any function operation, as it can be seen in the following image:
["read" command](./finnish-railways/read-command.PNG)

If you want to run several commands at once, then inside the **functional-tests folder** you could find some test functions that through the option **read** can be computed all at once. For this, the test file to be used would need to be moved to the main project folder, so that it can bw tracked by the project. 
[map display](./finnish-railways/map-example.PNG)
## Main features
### STL library
Most features of this project have been based on the use of STL library.
#### Containers
Main containers used have been map and unordered_map given their cheap cost for addition,removal and find operations.
 ```cpp
 std::unordered_map<StationID, Station*> stations;
 ```
Most of the used unordered_maps have used the standard hashing provided rules, but others have been based on the hash-table provided by the course staff for coordinates management.
 ```cpp
 std::map<Time, Station*> fromNeighbours;
 ```
Maps instead have been used as **red-black trees**, suitable for efficient sorting trees based on time.

#### Algorithms
In order to avoid expensive iterative operations, all the mainly used operations have been based on the **STL built-in** algorithms library, and also specific functions of each container used.
```cpp
const auto& comparisons = [](const auto& left_sta,const auto& right_sta){
            Distance distL = std::sqrt(std::pow(left_sta->locationXY.x, 2) + std::pow(left_sta->locationXY.y, 2));
            Distance distR = std::sqrt(std::pow(right_sta->locationXY.x, 2) + std::pow(right_sta->locationXY.y, 2));
            if (distL != distR){
                return distL < distR;
            }
            else {
                if (left_sta->locationXY.y <= right_sta->locationXY.y){
                    return distL < distR;
                }
                else {return distR < distL;}
            }
        };
 std::sort(unsorted.begin(), unsorted.end(), comparisons);
```
### Struct based objects
In this project all the necessary objects (Station, Region, Train...) have been produced as **data structures** to store all the needed attributes:
```cpp
    struct Station {
        StationID stationid = NO_STATION;
        Name stationName = NO_NAME;
        Coord locationXY = NO_COORD;
        std::unordered_map<TrainID, Time> departures;
        RegionID region = NO_REGION;

        SearchInfo* search_info;
        Station* parentNode = nullptr;

		// Each next station and the departure time.
        std::vector<std::pair<Station*, Time>>toNeighbours; 
        std::map<Time, Station*> fromNeighbours;
    };
```
## Traversal algorithms
Regional organization, stations' order and trains' itinerary are based on tree rules that, require tree-based traversal logic, when it comes to search efficiently for a specific result. Some of the algorithms used are listed below, for further information on them, wikipedia links with their logic have been attached at the end.
### Breath First Search
Used in functions ```route_any``` & ```route_least_stations```, it searches for the first existing route from a source station and a ending station, which turns to be the shortest path in terms of number of stations.
[BFS algorithm](./finnish-railways/BFS-Algorithm_Search_Way.gif)
### Depth First Search
Used in function ```route_with_cycle```, it shows the path that the train would do to **make the cycle**, that is, stopping by the same station more than once.
[DFS algorithm](./finnish-railways/dfs.gif)
### Dijkstra's Algorithm
Used in function ```route_shortest_distance```, it finds the shortest route between two stations based on the **distance weight**, which is calculated through the Euclidean distance.
[Dijkstra's algorithm](./finnish-railways/Dijkstras_progress_animation.gif)

## Further comments
This project should be seen from a **data-structures and algorithms** backed perspective. Indeed, all the implemented code has been based on this part of the project.

Also, if there's interest on knowing all the asymptotic efficiencies of each function, they can be found in the header file ```datastructures.hh``` with a short rationale.

## References
[Data Structures and Algorithms 1 (FITech) | Tampere universities (tuni.fi)](https://www.tuni.fi/en/study-with-us/data-structures-and-algorithms-1-fitech#url-controlled-expander-trigger--study-program-non-degree-298992)
[cppreference.com](https://en.cppreference.com/w/)
[Breadth-first search - Wikipedia](https://en.wikipedia.org/wiki/Breadth-first_search)
[Depth-first search - Wikipedia](https://en.wikipedia.org/wiki/Depth-first_search)
[Dijkstra's algorithm - Wikipedia](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)
