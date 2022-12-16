// Datastructures.hh
//
// Student name: Mikel Robredo Manero
// Student email: mikel.robredomanero@tuni.fi
// Student number: 150167534

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <list>
#include <map>
#include <cmath>
#include <queue>
#include <stack>
#include <unordered_set>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    //
    // Old assignment 1 operations
    //

    // Estimate of performance: O(1)
    // Short rationale for estimate: Constant - Method size() of container unordered_map takes this efficiency to check the map size.
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear operation as it deals with components of the containers with size n.
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: For loop goes over all the map through the iterator in order to push it back into the vector.
    std::vector<StationID> all_stations();

    // Estimate of performance: O(n)
    // Short rationale for estimate: find method takes as worst case n iterations, and worst case of inserting a value
    //                               by insert method in unordered_map is n.
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find method takes as worst case size n as the time complexity. Then there's nothing significant in the f()
    Name get_station_name(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find method takes as worst case size n as the time complexity. Then there's nothing significant in the f()
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: Since the map ordered alphabetically does all the work, only the for loop for ID push back into the
    //                               return vector makes a time complexity of size n.
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Sort algorithm drives the function to be n*logn through comparisons, appart from other methods in the
    //                               function.
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: find method is the unique method implemented, and it's complexity for the worst case is size n.
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Methods find and erase specially take as worst case a complexity level of size n.
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Worst of the cases would be driven by the method find, which takes a complexity of size n.
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Same as in add_departure, non nested methods like find and erase, take as worst time complexity size n.
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Among all the methods used, sort algorithm drives the function to have nlogn worst case time complexity.
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: find method takes as worst case size n as the time complexity. Then there's nothing significant in
    //                               the f()
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For loop goes over all the map through the iterator in order to push it back into the vector.
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(n)
    // Short rationale for estimate: find method takes as worst case size n as the time complexity. Then there's nothing significant
    //                               in the f().
    Name get_region_name(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find method takes as worst case size n as the time complexity. Then there's nothing significant
    //                               in the f()
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: two non-nested find methods in which their worst case would be n time complexity. Also, push-back method
    //                               for list of subregions can suppose O(1).
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: two non nested find methods for unordered_map in which their worst case is O(n), then a find_if algorithm
    //                               in which the worst case would be n time complexity as well. Also, the used push_back method for list is constant on time.
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: two non-nested find method and find_if algorithm in which their worst case would be n time complexity.
    //                               However here the push_back method goes for a vector, so time complexity would be n, so for  overall
    //                               f() it'd be still n.
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: one find method with size n complexity, and a nested operation of a while loop with copy_n that
    //                               that would be activated in the case a region has subregions, so, not n but logn complexity.
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: There are several different operations that contain n complexity, but used sort algorithm
    //                               supposes a time complexity of nlogn in terms of container size.
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find unordered_map method takes as worst case size n as the time complexity.
    //                               Then there's an erase method for unordered_map but it's time complexity in this
    //                               case would be constant as we are specifying the element to erase.
    bool remove_station(StationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Among other operations held in the function, the nested operations of a find_if algorithm
    //                               inside a while loop makes the complexity nlogn, since find_if takes n time complexity, and
    //                               the while loop doesn't compulsorily mean that the size of parent pointers will be n, and
    //                               each iteration will make the list smaller, so logn.
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

    //
    // New assignment 2 operations
    //

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: This function is composed of two help functions.
    //                               - First, addition_check drives a worst case of O(nlogn)
    //                               through the loop over station times in which then each station is found from stations main container.
    //                               - Same worst case is found in station_check but this function and previously mentioned for loop aren't
    //                               nested.
    bool add_train(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Main time complexity source is driven by the find method used to fetch the station through it's id.
    //                               And the for loop used to push back elements into the return vector.
    std::vector<StationID> next_stations_from(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Same as before, complexity of size n comes from the use of find method, and the while loop used to
    //                               iterate through the train stops of the given train.
    std::vector<StationID> train_stations_from(StationID stationid, TrainID trainid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For loop and for each algorithm to loop over all stations and trains and delete their memory allocations.
    void clear_trains();

    // Estimate of performance: O(n+m)
    // Short rationale for estimate: - BFS is the implemented algorithm, in its worst case, BFS will go over all the nodes in the graph of
    //                               size n.
    //                               - But also, each time it sticks to one node, it will loop over all its adjacent nodes, which means
    //                               that in its worst case it will drive a size m time complexity.
    std::vector<std::pair<StationID, Distance>> route_any(StationID fromid, StationID toid);

    // Non-compulsory operations

    // Estimate of performance: O(n+m)
    // Short rationale for estimate: BFS has been implemented too, indeed, the function route_any is called to compute this function.
    std::vector<std::pair<StationID, Distance>> route_least_stations(StationID fromid, StationID toid);

    // Estimate of performance: O(n+m)
    // Short rationale for estimate: - DFS is the implemented algorithm; here, first all nodes of the graphs are set to their initial stage (n).
    //                               - Then, in the worst case the while loop will make x node be all the nodes but the source node (m+1).
    //                               - All remaining methods will mean complexity m since they will be run the number of times x node neighbour has at each time.
    std::vector<StationID> route_with_cycle(StationID fromid);

    // Estimate of performance: O(n + mlogm)
    // Short rationale for estimate: - Each node in the graph is set to its initial stage in order to be processed, then this takes size (n).
    //                               - Each node will be processed in the while loop in a way that for each will loop over all the adjacent
    //                               nodes of each node, hence, size (m).
    //                               - But since there can be corrections on some adjacent nodes' distances
    //                               and if we use a heap as priority queue, then many additions for the same node can happen in the priority
    //                               queue, hence (mlogm).
    std::vector<std::pair<StationID, Distance>> route_shortest_distance(StationID fromid, StationID toid);

    // Estimate of performance: O(n + mlogm)
    // Short rationale for estimate: - Each node in the graph is set to its initial stage in order to be processed, then this takes size (n).
    //                               - Each node will be processed in the while loop in a way that for each will loop over all the adjacent
    //                               nodes of each node, hence, size (m).
    //                               - But since there can be corrections on some adjacent nodes' distances
    //                               and if we use a heap as priority queue, then many additions for the same node can happen in the priority
    //                               queue, hence (mlogm).
    std::vector<std::pair<StationID, Time>> route_earliest_arrival(StationID fromid, StationID toid, Time starttime);

private:

    enum Color {WHITE, GRAY, BLACK};

    // Definition of the data structure Train with it's attributes
    struct Train {
        TrainID trainid = NO_TRAIN;
        std::map<Time, StationID> stationStops;
    };

    struct SearchInfo {
        Distance distance;
        Color color;
        Time duration;
        Time fastDeparture;
        std::map<Time, Time> arrivals;
        std::map<Time, Time> departureTimes;
    };

    // Definition of the data structure Station with it's attributes
    // (NODE of the GRAPH)
    struct Station {
        StationID stationid = NO_STATION;
        Name stationName = NO_NAME;
        Coord locationXY = NO_COORD;
        std::unordered_map<TrainID, Time> departures;
        RegionID region = NO_REGION;

        SearchInfo* search_info;
        Station* parentNode = nullptr;

        std::vector<std::pair<Station*, Time>> toNeighbours; // Each next station and the departure time.
        std::map<Time, Station*> fromNeighbours;
    };

    // Definition of the data structure Region with it's attributes
    struct Region {
            RegionID regionid = NO_REGION;
            Name regionName = NO_NAME;
            std::vector<Coord> areaXY = {NO_COORD};
            // Since we may have to remove it later, and we don't wont invalidation problems.
            std::unordered_map<StationID, Station*> regStations;
            std::unordered_map<RegionID, Region*> subregions;
            Region *parent = nullptr;
        };

    // Data structure for the storage of stations:
    std::unordered_map<StationID, Station*> stations;

    // Help data structure for station locations.
    std::unordered_map<Coord, StationID, CoordHash> stationCoord;

    // Help data structure for station names.
    std::map<Name, StationID> stationsName;

    // Data structure for the storage of regions:
    std::unordered_map<RegionID, Region*> regions;

    // Data structure for the storage of trains:
    std::unordered_map<TrainID, Train*> trains;

    // HELP FUNCTIONS:
    Distance distance_calculator(Coord currentXY, Coord nextXY);

    // Check if all the stations in which the given trains stops exist.
    bool station_check (std::vector<std::pair<StationID, Time>> stationtimes);

    // Fills all the data structures.
    bool addition_check(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes);

    // Sets all the nodes to the initial stage to be processed by traversal algorithms
    void colorizeAndClean();

    // Compares if the current node could have a shorter distance from a second parent node.
    void relax(Station* mainNode, Station* neighbour);

    // Compares if the current node could have a faster duration from a second new parent node.
    void relax_time(Station* mainNode, std::pair<Station*, Time> neighbour);
};

#endif // DATASTRUCTURES_HH
