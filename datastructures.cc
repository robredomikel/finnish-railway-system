// Datastructures.cc
//
// Student name: Mikel Robredo Manero
// Student email: mikel.robredomanero@tuni.fi
// Student number: 150167534

#include "datastructures.hh"
#include <random>
#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    for (auto &station : stations){

        delete station.second->search_info;
        delete station.second;
    }
    for (auto &region : regions){
        delete region.second;
    }

    for (auto &train : trains){
        delete train.second;
    }
}

/**
 * @brief Datastructures::station_count counts the amount of station data objects inside the used container.
 * @return Size of the container, which is equal to the count of elements.
 */
unsigned int Datastructures::station_count()
{
    return stations.size();
}

/**
 * @brief Datastructures::clear_all clears the container where all the stations are stored, thus, it deletes all the stations.
 */
void Datastructures::clear_all()
{
    if (stations.empty() == false){
        for (auto &station : stations){
            delete station.second->search_info;
            delete station.second;
        }
        stations.clear();
        stationCoord.clear();
        stationsName.clear();
    }
    if (regions.empty() == false) {
        for (auto &region : regions){
            delete region.second;
        }
        regions.clear();
    }

    if (trains.empty() == false) {
        for (auto& train : trains){
            delete train.second;
        }
        trains.clear();
    }
}

/**
 * @brief Datastructures::all_stations creates a vector to be printed with all the existing stations.
 * @return a vector of type StationID with the id numbers of each and every existing station
 */
std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> all_stations = {};
    all_stations.reserve(stations.size());

    for (auto iter = stations.begin(); iter != stations.end();  ++iter){
        all_stations.push_back(iter->first);
    }
    return all_stations;
}

/**
 * @brief Datastructures::add_station adds a station to the main container of stations of this container
 * @param id identification number of the station to be added
 * @param name name of the station to be added
 * @param xy Coordinates of the station to be added
 * @return true and addition of the station to the container if the station doesn't already exist, else false and does nothing.
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{   
    // Addition to our data structure:
    // We find if the station's id already exists in the railway system or not through this iterator.
    const auto& iter = stations.find(id);

    // If the new id doesn't exist in the railway system, we add the station and return TRUE.
    if (iter == stations.end()){

        // Departures list for the new station:
        std::unordered_map<TrainID, Time> departures;

        SearchInfo* info = new SearchInfo;
        info->distance = 0;
        info->color = WHITE;
        info->arrivals = {};

        Station* stat = new Station;
        stat->stationid = id;
        stat->stationName = name;
        stat->locationXY = xy;
        stat->departures = departures;
        stat->region = NO_REGION;
        stat->parentNode = nullptr;
        stat->search_info = info;
        stat->toNeighbours = {};

        stations.insert({id, stat});
        stationCoord.insert({xy, id});
        stationsName.insert({name, id});

        return true;
    } else {
        return false;
    }
}

/**
 * @brief Datastructures::get_station_name Finds the station through its id and gets the name.
 * @param id identification number of the station to be processed
 * @return if the station exists, returns its name. Else, returns NO_NAME
 */
Name Datastructures::get_station_name(StationID id)
{
    const auto& iter = stations.find(id);

    if (iter != stations.end()){
        return iter->second->stationName;
    } else {
        return NO_NAME;
    }
}

/**
 * @brief Datastructures::get_station_coordinates finds the station through its id and gets its coordinates
 * @param id identification number of the station to be processed
 * @return if the station exists, its coordinates, else NO_COORD
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    const auto& iter = stations.find(id);

    if (iter != stations.end()){
        return iter->second->locationXY;
    } else {
        return NO_COORD;
    }
}

/**
 * @brief Datastructures::stations_alphabetically prints the existing stations in alphabetical order
 * @return a vector of station ids ordered in alphabetical order of stations' names
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    // Return vector:
    std::vector<StationID> sortedStations = {};
    sortedStations.reserve(stationsName.size());

    for (auto iter1 = stationsName.begin(); iter1 != stationsName.end(); ++iter1){
        sortedStations.push_back(iter1->second);
    }
    return sortedStations;
}

/**
 * @brief Datastructures::stations_distance_increasing prints the existing stations based on their euclidean distance from the origin location, which is (0,0)
 * @return a vector of station ids with the station ids placed in order based on the location of each station.
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<Station*> unsorted = {};
    unsorted.reserve(stations.size());

    for (auto iter1 = stations.begin(); iter1 != stations.end(); ++iter1){
        unsorted.push_back(iter1->second);
    }

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
                else {
                    return distR < distL;
                }
            }
        };

    std::sort(unsorted.begin(), unsorted.end(), comparisons);
    std::vector<StationID> sorted = {};
    sorted.reserve(unsorted.size());

    for (auto& iter : unsorted){
        sorted.push_back(iter->stationid);
    }

    return sorted;
}

/**
 * @brief Datastructures::find_station_with_coord finds the station of interest based on the given coordinates.
 * @param xy coordinates of type Coord to be used to find the station of interest
 * @return if no station exists with the given coordinates returns NO_STATION, else it returns the station id.
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    const auto& iter = stationCoord.find(xy);
    if (iter == stationCoord.end()){
        return NO_STATION;
    }
    else {
        return iter->second;
    }
}

/**
 * @brief Datastructures::change_station_coord changes the coordinates of the station with the given id.
 * @param id identication number of the given station
 * @param newcoord New coordinates to be used in the coordinate change of the station.
 * @return if no station with the given id exists returns False, if it does exist, it changes the coordinates and returns True.
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    const auto& iter = stations.find(id);

    if (iter == stations.end()){
        return false;
    }
    else {
        // change in the coords map
        Coord& oldCoord = iter->second->locationXY;
        stationCoord.erase(oldCoord);
        stationCoord.insert({newcoord, id});

        iter->second->locationXY = newcoord;
        return true;
    }
}

/**
 * @brief Datastructures::add_departure adds a train departure to the given station.
 * @param stationid id number of the given station where the departure must be added.
 * @param trainid train id of the departure to be added.
 * @param time time of the given departure to be added.
 * @return if no station with the given id exists, or if the station exists but the given train already exists, return False and
 *         does nothing. If the station exists but the departure doesn't, it adds the departure to the station and returns True.
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    const auto& iter = stations.find(stationid);
    if (iter == stations.end()){
        return false;
    }
    else {
        const auto& iter2 = iter->second->departures.find(trainid);

        if (iter2 != iter->second->departures.end()) {
            return false;
        }
        else {
            iter->second->departures.insert({trainid, time});
            return true;
        }
    }
}

/**
 * @brief Datastructures::remove_departure removes the departure with the specified departure details from the given station.
 * @param stationid id number of the given station
 * @param trainid id number of the given departure train
 * @param time time of the given departure train
 * @return False if the station doesn't exist or if the departure with the given details doesn't exist in the given station.
 *         True if both station and departure exist so that the removal is done.
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    const auto& iter = stations.find(stationid);

        if (iter == stations.end()){
            return false;
        }
        else {
            const auto& iter2 = iter->second->departures.find(trainid);

            if (iter2 == iter->second->departures.end() || iter2->second != time) {
                return false;
            }
            else {
                iter->second->departures.erase(iter2);
                return true;
            }
        }
}

/**
 * @brief Datastructures::station_departures_after shows the departure trains and time from the given station at the given time
 * @param stationid id number of the given station
 * @param time time of the given train depature
 * @return vector with NO_TIME and NO_TRAIN if the station doesn't exist, sorted list of pairs of time and trainid if the station
 *         exists even if there're no departures and the vector is empty.
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    const auto& iter = stations.find(stationid);

        if (iter == stations.end()){
            return {{NO_TIME, NO_TRAIN}};
        }
        else {
            std::vector<std::pair<Time, TrainID>> departs = {};
            departs.reserve(iter->second->departures.size());

            std::for_each(iter->second->departures.begin(), iter->second->departures.end(), [&time, &departs](const auto& departure){

                if (departure.second >= time) {
                    departs.push_back(std::make_pair(departure.second, departure.first));
                }
            });

            // Make the size of the vector cover only the trains that fulfill the time constraint.
            departs.resize(std::distance(departs.begin(), departs.end()));
            // Sorting the vector
            std::sort(departs.begin(), departs.end());

            return departs;
        }
}

/**
 * @brief Datastructures::add_region adds a region to the chosen container.
 * @param id id of the given region
 * @param name name of the given region
 * @param coords location coordinates of the given region
 * @return False if the region with the given id already exists, True if it doesn't and adds the region to the container.
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    // Addition to our data structure:
    // We find if the station's id already exists in the railway system or not through this iterator.
    const auto& iter = regions.find(id);

    // If the new id doesn't exist in the railway system, we add the station and return TRUE.
    if (iter == regions.end()){

        std::unordered_map<StationID, Station*> regStations;
        std::unordered_map<RegionID, Region*> subregions;

        Region* current_region = new Region;
        current_region->regionid = id;
        current_region->regionName = name;
        current_region->areaXY = coords;
        current_region->regStations = regStations;
        current_region->subregions = subregions;

        regions.insert({id, current_region});
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Datastructures::all_regions prints a list of the existing regions in the chosen container.
 * @return a vector of region ids with no specific order.
 */
std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> all_regions = {};
    all_regions.reserve(regions.size());

    for (auto& iter : regions){
        all_regions.push_back(iter.first);
    }

    return all_regions;
}

/**
 * @brief Datastructures::get_region_name gets the name of a region through its id.
 * @param id identification number of the given region.
 * @return NO_NAME if no region is found with the given id, or else the name of the station with the given id.
 */
Name Datastructures::get_region_name(RegionID id)
{
    const auto& iter = regions.find(id);

    if (iter != regions.end()){
        return iter->second->regionName;
    } else {
        return NO_NAME;
    }
}

/**
 * @brief Datastructures::get_region_coords gets the coordinates of a station through its id.
 * @param id identification number of the given region
 * @return Vector of NO_COORD if no region with the given id was found, or else a vector with the coordinates of the region with
 *         the given id
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    const auto& iter = regions.find(id);

    if (iter != regions.end()){
        return iter->second->areaXY;
    } else {
        return {NO_COORD};
    }
}

/**
 * @brief Datastructures::add_subregion_to_region adds an existing region as a subregion of another given region.
 * @param id identification name of the given region
 * @param parentid identification name of the region in which the child region will be included
 * @return False if one of the regions doesn't exist. False if the child region already has a parent region.
 *         True if both regions exist, and the given child region doesn't have a parent region yet. So the relationship gets created
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    const auto& subIter = regions.find(id);

    const auto& parIter = regions.find(parentid);

    if (( subIter == regions.end()) || (parIter == regions.end())){
        return false;
    }
    else if (subIter->second->parent != nullptr) {
        return false;
    }
    else {
        subIter->second->parent = parIter->second;
        parIter->second->subregions.insert({subIter->first, subIter->second});
        return true;
    }
}

/**
 * @brief Datastructures::add_station_to_region adds a given station to a given region.
 * @param id identification number of the given station
 * @param parentid identification name of the given region
 * @return False if station or region doesn't exist. Also False if the station is already included in the region area.
 *         Else it returns True if both previous cases aren't met.
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    const auto& stationIt = stations.find(id);
    const auto& regionIt = regions.find(parentid);

    // If station or region don't exist.
    if ( (stationIt == stations.end()) || (regionIt == regions.end()) ){
        return false;
    }
    else {

        if (stationIt->second->region != NO_REGION) {
            return false;
        } else {
            stationIt->second->region = parentid;
            regionIt->second->regStations.insert({stationIt->first, stationIt->second});
            return true;
        }
    }
}

/**
 * @brief Datastructures::station_in_regions prints the regions in which the given station exists directly or indirectly.
 * @param id identification number of the given station
 * @return A vector of type Region ID:
 *              - With NO_REGION inside if the station initially doesn't exist.
 *              - Empty vector if the region doesn't exist.
 *              - Vector with the directly connected region if this one doesn't have parent regions.
 *              - Vector with direct and indirect regions if the direct region is inside a subregion connection.
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    const auto& itStation = stations.find(id);
    const auto& itRegion = regions.find(itStation->second->region);

    if (itStation == stations.end()) {
        return {NO_REGION};
    }
    else if (itRegion == regions.end()){
        return {};
    }
    else {
        std::vector<RegionID> outputRegs = {};
        outputRegs.reserve(regions.size());
        outputRegs.push_back(itRegion->first);
        if (itRegion->second->parent == nullptr){
            return outputRegs;
        } else {
            Region *p = itRegion->second->parent;
            while (p != nullptr){
                outputRegs.push_back(p->regionid);
                p = p->parent;
            }
            return outputRegs;
        }
    }
}

/**
 * @brief Datastructures::all_subregions_of_region prints all the subregions that the given region contains in its area.
 * @param id identification name of the given parent region
 * @return A vector of type RegionID:
 *              - A vector with only NO_REGION if the region doesn't exist.
 *              - Empty vector if the parent region doesn't have any subregion.
 *              - A vector of the direct subregions, and all the subregions that belong indirectly to the given region
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    const auto& itRegion = regions.find(id);

    if (itRegion == regions.end()){ // If the region doesn't exist.
        return {NO_REGION};
    } else { // Start looping along the tree.
        if (itRegion->second->subregions.empty() == true){  // If the region doesn't have children.


                return {}; // Return empty vector.
        } else {

            std::vector<RegionID> outputRegs = {};
            outputRegs.reserve(regions.size());
            auto reg = itRegion->second->subregions.begin(); // Region pointer

            while (reg != itRegion->second->subregions.end()) { // Until we reach the end of the loop.

                outputRegs.push_back((*reg).first); // Add the current region id.
                if ((*reg).second->subregions.empty() == true) {
                    ++reg; // If the region doesn't have subregions, then go to the next.
                } else {
                    std::vector<RegionID> tempVec = all_subregions_of_region((*reg).first);
                    outputRegs.insert(outputRegs.end(), tempVec.begin(), tempVec.end());
                    ++reg;
                }
            }
            return outputRegs;
        }
    }
}


/**
 * @brief Datastructures::stations_closest_to shows the 3 closest stations, or less if there exist less, of the given coordinates
 * @param xy Given coordinates
 * @return Vector of type StationID with the stations with smallest euclidean distance from the given coordinates
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    std::vector<Station*> unsorted = {};
    unsorted.reserve(stations.size());
    for (auto iter1 = stations.begin(); iter1 != stations.end(); ++iter1){
        unsorted.push_back(iter1->second);
    }

    const auto& comparisons = [&xy](const auto& left_sta,const auto& right_sta){

        Distance distL = std::sqrt(std::pow(left_sta->locationXY.x - xy.x, 2) + std::pow(left_sta->locationXY.y - xy.y, 2));
        Distance distR = std::sqrt(std::pow(right_sta->locationXY.x - xy.x, 2) + std::pow(right_sta->locationXY.y - xy.y, 2));

        if (distL != distR){
            return distL < distR;
        }
        else {
            if (left_sta->locationXY.y <= right_sta->locationXY.y){
                return distL < distR;
            }
            else {
                return distR < distL;
            }
        }
    };

    std::sort(unsorted.begin(), unsorted.end(), comparisons);

    std::vector<StationID> sorted = {};
    sorted.reserve(3);

    for (auto& iter2 : unsorted){
        if (sorted.size() == 3){
            break;
        }
        sorted.push_back(iter2->stationid);
    }

    return sorted;
}

/**
 * @brief Datastructures::remove_station removes the station with the given id from the container of stations.
 * @param id identification number of the given station
 * @return False if no station with the given id exists, True if the station exists and it deletes it from the container
 */
bool Datastructures::remove_station(StationID id)
{
    const auto& itStation = stations.find(id);

    if (itStation == stations.end()){
        return false;
    } else {

        // Remove in the coords map
        Coord& oldCoord = itStation->second->locationXY;
        stationCoord.erase(oldCoord);

        // Remove in the names map
        Name& oldName = itStation->second->stationName;
        stationsName.erase(oldName);

        stations.erase(itStation);
        return true;
    }
}

/**
 * @brief Datastructures::common_parent_of_regions gets the id name of the parent of the given regions.
 * @param id1 identification name of the given first region
 * @param id2 identification name of the given second region
 * @return NO_REGION if:
 *              - Any station with the given ids doesn't exist.
 *              - Any of the stations doesn't have a parent (Means at this stage that it would be the root)
 *              - If no parent (direct or not) of the first region exists where the second region belongs.
 *
 *         Region id of the first parent region in which both regions belong.
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    const auto& region1 = regions.find(id1);
    const auto& region2 = regions.find(id2);
    Region *parent1 = region1->second->parent;
    Region *parent2 = region2->second->parent;

    if ( (region1 == regions.end()) || (region2 == regions.end()) ) { // Some of the regions doesn't exist.
        return NO_REGION;
    } else if ( (parent1 == nullptr) || (parent2 == nullptr) ){ // Some of the regions doesn't have a parent (it's the root)
        return NO_REGION;
    } else {
        while (parent1 != nullptr){

            if (parent1->subregions.find(id2) != parent1->subregions.end()) {
                break;
            } else {
                parent1 = parent1->parent;
            }
        }

        if (parent1 == nullptr){
            return NO_REGION;
        } else {
            return parent1->regionid;
        }
    }
}

/**
 * @brief Datastructures::add_train creates Train objects which are added to the main container of trains, and sets the neighbouring information
 *        of each station based on the connections made by each train.
 * @param trainid Identification char of the given train.
 * @param stationtimes Vector of pairs with ID of each station and the time in which the train departs from it. (Special case for the last
 *        station for which the time means the arrival time since the train does not depart from here.)
 * @return TRUE - If the given train didn't exist and the stations in which it stops do exist, hence, all the necessary operations are done.
 *         FALSE -  If the train with the given id already exist, then it cannot be added again.
 *         FALSE - If any of the stations in the route doesn't exist, then this route cannot be created.
 */
bool Datastructures::add_train(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes)
{
    const auto& iter = trains.find(trainid);
    if (iter != trains.end()) { // If the train already exists.
        return false;
    }

    else {
        if (addition_check(trainid, stationtimes) == false){
            return false;
        }
        return true;
    }
}

/**
 * @brief Datastructures::next_stations_from prints out the immediatee next stations from the given station based on the trains that depart
 *        from this station
 * @param id ID char of the given station id
 * @return Vector of NO_STATION - If the station with the given id doesn't exist.
 *         Empty vector - If the station doesn't actually have any train departure.
 *         Vector with Station IDs - If the station does exist and there are trains that depart from it.
 */
std::vector<StationID> Datastructures::next_stations_from(StationID id)
{
    const auto& station = stations.find(id);
    if (station == stations.end()){
        return {NO_STATION};
    }
    else {
        if (station->second->departures.empty() == true) {
            return {};
        }
        std::vector<StationID> next_stations;
        next_stations.reserve(station->second->departures.size());

        for (auto& next : station->second->toNeighbours){
            next_stations.push_back(next.first->stationid);
        }

        return next_stations;
    }
}

/**
 * @brief Datastructures::train_stations_from Prints the list of stations from which the given train stops by after departure from the
 *        given station
 * @param stationid ID char of the given station.
 * @param trainid ID char of the given train.
 * @return Vector of NO_STATION - If the given train or station don't exist.
 *         Vector of NO_STATION - If both happen to exist but the given station is the last one in the given route.
 *         Vector of Station IDs - If both happen to exist and there are still stations to stop by within the route.
 */
std::vector<StationID> Datastructures::train_stations_from(StationID stationid, TrainID trainid)
{
    const auto& train = trains.find(trainid);
    const auto& station = stations.find(stationid);

    if (train == trains.end() ||
            station == stations.end() ||
            station->second->departures.find(trainid) == station->second->departures.end()){

        return {NO_STATION};
    }
    else {
        std::vector<StationID> incomingStations = {};
        incomingStations.reserve(train->second->stationStops.size());

        const auto& departure = station->second->departures.find(trainid); // TrainID & Time
        auto current_station = train->second->stationStops.find(departure->second);

        ++current_station;
        if (current_station != train->second->stationStops.end()){

            while (current_station != train->second->stationStops.end()){
                incomingStations.push_back(current_station->second);
                ++current_station;
            }
            return incomingStations;
        }
        else {
            return {NO_STATION};
        }
    }
}

/**
 * @brief Datastructures::clear_trains removes all the trains from the data structures, same does with th stations.
 */
void Datastructures::clear_trains()
{
    if (!(trains.empty())){
        for (auto& train : trains){
            delete train.second;
        }
        trains.clear();

        std::for_each(stations.begin(), stations.end(), [](auto& station)
        {
            //delete station.second->search_info;
            station.second->departures.clear();
            station.second->toNeighbours.clear();
            //delete station.second;
        });
        /*
        stations.clear();
        */
    }
}

/**
 * @brief Datastructures::route_any prints any (arbitrary) route between the given stations so that it contains as less stations as possible.
 * @param fromid Station ID from where the route would start.
 * @param toid Station ID where the route would end.
 * @return Vector of pair NO_STATION, NO_DISTANCE - If any of the given stations doesn't exist.
 *         Empty vector - If there's non existing route between the given two stations.
 *         Vector of pair station, distance - Vector showing the shortest path between the given two stations, in which each stop station
 *                                            is shown with their distance from the source station.
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_any(StationID fromid, StationID toid)
{
    const auto& from = stations.find(fromid);
    const auto& to = stations.find(toid);

    if (from == stations.end() || to == stations.end()){ // if any of the stations doesn't exist
        return {{NO_STATION, NO_DISTANCE}};
    }
    else if (from->second->locationXY == to->second->locationXY){ // If both stations are the same (weird case)
        return {{to->first, 0}};
    }
    else { // BFS algorithm approach.
        colorizeAndClean(); // Make all stations white.

        from->second->search_info->color = GRAY;
        std::vector<std::pair<StationID, Distance>> path;
        path.reserve(stations.size());

        std::queue<std::pair<int, Station*>> queue;
        queue.push({0, from->second}); // source, and it's distance.
        while (queue.empty() == false) {
            std::pair<int, Station*>& x = queue.front();
            queue.pop();

            if (x.second->locationXY == to->second->locationXY){ // if goal node is found.

                path.push_back({to->first, x.second->search_info->distance}); // push it to the return path
                Station* pathNode = x.second->parentNode; // start going through its parents to build the path till the source.
                while (pathNode != nullptr){

                    path.push_back({pathNode->stationid,pathNode->search_info->distance});

                    pathNode = pathNode->parentNode;
                }
                break;
            }

            // If the source is still not found, we check the adjacents of the current node x.
            for (auto& iter : x.second->toNeighbours){
                if (iter.first->search_info->color == WHITE){ // If we discover a new node, we identify it.
                    iter.first->search_info->color = GRAY;
                    iter.first->parentNode = x.second;
                    // NOTE: The distance of the new identified node will be the addition of it's distance to its parent + the
                    // distance the parent was bookeeping from the source.
                    iter.first->search_info->distance = distance_calculator(x.second->locationXY, iter.first->locationXY)
                            + x.second->search_info->distance;
                    queue.push({iter.first->search_info->distance, iter.first});
                }
            }
            x.second->search_info->color = BLACK;
        }
        if (queue.empty() == true && path.empty() == true){ // If we checked all stations and the path is empty, means
            return {};                                      // that the final station wasn't found.
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
}

/**
 * @brief Datastructures::route_least_stations prints a route of stations and their distances from one given station to another given station
 *        so that the route contains as few stations as possible.
 * @param fromid Station ID of the given station where the route starts.
 * @param toid Station ID of the given station where the route ends.
 * @return Vector of pair NO_STATION, NO_DISTANCE - If any of the given stations doesn't exist.
 *         Empty vector - If there's non existing route between the given two stations.
 *         Vector of pair station, distance - Vector showing the shortest path between the given two stations, in which each stop station
 *                                            is shown with their distance from the source station.
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_least_stations(StationID fromid, StationID toid)
{
    std::vector<std::pair<StationID, Distance>> path = route_any(fromid, toid);
    return path;
}

/**
 * @brief Datastructures::route_with_cycle Creates a route from the given station so that it shows if any cycle can happen along the route.
 * @param fromid Station ID char of the given station where the route would start.
 * @return Vector of NO_STATION - The station with the given id doesn't exist
 *         Empty vector - If there's no cycle within all the possible routes from the given station.
 *         Vector of Station IDs - Vector showing the stations of the route in which the train would make a cycle, that's, it would stop
 *                                 more than once in the same train station.
 */
std::vector<StationID> Datastructures::route_with_cycle(StationID fromid)
{
    const auto& from = stations.find(fromid);

    if (from == stations.end()){
        return {NO_STATION};
    }
    colorizeAndClean();

    std::vector<StationID> path;
    path.reserve(stations.size());

    std::stack<Station*> stack;
    stack.push(from->second);

    while (!(stack.empty())) {

        Station* repeated = nullptr;
        Station* node = stack.top();
        stack.pop();

        if (node->search_info->color == WHITE) {

            node->search_info->color = GRAY;
            stack.push(node);

            for (auto iter : node->toNeighbours){
                if (iter.first->search_info->color == WHITE){
                    stack.push(iter.first);
                    iter.first->parentNode = node;
                }
                else if (iter.first->search_info->color == GRAY){

                    repeated = iter.first;
                    break;
                }
            }
            if (repeated != nullptr){
                path.push_back(repeated->stationid);
                Station* pathNode = node;
                while (pathNode != nullptr){
                    path.push_back(pathNode->stationid);
                    pathNode = pathNode->parentNode;
                }
                break;
            }
        }
        else { node->search_info->color = BLACK; }
    }
    if (stack.empty() == true && path.empty() == true){ // If we checked all stations and the path is empty, means
        return {};                                      // that the final station wasn't found.
    }
    std::reverse(path.begin(), path.end());
    return path;
}

/**
 * @brief Datastructures::route_shortest_distance returns a route between the given stations so that the length of the route distance is
 *        is as short as possible.
 * @param fromid Station ID char of the given station where the route would start.
 * @param toid Station ID char of the given station where the route would end.
 * @return Vector of pair NO_STATION, NO_DISTANCE - If any of the given stations doesn't exist.
 *         Empty vector - If there's non existing route between the given two stations.
 *         Vector of pair station, distance - Vector showing the path with shortest distance between the given two stations,
 *                                            in which each stop station is shown with their distance from the source station.
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_shortest_distance(StationID fromid, StationID toid)
{
    const auto& from = stations.find(fromid);
    const auto& to = stations.find(toid);

    if (from == stations.end() || to == stations.end()){
        return {{NO_STATION, NO_DISTANCE}};
    }

    colorizeAndClean(); // Set all the attributes to their initial stages.
    std::vector<std::pair<StationID, Distance>> path;
    path.reserve(stations.size());

    // Give source node appropiate values
    from->second->search_info->color = GRAY;
    from->second->search_info->distance = 0;

    // Priority queue is MAX-HEAP as default, but I'll use it as MIN-HEAP by introducing its values in negative format, so that
    // the priority queue will set the opposite order of the pair values, which is what we need in Dijkstra.
    std::priority_queue<std::pair<Distance, Station*>> queue;
    queue.push({from->second->search_info->distance, from->second});

    while (!(queue.empty())){
        auto x = queue.top();
        queue.pop();

        if (x.second->stationid == toid){

            path.push_back({x.second->stationid, (-1)*x.first});
            Station* pathNode = x.second->parentNode;
            while (pathNode != nullptr){
                path.push_back({pathNode->stationid, pathNode->search_info->distance});
                pathNode = pathNode->parentNode;
            }
            break;
        }

        for (auto& neighbour : x.second->toNeighbours){

            // RELAX step:
            Distance old = neighbour.first->search_info->distance;
            relax(x.second, neighbour.first);

            if (neighbour.first->search_info->color == WHITE){ // New node discovered
                neighbour.first->search_info->color = GRAY;
                neighbour.first->parentNode = x.second;

                neighbour.first->search_info->distance = x.second->search_info->distance
                        + distance_calculator(x.second->locationXY, neighbour.first->locationXY);
                // Distance added as negative so that min-heap is used.
                queue.push({(-1)*(neighbour.first->search_info->distance), neighbour.first});
            }
            else { // Means that this node was already discovered.

                if ( (neighbour.first->search_info->distance < old) && (neighbour.first->search_info->color != BLACK) ){

                    // This means that there is a shorter way to reach this node.
                    queue.push({(-1)*(neighbour.first->search_info->distance), neighbour.first});
                }
            }
        }
        // This node has been fully processed.
        x.second->search_info->color = BLACK;

    }

    if (queue.empty() == true && path.empty() == true){
        return {};
    }

    std::reverse(path.begin(), path.end());
    return path;
}

/**
 * @brief Datastructures::route_earliest_arrival returns a route between the given stations so that the time of the route journey is
 *        is as short as possible.
 * @param fromid Station ID char of the given station where the route would start.
 * @param toid Station ID char of the given station where the route would end.
 * @param starttime
 * @return Vector of pair NO_STATION, NO_DISTANCE - If any of the given stations doesn't exist.
 *         Empty vector - If there's non existing route between the given two stations.
 *         Vector of pair station, distance - Vector showing the path with shortest route time between the given two stations,
 *                                            in which each stop station is shown with their distance from the source station.
 */
std::vector<std::pair<StationID, Time>> Datastructures::route_earliest_arrival(StationID fromid, StationID toid, Time starttime)
{
    const auto& from = stations.find(fromid);
    const auto& to = stations.find(toid);

    if (from == stations.end() || to == stations.end()){
        return {{NO_STATION, NO_TIME}};
    }

    // If there's no departure from the station after the start station, then there's no possible route.
    if (from->second->search_info->departureTimes.upper_bound(starttime) == from->second->search_info->departureTimes.end()){
        return {};
    }


    colorizeAndClean(); // Set all the attributes to their initial stages.
    std::vector<std::pair<StationID, Time>> path;
    path.reserve(stations.size());

    // Give source node appropiate values
    from->second->search_info->color = GRAY;
    from->second->search_info->duration = 0;

    // Priority queue is MAX-HEAP as default, but I'll use it as MIN-HEAP by introducing its values in negative format, so that
    // the priority queue will set the opposite order of the pair values, which is what we need in Dijkstra.
    std::priority_queue<std::pair<int, Station*>> queue;
    queue.push({0, from->second}); // the duration at the initial station will be 0

    while (!(queue.empty())){
        auto x = queue.top();
        queue.pop();

        /*
        if (x.first >= 0){
            //x.second->search_info->color = BLACK;
            continue;
        }
        */

        // If the current node is the end station, then we reached the shortest route.
        if (x.second->stationid == toid){

            // We add the arrival time
            path.push_back({x.second->stationid, x.second->search_info->fastDeparture});
            Station* pathNode = x.second->parentNode;
            while (pathNode != nullptr){

                // We add the departure time of all the rest of stations in the route.
                path.push_back({pathNode->stationid, pathNode->search_info->fastDeparture});
                pathNode = pathNode->parentNode; // And move again to the previous station in the route.
            }
            break;
        }

        for (auto neighbour : x.second->toNeighbours){

            // If the departure time of the neighbour is before the departure time of the current node, then we directly omit it.
            if (neighbour.second < x.second->search_info->fastDeparture
                    || neighbour.second < starttime){
                continue;
            }

            // RELAX step:
            Time old = neighbour.first->search_info->duration; // Current duration that the route takes to reach the neighbour station

            // We check if the neighbour would have a faster route through the new x node.
            relax_time(x.second, neighbour);

            if (neighbour.first->search_info->color == WHITE){ // New node discovered

                // We store the fast departure time of the parent node so that we can fetch it if the route has been found.
                if (x.second->stationid == fromid){

                    // If there's no departure at the starting time from node x, and the next departure is later than starttime
                    // , we take the next one.
                    if (neighbour.first->fromNeighbours.find(starttime) == neighbour.first->fromNeighbours.end() &&
                            neighbour.first->fromNeighbours.upper_bound(starttime)->first > starttime){

                        x.second->search_info->fastDeparture = neighbour.first->fromNeighbours.upper_bound(starttime)->first;

                    } // NOTE: Map is a red-black tree, thus a Balanced tree, so find method takes only O(logn)

                    // If the current node has already been processed with the beginning starttime.
                    else if (x.second->search_info->fastDeparture == starttime &&
                             neighbour.first->fromNeighbours.upper_bound(starttime)->first > starttime){

                        x.second->search_info->fastDeparture = neighbour.first->fromNeighbours.upper_bound(starttime)->first;
                    }
                    else {
                        x.second->search_info->fastDeparture = starttime;

                    }
                }
                else if (neighbour.first->fromNeighbours.find(x.second->search_info->fastDeparture) == neighbour.first->fromNeighbours.end()
                         && neighbour.first->fromNeighbours.upper_bound(x.second->search_info->fastDeparture)->first > starttime)
                {
                    // We bookeep the initial departure time of the first station thanks to the information stored in the next station.
                        x.second->search_info->fastDeparture = neighbour.first->
                                fromNeighbours.upper_bound(x.second->search_info->fastDeparture)->first;
                } // NOTE: Map is a red-black tree, thus a Balanced tree, so find method takes only O(logn)


                // NOTE: If we find that inside the next station, its previous station constains a
                // departure time which is  equal or even higher than the one the neighbour has, then
                // it doesn't make any sense. We just consider departures that indeed come after the
                // departure time of each x current node.
                if (x.second->search_info->fastDeparture >= neighbour.second){
                    continue;
                }

                neighbour.first->search_info->color = GRAY;
                neighbour.first->parentNode = x.second;

                // The fastest departure of the neighbour (when the train will arrive to that station) is will be the arrival information
                // from the departure time of previous station)
                neighbour.first->search_info->fastDeparture = neighbour.first->search_info->
                        arrivals.find(x.second->search_info->fastDeparture)->second;

                neighbour.first->search_info->duration = x.second->search_info->duration
                        + (neighbour.first->search_info->fastDeparture
                           - x.second->search_info->fastDeparture);

                // Only if the departure time is within the same day
                if (neighbour.first->search_info->duration > 0){
                    // Duration added as negative so that min-heap is used.
                    queue.push({(-1)*(neighbour.first->search_info->duration), neighbour.first});
                }
            }
            else { // Means that this node was already discovered.

                // Only if the new duration after RELAX_TIME is shorter than the old durantion, if the neighbour isn't yet processed
                // as BLACK node, and if departure time is within the same day.
                if ( (neighbour.first->search_info->duration < old) && (neighbour.first->search_info->color != BLACK) &&
                     neighbour.first->search_info->duration > 0){

                    // This means that there is a shorter way to reach this node.
                    queue.push({(-1)*(neighbour.first->search_info->duration), neighbour.first});
                    neighbour.first->search_info->fastDeparture = neighbour.first->search_info->fastDeparture;
                }
            }
        }
        // This node has been fully processed.
        x.second->search_info->color = BLACK;

    }

    if (queue.empty() == true && path.empty() == true){
        return {};
    }

    std::reverse(path.begin(), path.end());
    return path;
}

/**
 * @brief Datastructures::distance_calculator calculates the euclidean distance between the given two locations
 * @param currentXY Coordinates of the given source location
 * @param nextXY Coordinates of the given next location
 * @return Euclidean distance between the two locations.
 */
Distance Datastructures::distance_calculator(Coord currentXY, Coord nextXY)
{
    Distance distance = std::floor(std::sqrt( (std::pow(currentXY.x - nextXY.x,2)) + (std::pow(currentXY.y - nextXY.y,2)) ));
    return distance;
}

/**
 * @brief Datastructures::station_check Checks if all the stations from the given train route exist in the graph.
 * @param stationtimes Vector of pair StationID, Time with the given station IDs and the train departure time.
 * @return TRUE - All the stations in the route exist.
 *         FALSE - Any of the stations does not exist.
 */
bool Datastructures::station_check(std::vector<std::pair<StationID, Time> > stationtimes)
{
    for (auto iter = stationtimes.begin(); iter != stationtimes.end(); ++iter){

        if (stations.find(iter->first) == stations.end()){ // If the station doesn't exist.
            return false;
        }
    }
    return true;
}

/**
 * @brief Datastructures::addition_check based on the station check, performs all the low level additions for the add_train function
 * @param trainid Id char of the given train to add into the graph
 * @param stationtimes Vector of pair StationID, Time with the given station IDs and the train departure time.
 * @return TRUE - Station_check returns true, and all the Train object and the connections between stations have been correctly made.
 *         FALSE -  Station_check returns false, hence some station does not exist, so the connections and the train cannot exist.
 */
bool Datastructures::addition_check(TrainID trainid, std::vector<std::pair<StationID, Time> > stationtimes)
{
    if (station_check(stationtimes) != true){
        return false;
    }
    else {

        // station stops ordered timewise for each train's information.
        std::map<Time, StationID> stationStops;

        // We create an object Train:
        Train* train = new Train;
        train->trainid = trainid;
        train->stationStops = stationStops;

        Station* previous = nullptr;
        std::pair<StationID, Time> prev_departure;

        for (auto& iter : stationtimes){
            const auto& station = stations.find(iter.first);

            station->second->departures.insert({trainid, iter.second}); // Departures added.
            train->stationStops.insert({iter.second, iter.first}); // Train stops added.

            if (previous == nullptr){ // if the station is the first one, it should be the source of that route.
                previous = station->second;
                prev_departure = iter;
            }
            else{
                // if it's not, then the new station is the NEXT neighbour of the previous station.
                previous->toNeighbours.push_back({station->second,iter.second});
                // Also, the previous station is PREVIOUS neighbout of the current station.
                station->second->fromNeighbours.insert({previous->departures.at(trainid), previous});
                station->second->search_info->arrivals.insert({prev_departure.second, iter.second});
                previous->search_info->departureTimes.insert({prev_departure.second, iter.second});
                prev_departure = iter;
                previous = station->second;
            }
        }

        trains.insert({trainid, train});
        return true;
    }
}

/**
 * @brief Datastructures::colorizeAndClean Sets all the graph stations into the initial stage so that they can be processed by the
 *        traversal algorithms.
 */
void Datastructures::colorizeAndClean()
{
    std::for_each(stations.begin(), stations.end(), [](auto& station) {
        station.second->search_info->color = WHITE;
        station.second->parentNode = nullptr;
        station.second->search_info->distance = 0;
        station.second->search_info->duration = 0;
        station.second->search_info->fastDeparture = 0;
    });
}

/**
 * @brief Datastructures::relax compares if the old distance from the source to the given neighbour node is longer or shorter than the
 *        the distance that it would suppose if the path would come from the new parent node
 * @param mainNode Parent node to which neighbout node is adjacent.
 * @param neighbour Adjacent node of mainNode (Node where the route from the source comes from)
 */
void Datastructures::relax(Station *mainNode, Station *neighbour)
{
    if (neighbour->search_info->distance > ( (mainNode->search_info->distance)
                                             + distance_calculator(mainNode->locationXY, neighbour->locationXY) )){

        // Shorter path than the current path has been found. Hence, we update the connection information for the neighbour.
        neighbour->search_info->distance = (mainNode->search_info->distance)
                + distance_calculator(mainNode->locationXY, neighbour->locationXY);
        neighbour->parentNode = mainNode;
    }

}

/**
 * @brief Datastructures::relax_time
 * @param mainNode
 * @param neighbour
 */
void Datastructures::relax_time(Station *mainNode, std::pair<Station*, Time> neighbour)
{
    // If the duration recorded for the neighbour is higher than the new duration given from the journey time mainNode to the
    // neighbour station, then we update the duration of the neighbour station.
    if (neighbour.first->search_info->duration > ( (mainNode->search_info->duration)
                                             + (neighbour.second - mainNode->search_info->fastDeparture) )){
        // Where the Time component of neighbour is the Time in which the train departs from that station.

        /*
        if (mainNode == neighbour.first->parentNode){
            mainNode->search_info->fastDeparture = neighbour.first->search_info
                    ->arrivals.upper_bound(mainNode->search_info->fastDeparture)->first;
        }
        */
        if (mainNode == neighbour.first->parentNode &&
                neighbour.first->search_info->arrivals.upper_bound(mainNode->search_info->fastDeparture) !=
                neighbour.first->search_info->arrivals.end()){
            mainNode->search_info->fastDeparture = neighbour.first->search_info
                    ->arrivals.upper_bound(mainNode->search_info->fastDeparture)->first;

            neighbour.first->search_info->duration = (mainNode->search_info->duration)
                    + (neighbour.second - mainNode->search_info->fastDeparture);

            neighbour.first->search_info->fastDeparture = neighbour.first->search_info->arrivals.find(mainNode->search_info->fastDeparture)->second;
        }
        // Shorter path than the current path has been found. Hence, we update the connection information for the neighbour.
        neighbour.first->search_info->duration = (mainNode->search_info->duration)
                + (neighbour.second - mainNode->search_info->fastDeparture);
        // neighbour.first->search_info->duration = (mainNode->search_info->duration)
        //         + (neighbour.first->search_info->fastDeparture - mainNode->search_info->fastDeparture);


        neighbour.first->parentNode = mainNode;
    }
}
