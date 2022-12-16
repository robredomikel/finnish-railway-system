// The main program (provided by the course), TIE-20100/TIE-20106
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do so
//  (for debugging, for example), DO NOT commit any changes to git, but revert all
//  changes later. Otherwise you won't be able to get any updates/fixes to this
//  file from git!)

#include <string>
using std::string;
using std::getline;

#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::flush;
using std::noskipws;

#include <iomanip>
using std::setfill;
using std::setw;

#include <istream>
using std::istream;

#include <ostream>
using std::ostream;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::istringstream;
using std::ostringstream;
using std::stringstream;

#include <iomanip>
using std::setw;

#include <tuple>
using std::tuple;
using std::make_tuple;
using std::get;
using std::tie;

#include <regex>
using std::regex_match;
using std::regex_search;
using std::smatch;
using std::regex;
using std::sregex_token_iterator;

#include <algorithm>
using std::find_if;
using std::find;
using std::binary_search;
using std::max_element;
using std::max;
using std::min;
using std::shuffle;
using std::sort;

#include <random>
using std::minstd_rand;
using std::uniform_int_distribution;

#include <chrono>

#include <functional>
using std::function;
using std::equal_to;

#include <vector>
using std::vector;

#include <set>
using std::set;

#include <array>
using std::array;

#include <bitset>
using std::bitset;

#include <iterator>
using std::next;

#include <ctime>
using std::time;

#include <memory>
using std::move;

#include <utility>
using std::pair;
using std::make_pair;

#include <cmath>
using std::abs;

#include <cstdlib>
using std::div;

#include <algorithm>
using std::transform;

#include <iterator>
using std::back_inserter;

#include <cstddef>
#include <cassert>


#include "mainprogram.hh"

#include "datastructures.hh"

#ifdef GRAPHICAL_GUI
#include "mainwindow.hh"
#endif

string const MainProgram::PROMPT = "> ";

void MainProgram::test_get_functions(StationID id)
{
    ds_.get_station_name(id);
    ds_.get_station_coordinates(id);
}

MainProgram::CmdResult MainProgram::cmd_add_station(ostream& /*output*/, MatchIter begin, MatchIter end)
{
    StationID id = *begin++;
    string name = *begin++;
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);

    bool success = ds_.add_station(id, name, {x, y});

    view_dirty = true;
    return {ResultType::IDLIST, CmdResultIDs{{}, {success ? id : NO_STATION}}};
}

MainProgram::CmdResult MainProgram::cmd_station_info(std::ostream& /*output*/, MatchIter begin, MatchIter end)
{
    StationID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    return {ResultType::IDLIST, CmdResultIDs{{}, {id}}};
}

void MainProgram::test_station_info()
{
    if (random_stations_added_ > 0) // Don't do anything if there's no stations
    {
        auto id = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        test_get_functions(id);
    }
}

void MainProgram::test_next_stations_from()
{
    if (random_stations_added_ > 0) // Don't do anything if there's no stations
    {
        auto id = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        ds_.next_stations_from(id);
    }
}

void MainProgram::test_train_stations_from()
{
 if (random_stations_added_ > 0 && random_trains_added_ > 0) // Don't do anything if there's no stations or trains
 {
     auto stationid = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
     auto trainid = n_to_trainid(random<decltype(random_trains_added_)>(0, random_trains_added_));
     ds_.train_stations_from(stationid, trainid);
 }
}

MainProgram::CmdResult MainProgram::cmd_change_station_coord(std::ostream& /*output*/, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    StationID id = *begin++;
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);

    bool success = ds_.change_station_coord(id, {x,y});

    view_dirty = true;
    return {ResultType::IDLIST, CmdResultIDs{{}, {success ? id : NO_STATION}}};
}

MainProgram::CmdResult MainProgram::cmd_add_departure(std::ostream& output, MatchIter begin, MatchIter end)
{
    StationID stationid = *begin++;
    TrainID trainid = *begin++;
    Time time = convert_string_to<Time>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    bool success = ds_.add_departure(stationid, trainid, time);

    if (success)
    {
        output << "Train " << trainid << " leaves from station ";
        print_station_brief(stationid, output, false);
        output << " at ";
        print_time (time, output);
//        return {ResultType::IDLIST, CmdResultIDs{{}, {stationid}}};
        return {};
    }
    else
    {
        output << "Adding departure failed!" << endl;
        return {};
    }
}

void MainProgram::test_add_departure()
{
    if (random_stations_added_ > 0) // Don't do anything if there's no stations
    {
        auto id = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        auto trainid = n_to_trainid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        auto time = 100*random(0,23) + random(0,59);
        ds_.add_departure(id, trainid, time);
    }
}

MainProgram::CmdResult MainProgram::cmd_remove_departure(std::ostream &output, MatchIter begin, MatchIter end)
{
    StationID stationid = *begin++;
    TrainID trainid = *begin++;
    Time time = convert_string_to<Time>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    bool success = ds_.remove_departure(stationid, trainid, time);

    if (success)
    {
        output << "Removed departure of train " << trainid << " from station ";
        print_station_brief(stationid, output, false);
        output << " at ";
        print_time(time, output);
//        return {ResultType::IDLIST, CmdResultIDs{{}, {stationid}}};
        return {};
    }
    else
    {
        output << "Adding departure failed!" << endl;
        return {};
    }
}

void MainProgram::test_remove_departure()
{
    // Note: It's quite improbable that any departure actually gets removed (because of randomness)
    if (random_stations_added_ > 0) // Don't do anything if there's no stations
    {
        auto id = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        auto trainid = n_to_trainid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        auto time = 100*random(0,23) + random(0,59);
        ds_.remove_departure(id, trainid, time);
    }
}

MainProgram::CmdResult MainProgram::cmd_station_departures_after(std::ostream &output, MatchIter begin, MatchIter end)
{
    StationID stationid = *begin++;
    Time time = convert_string_to<Time>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    auto departures = ds_.station_departures_after(stationid, time);

    if (departures.size() == 1 && departures.front() == std::make_pair(NO_TIME, NO_TRAIN))
    {
        output << "No such station (NO_TIME, NO_TRAIN returned)" << endl;
        return {};
    }

    if (!departures.empty())
    {
        output << "Departures from station ";
        print_station_brief(stationid, output, false);
        output << " after ";
        print_time(time, output, false);
        output << ":" << endl;
        for (auto& [deptime, trainid] : departures)
        {
            output << " " << trainid << " at ";
            print_time(deptime, output);
        }
    }
    else
    {
        output << "No departures from station ";
        print_station_brief(stationid, output, false);
        output << " after ";
        print_time(time, output);
    }

//    return {ResultType::IDLIST, CmdResultIDs{{}, {stationid}}};
    return {};
}

void MainProgram::test_station_departures_after()
{
    if (random_stations_added_ > 0) // Don't do anything if there's no stations
    {
        auto id = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        auto time = 100*random(0,23) + random(0,59);
        ds_.station_departures_after(id, time);
    }
}

MainProgram::CmdResult MainProgram::cmd_add_train(std::ostream& output, MatchIter begin, MatchIter end)
{
    TrainID trainid = *begin++;
    string stationtimesstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    CmdResultTrains result;
    std::vector<std::pair<StationID,Time>> stationtimes;
    smatch stationtime;
    auto sbeg = stationtimesstr.cbegin();
    auto send = stationtimesstr.cend();
    StationID prevstation = NO_STATION;
    Time prevtime = NO_TIME;
    for ( ; regex_search(sbeg, send, stationtime, stationtimes_regex_); sbeg = stationtime.suffix().first)
    {
        StationID stationid = stationtime[1];
        Time time = convert_string_to<Time>(stationtime[2]);
        if (prevstation != NO_STATION)
        {
            result.push_back({trainid, prevstation, stationid, prevtime});
        }
        prevstation = stationid;
        prevtime = time;
        stationtimes.push_back({stationid, time});
    }
    result.push_back({trainid, prevstation, NO_STATION, prevtime});

    assert(stationtimes.size() >= 2 && "Train with <2 stations");

    bool success = ds_.add_train(trainid, stationtimes);

    if (success)
    {
        view_dirty = true;
        return {ResultType::TRAINS, result};
    }
    else
    {
        output << "Adding train failed!" << endl;
        return {};
    }
}

void MainProgram::test_change_station_coord()
{
    if (random_stations_added_ > 0) // Don't do anything if there's no stations
    {
        auto id = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        int x = random<int>(1, 10000);
        int y = random<int>(1, 10000);
        ds_.change_station_coord(id, {x,y});
    }
}

MainProgram::CmdResult MainProgram::cmd_add_subregion_to_region(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    RegionID subregionid = convert_string_to<RegionID>(*begin++);
    RegionID parentid = convert_string_to<RegionID>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    bool ok = ds_.add_subregion_to_region(subregionid, parentid);
    if (ok)
    {
        try
        {
            auto subregionname = ds_.get_region_name(subregionid);
            auto parentname = ds_.get_region_name(parentid);
            output << "Added '" << subregionname << "' as a subregion of '" << parentname << "'" << endl;
        }
        catch (NotImplemented&)
        {
            output << "Added a subregion to region." << endl;
        }
        return {ResultType::IDLIST, CmdResultIDs{{subregionid, parentid}, {}}};
    }
    else
    {
        output << "Adding a station to region failed!" << endl;
        return {};
    }
}

MainProgram::CmdResult MainProgram::cmd_add_station_to_region(std::ostream &output, MatchIter begin, MatchIter end)
{
    StationID stationid = *begin++;
    RegionID regionid = convert_string_to<RegionID>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    bool ok = ds_.add_station_to_region(stationid, regionid);
    if (ok)
    {
        try
        {
            auto stationname = ds_.get_station_name(stationid);
            auto regionname = ds_.get_region_name(regionid);
            output << "Added '" << stationname << "' to region '" << regionname << "'" << endl;
        }
        catch (NotImplemented&)
        {
            output << "Added a station to region." << endl;
        }
        return {ResultType::IDLIST, CmdResultIDs{{regionid}, {stationid}}};
    }
    else
    {
        output << "Adding a subregion failed!" << endl;
        return {};
    }
}

MainProgram::CmdResult MainProgram::cmd_stations_closest_to(std::ostream &output, MatchIter begin, MatchIter end)
{
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);

    auto stations = ds_.stations_closest_to({x,y});
    if (stations.empty())
    {
        output << "No stations!" << endl;
    }

    return {ResultType::IDLIST, CmdResultIDs{{}, stations}};
}

MainProgram::CmdResult MainProgram::cmd_common_parent_of_regions(std::ostream &output, MatchIter begin, MatchIter end)
{
    RegionID regionid1 = convert_string_to<RegionID>(*begin++);
    RegionID regionid2 = convert_string_to<RegionID>(*begin++);
    assert( begin == end && "Impossible number of parameters!");


    auto regionid = ds_.common_parent_of_regions(regionid1, regionid2);
    if (regionid == NO_REGION)
    {
        output << "No common parent region found." << endl;
    }

    return {ResultType::IDLIST, CmdResultIDs{{regionid1, regionid2, regionid}, {}}};
}

MainProgram::CmdResult MainProgram::cmd_station_in_regions(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    StationID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.station_in_regions(id);
    if (result.empty()) { output << "Station does not belong to any region." << std::endl; }
    return {ResultType::IDLIST, CmdResultIDs{result, {id}}};
}

void MainProgram::test_station_in_regions()
{
    if (random_stations_added_ > 0) // Don't do anything if there's no stations
    {
        auto id = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        ds_.station_in_regions(id);
    }
}

void MainProgram::test_all_subregions_of_region()
{
    if (random_regions_added_ > 0) // Don't do anything if there's no regions
    {
        auto id = n_to_regionid(random<decltype(random_regions_added_)>(0, random_regions_added_));
        ds_.all_subregions_of_region(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_remove_station(ostream& output, MatchIter begin, MatchIter end)
{
    string id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto name = ds_.get_station_name(id);
    bool success = ds_.remove_station(id);
    if (success)
    {
        output << name << " removed." << endl;
        view_dirty = true;
        return {};
    }
    else
    {
        return {ResultType::IDLIST, CmdResultIDs{{}, {NO_STATION}}};
    }
}

void MainProgram::test_remove_station()
{
    // Choose random number to remove
    if (random_stations_added_ > 0) // Don't remove if there's nothing to remove
    {
        auto stationid = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        ds_.remove_station(stationid);
    }
}

void MainProgram::add_random_stations_regions(unsigned int size, Coord min, Coord max)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        auto name = n_to_name(random_stations_added_);
        StationID id = n_to_stationid(random_stations_added_);

        int x = random<int>(min.x, max.x);
        int y = random<int>(min.y, max.y);

        ds_.add_station(id, name, {x, y});

        // Add a new region for every 10 stations
        if (random_stations_added_ % 10 == 0)
        {
            auto regionid = n_to_regionid(random_regions_added_);
            vector<Coord> coords;
            for (int j=0; j<3; ++j)
            {
                coords.push_back({random<int>(min.x, max.x),random<int>(min.y, max.y)});
            }
            ds_.add_region(regionid, convert_to_string(regionid), std::move(coords));
            // Add area as subarea so that we get a binary tree
            if (random_regions_added_ > 0)
            {
                auto parentid = n_to_regionid(random_regions_added_ / 2);
                ds_.add_subregion_to_region(regionid, parentid);
            }
            ++random_regions_added_;
        }

        // With a 50 % chance, add station to random region
        if (random_regions_added_ > 0 && random(0,2) == 0)
        {
            auto rid = n_to_regionid(random<decltype(random_regions_added_)>(0, random_regions_added_));
            ds_.add_station_to_region(id, rid);
        }

        ++random_stations_added_;
    }
}

MainProgram::CmdResult MainProgram::cmd_random_stations(ostream& output, MatchIter begin, MatchIter end)
{
    string sizestr = *begin++;
    string minxstr = *begin++;
    string minystr = *begin++;
    string maxxstr = *begin++;
    string maxystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    unsigned int size = convert_string_to<unsigned int>(sizestr);

    Coord min{1, 1};
    Coord max{100, 100};
    if (!minxstr.empty() && !minystr.empty() && !maxxstr.empty() && !maxystr.empty())
    {
        min.x = convert_string_to<unsigned int>(minxstr);
        min.y = convert_string_to<unsigned int>(minystr);
        max.x = convert_string_to<unsigned int>(maxxstr);
        max.y = convert_string_to<unsigned int>(maxystr);
    }
    else
    {
        auto stations = ds_.all_stations();
        if (!stations.empty())
        {
            // Find out bounding box
            min = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
            max = {std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};
            for (auto const& station : stations)
            {
                auto [x,y] = ds_.get_station_coordinates(station);
                if (x < min.x) { min.x = x; }
                if (y < min.y) { min.y = y; }
                if (x > max.x) { max.x = x; }
                if (y > max.y) { max.y = y; }
            }
        }
    }

    add_random_stations_regions(size, min, max);

    output << "Added: " << size << " stations." << endl;

    view_dirty = true;

    return {};
}

void MainProgram::test_random_stations()
{
    add_random_stations_regions(1);
}

MainProgram::CmdResult MainProgram::cmd_station_count(ostream& output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    output << "Number of stations: " << ds_.station_count() << endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_next_stations_from(std::ostream& /*output*/, MatchIter begin, MatchIter end)
{
    string id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto opresult = ds_.next_stations_from(id);
    std::sort(opresult.begin(), opresult.end());

    CmdResultTrains result;
    result.reserve(opresult.size());
    std::transform(opresult.begin(), opresult.end(), std::back_inserter(result),
                   [id](auto& id2){ return std::make_tuple(NO_TRAIN, id, id2, NO_TIME) ;});
    return {ResultType::TRAINS, result};
}

MainProgram::CmdResult MainProgram::cmd_train_stations_from(std::ostream& output, MatchIter begin, MatchIter end)
{
 string stationid = *begin++;
 string trainid = *begin++;
 assert( begin == end && "Impossible number of parameters!");

 auto opresult = ds_.train_stations_from(stationid, trainid);

 if (opresult.size() == 1 && opresult.front() == NO_STATION)
 {
     output << "Unsuccessful, operation returned {NO_STATION}." << endl;
     return {};
 }

 CmdResultRoute result;
 result.reserve(opresult.size());
 auto previd = stationid;
 std::transform(opresult.begin(), opresult.end(), std::back_inserter(result),
                [&previd](auto& id){ auto res=std::make_tuple(NO_TRAIN, previd, id, NO_TIME, NO_DISTANCE); previd=id; return res; });
 return {ResultType::ROUTE, result};
}

MainProgram::CmdResult MainProgram::cmd_all_stations(ostream& output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto stations = ds_.all_stations();
    if (stations.empty())
    {
        output << "No stations!" << endl;
    }

    std::sort(stations.begin(), stations.end());
    return {ResultType::IDLIST, CmdResultIDs{{}, stations}};
}

MainProgram::CmdResult MainProgram::cmd_add_region(std::ostream& /*output*/, MatchIter begin, MatchIter end)
{
    RegionID id = convert_string_to<RegionID>(*begin++);
    string name = *begin++;
    string coordsstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    vector<Coord> coords;
    smatch coord;
    auto sbeg = coordsstr.cbegin();
    auto send = coordsstr.cend();
    for ( ; regex_search(sbeg, send, coord, coords_regex_); sbeg = coord.suffix().first)
    {
        coords.push_back({convert_string_to<int>(coord[1]),convert_string_to<int>(coord[2])});
    }

    assert(coords.size() >= 3 && "Region with <3 coords");

    bool success = ds_.add_region(id, name, coords);

    view_dirty = true;
    return {ResultType::IDLIST, CmdResultIDs{{success ? id : NO_REGION}, {}}};
}

MainProgram::CmdResult MainProgram::cmd_all_regions(std::ostream &output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto regions = ds_.all_regions();
    if (regions.empty())
    {
        output << "No regions!" << endl;
    }

    std::sort(regions.begin(), regions.end());
    return {ResultType::IDLIST, CmdResultIDs{regions, {}}};
}

MainProgram::CmdResult MainProgram::cmd_region_info(std::ostream& /*output*/, MatchIter begin, MatchIter end)
{
    RegionID id = convert_string_to<RegionID>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    return {ResultType::IDLIST, CmdResultIDs{{id}, {}}};
}

void MainProgram::test_all_stations()
{
    ds_.all_stations();
}

MainProgram::CmdResult MainProgram::cmd_all_subregions_of_region(std::ostream &output, MatchIter begin, MatchIter end)
{
    RegionID regionid = convert_string_to<RegionID>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    auto regions = ds_.all_subregions_of_region(regionid);
    if (regions.empty())
    {
        output << "No regions!" << endl;
    }

    std::sort(regions.begin(), regions.end());
    regions.insert(regions.begin(), regionid); // Add parameter as the first region
    return {ResultType::IDLIST, CmdResultIDs{regions, {}}};
}

MainProgram::CmdResult MainProgram::cmd_random_trains(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string sizestr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    if (ds_.station_count() < 2)
    {
        output << "Not enough stations to add trains!" << std::endl;
        return {};
    }

    unsigned int random_trains = convert_string_to<unsigned int>(sizestr);

    add_random_trains(random_trains);

    output << "Added random " << random_trains << " trains." << endl;

    view_dirty = true;

    return {};
}

void MainProgram::test_random_trains()
{
    add_random_trains(10);
}

void MainProgram::add_random_trains(unsigned int n)
{
    if (random_stations_added_ > 0) // Don't do anything if there's no stations
    {
        for (unsigned int i=0; i<n; ++i)
        {
            auto trainid = n_to_trainid(random_trains_added_++);
            auto stations = random<unsigned int>(2,10);
            std::vector<std::pair<StationID, Time>> departures;
            departures.reserve(stations);
            auto time = 100*random(0,23) + random(0,59);
            for (unsigned int j=0; j<stations; ++j)
            {
                auto stationid = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
                departures.emplace_back(stationid, time);
                time += random(0,60);
                if (time % 100 > 59) // Fix if minutes exceed 59
                {
                    time += 40;
                }
                if (time > 2359)
                {
                    time -= 2400;
                }
            }
            ds_.add_train(trainid, departures);
        }
    }
}

Distance MainProgram::calc_distance(Coord c1, Coord c2)
{
    if (c1 == NO_COORD || c2 == NO_COORD) { return NO_DISTANCE; }

    long long int deltax = c1.x - c2.x;
    long long int deltay = c1.y - c2.y;
    return static_cast<Distance>(std::sqrt(deltax*deltax + deltay*deltay));
}

MainProgram::CmdResult MainProgram::cmd_clear_all(ostream& output, MatchIter begin, MatchIter end)
{
    assert(begin == end && "Invalid number of parameters");

    ds_.clear_all();
    init_primes();

    output << "Cleared all stations" << endl;

    view_dirty = true;

    return {};
}

string MainProgram::print_station(StationID id, ostream& output, bool nl)
{
    try
    {
        if (id != NO_STATION)
        {
            auto name = ds_.get_station_name(id);
            auto xy = ds_.get_station_coordinates(id);
            if (!name.empty())
            {
                output << name << ": ";
            }
            else
            {
                output << "*: ";
            }

            output << "pos=";
            print_coord(xy, output, false);
            output << ", id=" << id;
            if (nl) { output << endl; }

            ostringstream retstream;
            retstream << id;
            return retstream.str();
        }
        else
        {
            output << "--NO_STATION--";
            if (nl) { output << endl; }
            return "";
        }
    }
    catch (NotImplemented const& e)
    {
        output << endl << "NotImplemented while printing station : " << e.what() << endl;
        std::cerr << endl << "NotImplemented while printing station : " << e.what() << endl;
        return "";
    }
}

string MainProgram::print_station_brief(StationID id, std::ostream &output, bool nl)
{
    try
    {
        if (id != NO_STATION)
        {
            auto name = ds_.get_station_name(id);
            if (!name.empty())
            {
                output << name << " ";
            }
            else
            {
                output << "* ";
            }

            output << "(" << id << ")";
            if (nl) { output << endl; }

            ostringstream retstream;
            retstream << id;
            return retstream.str();
        }
        else
        {
            output << "--NO_STATION--";
            if (nl) { output << endl; }
            return "";
        }
    }
    catch (NotImplemented const& e)
    {
        output << endl << "NotImplemented while printing station : " << e.what() << endl;
        std::cerr << endl << "NotImplemented while printing station : " << e.what() << endl;
        return "";
    }
}

string MainProgram::print_region(RegionID id, std::ostream &output, bool nl)
{
    try
    {
        if (id != NO_REGION)
        {
            auto name = ds_.get_region_name(id);
//            auto xy = ds_.get_region_coords(id);
            if (!name.empty())
            {
                output << name << ": ";
            }
            else
            {
                output << "*: ";
            }

            output << "id=" << id;
            if (nl) { output << endl; }

            ostringstream retstream;
            retstream << id;
            return retstream.str();
        }
        else
        {
            output << "--NO_REGION--";
            if (nl) { output << endl; }
            return "";
        }
    }
    catch (NotImplemented const& e)
    {
        output << endl << "NotImplemented while printing region : " << e.what() << endl;
        std::cerr << endl << "NotImplemented while printing region : " << e.what() << endl;
        return "";
    }
}

MainProgram::CmdResult MainProgram::cmd_find_station_with_coord(ostream& /* output */, MatchIter begin, MatchIter end)
{
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);

    auto result = ds_.find_station_with_coord({x,y});
//    std::sort(result.begin(), result.end());

//    if (result.empty())
//    {
//        output << "No stations found!" << std::endl;
//    }

    return {ResultType::IDLIST, CmdResultIDs{{}, {result}}};
}

void MainProgram::test_find_station_with_coord()
{
    int x = random<int>(1, 10000);
    int y = random<int>(1, 10000);
    ds_.find_station_with_coord({x,y});
}

void MainProgram::test_region_info()
{
    if (random_regions_added_ > 0) // Don't do anything if there's no regions
    {
        auto id = n_to_regionid(random<decltype(random_regions_added_)>(0, random_regions_added_));
        ds_.get_region_name(id);
        ds_.get_region_coords(id);
    }
}

void MainProgram::test_stations_closest_to()
{
    int x = random<int>(1, 10000);
    int y = random<int>(1, 10000);
    ds_.stations_closest_to({x,y});
}

void MainProgram::test_common_parent_of_regions()
{
    if (random_regions_added_ > 0) // Don't do anything if there's no regions
    {
        auto id1 = n_to_regionid(random<decltype(random_regions_added_)>(0, random_regions_added_));
        auto id2 = n_to_regionid(random<decltype(random_regions_added_)>(0, random_regions_added_));
        ds_.common_parent_of_regions(id1, id2);
    }
}

MainProgram::CmdResult MainProgram::cmd_route_any(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromid = *begin++;
    string toid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto steps = ds_.route_any(fromid, toid);
    CmdResultRoute result;

    if (steps.empty())
    {
        output << "No route found!" << endl;
    }
    else if (steps.front() == make_pair(NO_STATION, NO_DISTANCE))
    {
        output << "Starting or destination station not found!" << endl;
    }
    else
    {
        auto [id, dist] = steps.front();
        for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
        {
            auto& [nid, ndist] = *iter;
            result.emplace_back(NO_TRAIN, id, nid, NO_TIME, dist);
            id = nid; dist = ndist;
        }
        result.emplace_back(NO_TRAIN, id, NO_STATION, NO_TIME, dist);
    }
    return {ResultType::ROUTE, result};
}

void MainProgram::test_route_any()
{
    if (random_stations_added_ > 0)
    {
        // Choose two random stations
        auto id1 = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        auto id2 = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        ds_.route_any(id1, id2);
    }
}

MainProgram::CmdResult MainProgram::cmd_route_shortest_distance(ostream& output, MatchIter begin, MatchIter end)
{
    string fromid = *begin++;
    string toid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto steps = ds_.route_shortest_distance(fromid, toid);
    CmdResultRoute result;

    if (steps.empty())
    {
        output << "No route found!" << endl;
    }
    else if (steps.front() == make_pair(NO_STATION, NO_DISTANCE))
    {
        output << "Starting or destination station not found!" << endl;
    }
    else
    {
        auto [id, dist] = steps.front();
        for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
        {
            auto& [nid, ndist] = *iter;
            result.emplace_back(NO_TRAIN, id, nid, NO_TIME, dist);
            id = nid; dist = ndist;
        }
        result.emplace_back(NO_TRAIN, id, NO_STATION, NO_TIME, dist);
    }

    return {ResultType::ROUTE, result};
}

void MainProgram::test_route_shortest_distance()
{
    if (random_stations_added_ > 0)
    {
        // Choose two random stations
        auto id1 = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        auto id2 = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        ds_.route_shortest_distance(id1, id2);
    }
}

MainProgram::CmdResult MainProgram::cmd_route_least_stations(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromid = *begin++;
    string toid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto steps = ds_.route_least_stations(fromid, toid);
    CmdResultRoute result;

    if (steps.empty())
    {
        output << "No route found!" << endl;
    }
    else if (steps.front() == make_pair(NO_STATION, NO_DISTANCE))
    {
        output << "Starting or destination station not found!" << endl;
    }
    else
    {
        auto [id, dist] = steps.front();
        for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
        {
            auto& [nid, ndist] = *iter;
            result.emplace_back(NO_TRAIN, id, nid, NO_TIME, dist);
            id = nid; dist = ndist;
        }
        result.emplace_back(NO_TRAIN, id, NO_STATION, NO_TIME, dist);
    }

    return {ResultType::ROUTE, result};
}

void MainProgram::test_route_least_stations()
{
    if (random_stations_added_ > 0)
    {
        // Choose two random stations
        auto id1 = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        auto id2 = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        ds_.route_least_stations(id1, id2);
    }
}

MainProgram::CmdResult MainProgram::cmd_route_with_cycle(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto steps = ds_.route_with_cycle(fromid);
    if (steps.empty())
    {
        output << "No route found." << std::endl;
        return {};
    }

    if (steps.front() == NO_STATION)
    {
        output << "Starting station not found!" << endl;
        return {};
    }

    if (steps.size() < 2)
    {
        output << "Too short route (" << steps.size() << ") to contain cycles!" << endl;
        return {};
    }

    auto laststation = steps.back();
    auto cycbeg = std::find_if(steps.begin(), steps.end()-1, [laststation](auto const& e){ return e == laststation; });
    if (cycbeg == steps.end())
    {
        output << "No cycle found in returned route!";
        return {};
    }

//    // If necessary, swap cycle so that it starts with smaller coord
//    if ((cycbeg+1) < (steps.end()-2))
//    {
//        auto stationfirst = *cycbeg;
//        auto stationlast = *(steps.end()-2);
//        if (stationlast < stationfirst)
//        {
//           std::reverse(cycbeg+1, steps.end()-1);
//        }
//    }

    CmdResultRoute result;
    auto id = steps.front();
    for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
    {
        auto& nid = *iter;
        result.emplace_back(NO_TRAIN, id, nid, NO_TIME, NO_DISTANCE);
        id = nid;
    }
    result.emplace_back(NO_TRAIN, id, NO_STATION, NO_TIME, NO_DISTANCE);

    return {ResultType::ROUTE, result};
}

void MainProgram::test_route_with_cycle()
{
    if (random_stations_added_ > 0)
    {
        // Choose random station
        auto id = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        ds_.route_with_cycle(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_route_earliest_arrival(std::ostream &output, MatchIter begin, MatchIter end)
{
    string fromid = *begin++;
    string toid = *begin++;
    string starttimestr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    Time starttime = convert_string_to<Time>(starttimestr);
    auto steps = ds_.route_earliest_arrival(fromid, toid, starttime);
    CmdResultRoute result;

    if (steps.empty())
    {
        output << "No route found!" << endl;
    }
    else if (steps.front() == make_pair(NO_STATION, NO_TIME))
    {
        output << "Starting or destination station not found!" << endl;
    }
    else
    {
        auto [id, time] = steps.front();
        for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
        {
            auto& [nid, ntime] = *iter;
            result.emplace_back(NO_TRAIN, id, nid, time, NO_DISTANCE);
            id = nid; time = ntime;
        }
        result.emplace_back(NO_TRAIN, id, NO_STATION, time, NO_DISTANCE);
    }

    return {ResultType::ROUTE, result};
}

void MainProgram::test_route_earliest_arrival()
{
    if (random_stations_added_ > 0)
    {
        // Choose two random stations
        auto id1 = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        auto id2 = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
        auto hours = random(0, 24);
        auto minutes = random(0, 60);
        ds_.route_earliest_arrival(id1, id2, 100*hours+minutes);
    }
}

MainProgram::CmdResult MainProgram::cmd_clear_trains(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    ds_.clear_trains();
    output << "All trains removed." << std::endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_randseed(std::ostream& output, MatchIter begin, MatchIter end)
{
    string seedstr = *begin++;
    assert(begin == end && "Invalid number of parameters");

    unsigned long int seed = convert_string_to<unsigned long int>(seedstr);

    rand_engine_.seed(seed);
    init_primes();

    output << "Random seed set to " << seed << endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_read(std::ostream& output, MatchIter begin, MatchIter end)
{
    string filename = *begin++;
    string silentstr =  *begin++;
    assert( begin == end && "Impossible number of parameters!");

    bool silent = !silentstr.empty();
    ostream* new_output = &output;

    ostringstream dummystr; // Given as output if "silent" is specified, the output is discarded
    if (silent)
    {
        new_output = &dummystr;
    }

    ifstream input(filename);
    if (input)
    {
        output << "** Commands from '" << filename << "'" << endl;
        command_parser(input, *new_output, PromptStyle::NORMAL);
        if (silent) { output << "...(output discarded in silent mode)..." << endl; }
        output << "** End of commands from '" << filename << "'" << endl;
    }
    else
    {
        output << "Cannot open file '" << filename << "'!" << endl;
    }

    return {};
}


MainProgram::CmdResult MainProgram::cmd_testread(std::ostream& output, MatchIter begin, MatchIter end)
{
    string infilename = *begin++;
    string outfilename = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    ifstream input(infilename);
    if (input)
    {
        ifstream expected_output(outfilename);
        if (output)
        {
            stringstream actual_output;
            command_parser(input, actual_output, PromptStyle::NO_NESTING);

            vector<string> actual_lines;
            while (actual_output)
            {
                string line;
                getline(actual_output, line);
                if (!actual_output) { break; }
                actual_lines.push_back(line);
            }

            vector<string> expected_lines;
            while (expected_output)
            {
                string line;
                getline(expected_output, line);
                if (!expected_output) { break; }
                expected_lines.push_back(line);
            }

            string heading_actual = "Actual output";
            unsigned int actual_max_length = heading_actual.length();
            auto actual_max_iter = max_element(actual_lines.cbegin(), actual_lines.cend(),
                                               [](string s1, string s2){ return s1.length() < s2.length(); });
            if (actual_max_iter != actual_lines.cend())
            {
                actual_max_length = actual_max_iter->length();
            }

            string heading_expected = "Expected output";
            unsigned int expected_max_length = heading_expected.length();
            auto expected_max_iter = std::max_element(expected_lines.cbegin(), expected_lines.cend(),
                                                    [](string s1, string s2){ return s1.length() < s2.length(); });
            if (expected_max_iter != expected_lines.cend())
            {
                expected_max_length = expected_max_iter->length();
            }

            auto pos_actual = actual_lines.cbegin();
            auto pos_expected = expected_lines.cbegin();
            output << "  " << heading_actual << string(actual_max_length - heading_actual.length(), ' ') << " | " << heading_expected << endl;
            output << "--" << string(actual_max_length, '-') << "-|-" << string(expected_max_length, '-') << endl;

            bool lines_ok = true;
            while (pos_expected != expected_lines.cend() || pos_actual != actual_lines.cend())
            {
                if (pos_expected != expected_lines.cend())
                {
                    if (pos_actual != actual_lines.cend())
                    {
                        bool ok = (*pos_expected == *pos_actual);
                        output << (ok ? ' ' : '?') << ' ' << *pos_actual << string(actual_max_length - pos_actual->length(), ' ')
                               << " | " << *pos_expected << endl;
                        lines_ok = lines_ok && ok;
                        ++pos_actual;
                    }
                    else
                    { // Actual output was too short
                        output << "? " << string(actual_max_length, ' ')
                               << " | " << *pos_expected << endl;
                        lines_ok = false;
                    }
                    ++pos_expected;
                }
                else
                { // Actual output was too long
                    output << "? " << *pos_actual << string(actual_max_length - pos_actual->length(), ' ')
                           << " | " << endl;
                    lines_ok = false;
                    ++pos_actual;
                }
            }
            if (lines_ok)
            {
                output << "**No differences in output.**" << endl;
                if (test_status_ == TestStatus::NOT_RUN)
                {
                    test_status_ = TestStatus::NO_DIFFS;
                }
            }
            else
            {
                output << "**Differences found! (Lines beginning with '?')**" << endl;
                test_status_ = TestStatus::DIFFS_FOUND;
            }

        }
        else
        {
            output << "Cannot open file '" << outfilename << "'!" << endl;
        }
    }
    else
    {
        output << "Cannot open file '" << infilename << "'!" << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_stopwatch(std::ostream& output, MatchIter begin, MatchIter end)
{
    string on = *begin++;
    string off = *begin++;
    string next = *begin++;
    assert(begin == end && "Invalid number of parameters");

    if (!on.empty())
    {
        stopwatch_mode = StopwatchMode::ON;
        output << "Stopwatch: on" << endl;
    }
    else if (!off.empty())
    {
        stopwatch_mode = StopwatchMode::OFF;
        output << "Stopwatch: off" << endl;
    }
    else if (!next.empty())
    {
        stopwatch_mode = StopwatchMode::NEXT;
        output << "Stopwatch: on for the next command" << endl;
    }
    else
    {
        assert(!"Impossible stopwatch mode!");
    }

    return {};
}

std::string MainProgram::print_station_name(StationID id, std::ostream &output, bool nl)
{
    try
    {
        if (id != NO_STATION)
        {
            auto name = ds_.get_station_name(id);
            if (!name.empty())
            {
                output << name;
            }
            else
            {
                output << "*";
            }

            ostringstream retstream;
            retstream << name;
            if (nl) { output << endl; }
            return retstream.str();
        }
        else
        {
            output << "--NO_STATION--";
            if (nl) { output << endl; }
            return "";
        }
    }
    catch (NotImplemented const& e)
    {
        output << endl << "NotImplemented while printing station name : " << e.what() << endl;
        std::cerr << endl << "NotImplemented while printing station name : " << e.what() << endl;
        return "";
    }
}

std::string MainProgram::print_coord(Coord coord, std::ostream& output, bool nl)
{
    if (coord != NO_COORD)
    {
        output << "(" << coord.x << "," << coord.y << ")";
        ostringstream retstream;
        retstream << "(" << coord.x << "," << coord.y << ")";
        if (nl) { output << endl; }
        return retstream.str();
    }
    else
    {
        output << "(--NO_COORD--)";
        if (nl) { output << endl; }
        return "";
    }
}

string MainProgram::print_train(TrainID id, std::ostream &output, bool nl)
{
    try
    {
        if (id != NO_TRAIN)
        {
            output << id;
            if (nl) { output << endl; }

            ostringstream retstream;
            retstream << id;
            return retstream.str();
        }
        else
        {
            output << "--NO_TRAIN--";
            if (nl) { output << endl; }
            return "";
        }
    }
    catch (NotImplemented const& e)
    {
        output << endl << "NotImplemented while printing train : " << e.what() << endl;
        std::cerr << endl << "NotImplemented while printing train : " << e.what() << endl;
        return "";
    }
}

string MainProgram::print_time(Time time, std::ostream &output, bool nl)
{
    if (time != NO_TIME)
    {
        output << setw(4) << setfill('0') << time;
        ostringstream retstream;
        retstream << setw(4) << setfill('0') << time;
        if (nl) { output << endl; }
        return retstream.str();
    }
    else
    {
        output << "(--NO_TIME--)";
        if (nl) { output << endl; }
        return "";
    }
}

string const stationidx = "([a-zA-Z0-9-]+)";
string const trainidx = "([a-zA-Z0-9-]+)";
string const regionidx = "([0-9]+)";
string const namex = "([ a-zA-Z0-9-]+)";
string const timex = "((?:[01][0-9][0-5][0-9])|(?:2[0-3][0-5][0-9]))";
string const numx = "([0-9]+)";
string const optcoordx = "\\([[:space:]]*[0-9]+[[:space:]]*,[[:space:]]*[0-9]+[[:space:]]*\\)";
string const coordx = "\\([[:space:]]*([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)[[:space:]]*\\)";
string const wsx = "[[:space:]]+";
string const optstationtimeidx = "[a-zA-Z0-9-]+:(?:(?:[01][0-9][0-5][0-9])|(?:2[0-3][0-5][0-9]))";

vector<MainProgram::CmdInfo> MainProgram::cmds_ =
{
    {"station_count", "", "", &MainProgram::cmd_station_count, nullptr },
    {"clear_all", "", "", &MainProgram::cmd_clear_all, nullptr },
    {"all_stations", "", "", &MainProgram::cmd_all_stations, &MainProgram::test_all_stations },
    {"add_station", "StationID \"Name\" (x,y)", stationidx+wsx+'"'+namex+'"'+wsx+coordx, &MainProgram::cmd_add_station, nullptr },
    {"station_info", "StationID", stationidx, &MainProgram::cmd_station_info, &MainProgram::test_station_info },
    {"stations_alphabetically", "", "", &MainProgram::NoParListCmd<&Datastructures::stations_alphabetically>, &MainProgram::NoParListTestCmd<&Datastructures::stations_alphabetically> },
    {"stations_distance_increasing", "", "", &MainProgram::NoParListCmd<&Datastructures::stations_distance_increasing>,
                                             &MainProgram::NoParListTestCmd<&Datastructures::stations_distance_increasing> },
    {"find_station_with_coord", "(x,y)", coordx, &MainProgram::cmd_find_station_with_coord, &MainProgram::test_find_station_with_coord },
    {"change_station_coord", "StationID (x,y)", stationidx+wsx+coordx, &MainProgram::cmd_change_station_coord, &MainProgram::test_change_station_coord },
    {"add_departure", "StationID TrainID Time", stationidx+wsx+trainidx+wsx+timex, &MainProgram::cmd_add_departure, &MainProgram::test_add_departure },
    {"remove_departure", "StationID TrainID Time", stationidx+wsx+trainidx+wsx+timex, &MainProgram::cmd_remove_departure, &MainProgram::test_remove_departure },
    {"station_departures_after", "StationID Time", stationidx+wsx+timex, &MainProgram::cmd_station_departures_after, &MainProgram::test_station_departures_after },
//    {"mindist", "", "", &MainProgram::NoParstationCmd<&Datastructures::min_distance>, &MainProgram::NoParstationTestCmd<&Datastructures::min_distance> },
//    {"maxdist", "", "", &MainProgram::NoParstationCmd<&Datastructures::max_distance>, &MainProgram::NoParstationTestCmd<&Datastructures::max_distance> },
    {"add_region", "RegionID \"Name\" (x,y) (x,y)...", regionidx+wsx+'"'+namex+'"'+"((?:"+wsx+optcoordx+")+)", &MainProgram::cmd_add_region, nullptr },
    {"all_regions", "", "", &MainProgram::cmd_all_regions, nullptr },
    {"region_info", "RegionID", regionidx, &MainProgram::cmd_region_info, &MainProgram::test_region_info },
    {"add_subregion_to_region", "SubregionID RegionID", regionidx+wsx+regionidx, &MainProgram::cmd_add_subregion_to_region, nullptr },
    {"add_station_to_region", "StationID RegionID", stationidx+wsx+regionidx, &MainProgram::cmd_add_station_to_region, nullptr },
    {"station_in_regions", "StationID", stationidx, &MainProgram::cmd_station_in_regions, &MainProgram::test_station_in_regions },
    {"all_subregions_of_region", "RegionID", regionidx, &MainProgram::cmd_all_subregions_of_region, &MainProgram::test_all_subregions_of_region },
    {"stations_closest_to", "(x,y)", coordx, &MainProgram::cmd_stations_closest_to, &MainProgram::test_stations_closest_to },
    {"remove_station", "StationID", stationidx, &MainProgram::cmd_remove_station, &MainProgram::test_remove_station },
    {"common_parent_of_regions", "RegionID1 RegionID2", regionidx+wsx+regionidx, &MainProgram::cmd_common_parent_of_regions, &MainProgram::test_common_parent_of_regions },
    {"add_train", "TrainID StationID1:Time1 ... StationIDlast:Timelast", trainidx+"((?:"+wsx+optstationtimeidx+")+)", &MainProgram::cmd_add_train, nullptr },
    {"next_stations_from", "StationID", stationidx, &MainProgram::cmd_next_stations_from, &MainProgram::test_next_stations_from },
    {"train_stations_from", "StationID TrainID", stationidx+wsx+trainidx, &MainProgram::cmd_train_stations_from, &MainProgram::test_train_stations_from },
    {"clear_trains", "", "", &MainProgram::cmd_clear_trains, nullptr },
    {"route_any", "StationID StationID", stationidx+wsx+stationidx, &MainProgram::cmd_route_any, &MainProgram::test_route_any },
    {"route_least_stations", "StationID StationID", stationidx+wsx+stationidx, &MainProgram::cmd_route_least_stations, &MainProgram::test_route_least_stations },
    {"route_with_cycle", "StationID", stationidx, &MainProgram::cmd_route_with_cycle, &MainProgram::test_route_with_cycle },
    {"route_shortest_distance", "StationID StationID", stationidx+wsx+stationidx, &MainProgram::cmd_route_shortest_distance, &MainProgram::test_route_shortest_distance },
    {"route_earliest_arrival", "StationID StationID StartTime", stationidx+wsx+stationidx+wsx+timex, &MainProgram::cmd_route_earliest_arrival, &MainProgram::test_route_earliest_arrival },
    {"quit", "", "", nullptr, nullptr },
    {"help", "", "", &MainProgram::help_command, nullptr },
    {"random_stations", "number_of_stations_to_add  (minx,miny) (maxx,maxy) (coordinates optional)",
     numx+"(?:"+wsx+coordx+wsx+coordx+")?", &MainProgram::cmd_random_stations, &MainProgram::test_random_stations },
    {"random_trains", "max_number_of_trains_to_add", numx,
     &MainProgram::cmd_random_trains, &MainProgram::test_random_trains },
    {"read", "\"in-filename\" [silent]", "\"([-a-zA-Z0-9 ./:_]+)\"(?:"+wsx+"(silent))?", &MainProgram::cmd_read, nullptr },
    {"testread", "\"in-filename\" \"out-filename\"", "\"([-a-zA-Z0-9 ./:_]+)\""+wsx+"\"([-a-zA-Z0-9 ./:_]+)\"", &MainProgram::cmd_testread, nullptr },
    {"perftest", "cmd1|all|compulsory[;cmd2...] timeout repeat_count n1[;n2...] (parts in [] are optional, alternatives separated by |)",
     "([0-9a-zA-Z_]+(?:;[0-9a-zA-Z_]+)*)"+wsx+numx+wsx+numx+wsx+"([0-9]+(?:;[0-9]+)*)", &MainProgram::cmd_perftest, nullptr },
    {"stopwatch", "on|off|next (alternatives separated by |)", "(?:(on)|(off)|(next))", &MainProgram::cmd_stopwatch, nullptr },
    {"random_seed", "new-random-seed-integer", numx, &MainProgram::cmd_randseed, nullptr },
    {"#", "comment text", ".*", &MainProgram::cmd_comment, nullptr },
};

MainProgram::CmdResult MainProgram::help_command(std::ostream& output, MatchIter /*begin*/, MatchIter /*end*/)
{
    output << "Commands:" << endl;
    for (auto& i : cmds_)
    {
        output << "  " << i.cmd << " " << i.info << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_perftest(std::ostream& output, MatchIter begin, MatchIter end)
{
#ifdef _GLIBCXX_DEBUG
    output << "WARNING: Debug STL enabled, performance will be worse than expected (maybe also asymptotically)!" << endl;
#endif // _GLIBCXX_DEBUG

    try {
    // Note: everything below is indented too little by one indentation level! (because of try block above)

    vector<string> optional_cmds({"route_least_stations", "route_with_cycle", "route_shortest_distance", "route_earliest_arrival"});
    vector<string> nondefault_cmds({"station_count","all_stations","station_info","stations_alphabetically","stations_distance_increasing","find_station_with_coord",
                                    "change_station_coord","add_departure","remove_departure","region_info","station_in_regions","all_subregions_of_region",
                                    "stations_closest_to","remove_station","common_parent_of_regions"});

    string commandstr = *begin++;
    unsigned int timeout = convert_string_to<unsigned int>(*begin++);
    unsigned int repeat_count = convert_string_to<unsigned int>(*begin++);
    string sizes = *begin++;
    assert(begin == end && "Invalid number of parameters");

    vector<string> testcmds;
    bool additional_get_cmds = true;
    if (commandstr != "all" && commandstr != "compulsory")
    {
        additional_get_cmds = false;
        smatch scmd;
        auto cbeg = commandstr.cbegin();
        auto cend = commandstr.cend();
        for ( ; regex_search(cbeg, cend, scmd, commands_regex_); cbeg = scmd.suffix().first)
        {
            testcmds.push_back(scmd[1]);
        }
    }

    vector<unsigned int> init_ns;
    smatch size;
    auto sbeg = sizes.cbegin();
    auto send = sizes.cend();
    for ( ; regex_search(sbeg, send, size, sizes_regex_); sbeg = size.suffix().first)
    {
        init_ns.push_back(convert_string_to<unsigned int>(size[1]));
    }

    output << "Timeout for each N is " << timeout << " sec. " << endl;
    output << "For each N perform " << repeat_count << " random command(s) from:" << endl;

    // Initialize test functions
    vector<void(MainProgram::*)()> testfuncs;
    if (testcmds.empty())
    { // Add all commands
        for (auto& i : cmds_)
        {
            if (i.testfunc)
            {
                if (find(nondefault_cmds.begin(), nondefault_cmds.end(), i.cmd) == nondefault_cmds.end() &&
                    (commandstr == "all" || find(optional_cmds.begin(), optional_cmds.end(), i.cmd) == optional_cmds.end()))
                {
                    output << i.cmd << " ";
                    testfuncs.push_back(i.testfunc);
                }
            }
        }
    }
    else
    {
        for (auto& i : testcmds)
        {
            auto pos = find_if(cmds_.begin(), cmds_.end(), [&i](auto const& cmd){ return cmd.cmd == i; });
            if (pos != cmds_.end() && pos->testfunc)
            {
                output << i << " ";
                testfuncs.push_back(pos->testfunc);
            }
            else
            {
                output << "(cannot test " << i << ") ";
            }
        }
    }
    output << endl << endl;

    if (testfuncs.empty())
    {
        output << "No commands to test!" << endl;
        return {};
    }

#ifdef USE_PERF_EVENT
    output << setw(7) << "N" << " , " << setw(12) << "add (sec)" << " , " << setw(12) << "add (count)" << " , " << setw(12) << "cmds (sec)" << " , "
           << setw(12) << "cmds (count)"  << " , " << setw(12) << "total (sec)" << " , " << setw(12) << "total (count)" << endl;
#else
    output << setw(7) << "N" << " , " << setw(12) << "add (sec)" << " , " << setw(12) << "cmds (sec)" << " , "
           << setw(12) << "total (sec)" << endl;
#endif
    flush_output(output);

    auto stop = false;
    for (unsigned int n : init_ns)
    {
        if (stop) { break; }

        output << setw(7) << n << " , " << flush;

        ds_.clear_all();
        ds_.clear_trains();
        init_primes();

        Stopwatch stopwatch(true); // Use also instruction counting, if enabled

        // Add random stations
        for (unsigned int i = 0; i < n / 1000; ++i)
        {
            stopwatch.start();
            add_random_stations_regions(1000);
            stopwatch.stop();

            if (stopwatch.elapsed() >= timeout)
            {
                output << "Timeout!" << endl;
                stop = true;
                break;
            }
            if (check_stop())
            {
                output << "Stopped!" << endl;
                stop = true;
                break;
            }
        }
        if (stop) { break; }

        if (n % 1000 != 0)
        {
            stopwatch.start();
            add_random_stations_regions(n % 1000);
            stopwatch.stop();
        }

        // Add random trains
        for (unsigned int i = 0; i < n / 1000; ++i)
        {
            stopwatch.start();
            add_random_trains(1000);
            stopwatch.stop();

            if (stopwatch.elapsed() >= timeout)
            {
                output << "Timeout!" << endl;
                stop = true;
                break;
            }
            if (check_stop())
            {
                output << "Stopped!" << endl;
                stop = true;
                break;
            }
        }
        if (stop) { break; }

        if (n % 1000 != 0)
        {
            stopwatch.start();
            add_random_trains(n % 1000);
            stopwatch.stop();
        }

#ifdef USE_PERF_EVENT
        auto addcount = stopwatch.count();
#endif
        auto addsec = stopwatch.elapsed();

#ifdef USE_PERF_EVENT
        output << setw(12) << addsec << " , " << setw(12) << addcount << " , " << flush;
#else
        output << setw(12) << addsec << " , " << flush;
#endif

        if (addsec >= timeout)
        {
            output << "Timeout!" << endl;
            stop = true;
            break;
        }

        stopwatch.start();
        for (unsigned int repeat = 0; repeat < repeat_count; ++repeat)
        {
            auto cmdpos = random(testfuncs.begin(), testfuncs.end());

            (this->**cmdpos)();
            if (additional_get_cmds)
            {
                if (random_stations_added_ > 0) // Don't do anything if there's no stations
                {
                    StationID id = n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
                    ds_.get_station_name(id);
                    ds_.get_station_coordinates(id);
                }
            }

            if (repeat % 10 == 0)
            {
                stopwatch.stop();
                if (stopwatch.elapsed() >= timeout)
                {
                    output << "Timeout!" << endl;
                    stop = true;
                    break;
                }
                if (check_stop())
                {
                    output << "Stopped!" << endl;
                    stop = true;
                    break;
                }
                stopwatch.start();
            }
        }
        stopwatch.stop();
        if (stop) { break; }

#ifdef USE_PERF_EVENT
        auto totalcount = stopwatch.count();
#endif
        auto totalsec = stopwatch.elapsed();

#ifdef USE_PERF_EVENT
        output << setw(12) << totalsec-addsec << " , " << setw(12) << totalcount-addcount << " , " << setw(12) << totalsec << " , " << setw(12) << totalcount;
#else
        output << setw(12) << totalsec-addsec << " , " << setw(12) << totalsec;
#endif

//        unsigned long int maxmem;
//        string unit;
//        tie(maxmem, unit) = mempeak();
//        maxmem -=  startmem;
//        if (maxmem != 0)
//        {
//            output << ", memory " << maxmem << " " << unit;
//        }
        output << endl;
        flush_output(output);
    }

    ds_.clear_all();
    ds_.clear_trains();
    init_primes();

    }
    catch (NotImplemented const&)
    {
        // Clean up after NotImplemented
        ds_.clear_all();
        init_primes();
        throw;
    }

#ifdef _GLIBCXX_DEBUG
    output << "WARNING: Debug STL enabled, performance will be worse than expected (maybe also asymptotically)!" << endl;
#endif // _GLIBCXX_DEBUG

    return {};
}

MainProgram::CmdResult MainProgram::cmd_comment(std::ostream& /*output*/, MatchIter /*begin*/, MatchIter /*end*/)
{
    return {};
}

bool MainProgram::command_parse_line(string inputline, ostream& output)
{
//    static unsigned int nesting_level = 0; // UGLY! Remember nesting level to print correct amount of >:s.
//    if (promptstyle != PromptStyle::NO_NESTING) { ++nesting_level; }

    if (inputline.empty()) { return true; }

    smatch match;
    bool matched = regex_match(inputline, match, cmds_regex_);
    if (matched)
    {
        assert(match.size() == 3);
        string cmd = match[1];
        string params = match[2];

        auto pos = find_if(cmds_.begin(), cmds_.end(), [cmd](CmdInfo const& ci) { return ci.cmd == cmd; });
        assert(pos != cmds_.end());

        smatch match2;
        bool matched2 = regex_match(params, match2, pos->param_regex);
        if (matched2)
        {
            if (pos->func)
            {
                assert(!match2.empty());

                Stopwatch stopwatch;
                bool use_stopwatch = (stopwatch_mode != StopwatchMode::OFF);
                // Reset stopwatch mode if only for the next command
                if (stopwatch_mode == StopwatchMode::NEXT) { stopwatch_mode = StopwatchMode::OFF; }

               TestStatus initial_status = test_status_;
               test_status_ = TestStatus::NOT_RUN;

                if (use_stopwatch)
                {
                    stopwatch.start();
                }

                CmdResult result;
                try
                {
                    result = (this->*(pos->func))(output, ++(match2.begin()), match2.end());
                }
                catch (NotImplemented const& e)
                {
                    output << endl << "NotImplemented from cmd " << pos->cmd << " : " << e.what() << endl;
                    std::cerr << endl << "NotImplemented from cmd " << pos->cmd << " : " << e.what() << endl;
                }

                if (use_stopwatch)
                {
                    stopwatch.stop();
                }

                switch (result.first)
                {
                    case ResultType::NOTHING:
                    {
                        break;
                    }
                    case ResultType::IDLIST:
                    {
                        auto& [regions, stations] = std::get<CmdResultIDs>(result.second);
                        if (stations.size() == 1 && stations.front() == NO_STATION)
                        {
                            output << "Failed (NO_STATION returned)!" << std::endl;
                        }
                        else
                        {
                            if (!stations.empty())
                            {
                                if (stations.size() == 1) { output << "Station:" << std::endl; }
                                else { output << "Stations:" << std::endl; }

                                unsigned int num = 0;
                                for (StationID id : stations)
                                {
                                    ++num;
                                    if (stations.size() > 1) { output << num << ". "; }
                                    else { output << "   "; }
                                    print_station(id, output);
                                }
                            }
                        }

                        if (regions.size() == 1 && regions.front() == NO_REGION)
                        {
                            output << "Failed (NO_REGION returned)!" << std::endl;
                        }
                        else
                        {
                            if (!regions.empty())
                            {
                                if (regions.size() == 1) { output << "Region:" << std::endl; }
                                else { output << "Regions:" << std::endl; }

                                unsigned int num = 0;
                                for (RegionID id : regions)
                                {
                                    ++num;
                                    if (regions.size() > 1) { output << num << ". "; }
                                    else { output << "   "; }
                                    print_region(id, output);
                                }
                            }
                        }
                        break;
                    }
                    case ResultType::ROUTE:
                    {
                        auto& route = std::get<CmdResultRoute>(result.second);
                        if (!route.empty())
                        {
                            if (route.size() == 1 && get<1>(route.front()) == NO_STATION)
                            {
                                output << "Failed (...NO_STATION... returned)!" << std::endl;
                            }
                            else
                            {
                                unsigned int num = 1;
                                for (auto& r : route)
                                {
                                    auto [trainid, stationid1, stationid2, time, dist] = r;
                                    output << num << ". ";
                                    if (stationid1 != NO_STATION)
                                    {
                                        print_station_brief(stationid1, output, false);
                                    }
                                    if (stationid2 != NO_STATION)
                                    {
                                        output << " -> ";
                                        print_station_brief(stationid2, output, false);
                                    }
                                    if (trainid != NO_TRAIN)
                                    {
                                        output << ": ";
                                        print_train(trainid, output, false);
                                    }
                                    if (time != NO_TIME)
                                    {
                                        output << " (at ";
                                        print_time(time, output, false);
                                        output << ")";
                                    }
                                    if (dist != NO_DISTANCE)
                                    {
                                        output << " (distance " << dist << ")";
                                    }
                                    output << endl;

                                    ++num;
                                }
                            }
                        }
                        break;
                    }
                    case ResultType::TRAINS:
                    {
                    auto& route = std::get<CmdResultTrains>(result.second);
                    if (!route.empty())
                    {
                        if (route.size() == 1 && get<1>(route.front()) == NO_STATION)
                        {
                            output << "Failed (...NO_STATION... returned)!" << std::endl;
                        }
                        else
                        {
                            unsigned int num = 1;
                            for (auto& r : route)
                            {
                                auto [trainid, stationid1, stationid2, time] = r;
                                output << num << ". ";
                                if (stationid1 != NO_STATION)
                                {
                                    print_station_brief(stationid1, output, false);
                                }
                                if (stationid2 != NO_STATION)
                                {
                                    output << " -> ";
                                    print_station_brief(stationid2, output, false);
                                }
                                if (trainid != NO_TRAIN)
                                {
                                    output << ": ";
                                    print_train(trainid, output, false);
                                }
                                if (time != NO_TIME)
                                {
                                    output << " (at ";
                                    print_time(time, output, false);
                                    output << ")";
                                }
                                output << endl;

                                ++num;
                            }
                        }
                    }
                    break;
                    }
                    default:
                    {
                        assert(false && "Unsupported result type!");
                    }
                }

                if (result != prev_result)
                {
                    prev_result = move(result);
                    view_dirty = true;
                }

                if (use_stopwatch)
                {
                    output << "Command '" << cmd << "': " << stopwatch.elapsed() << " sec" << endl;
                }

                if (test_status_ != TestStatus::NOT_RUN)
                {
                    output << "Testread-tests have been run, " << ((test_status_ == TestStatus::DIFFS_FOUND) ? "differences found!" : "no differences found.") << endl;
                }
                if (test_status_ == TestStatus::NOT_RUN || (test_status_ == TestStatus::NO_DIFFS && initial_status == TestStatus::DIFFS_FOUND))
                {
                    test_status_ = initial_status;
                }
            }
            else
            { // No function to run = quit command
                return false;
            }
        }
        else
        {
            output << "Invalid parameters for command '" << cmd << "'!" << endl;
        }
    }
    else
    {
        output << "Unknown command!" << endl;
    }

    return true; // Signal continuing
}

void MainProgram::command_parser(istream& input, ostream& output, PromptStyle promptstyle)
{
    string line;
    do
    {
//        output << string(nesting_level, '>') << " ";
        output << PROMPT;
        getline(input, line, '\n');

        if (promptstyle != PromptStyle::NO_ECHO)
        {
            output << line << endl;
        }

        if (!input) { break; }

        bool cont = command_parse_line(line, output);
        view_dirty = false; // No need to keep track of individual result changes
        if (!cont) { break; }
    }
    while (input);
    //    if (promptstyle != PromptStyle::NO_NESTING) { --nesting_level; }

    view_dirty = true; // To be safe, assume that results have been changed
}

void MainProgram::setui(MainWindow* ui)
{
    ui_ = ui;
}

#ifdef GRAPHICAL_GUI
void MainProgram::flush_output(std::ostream& output)
{
    if (ui_)
    {
        if (auto soutput = dynamic_cast<ostringstream*>(&output))
        {
            ui_->output_text(*soutput);
        }
    }
}
#else
void MainProgram::flush_output(std::ostream& /*output*/)
{
}
#endif

bool MainProgram::check_stop() const
{
#ifdef GRAPHICAL_GUI
    if (ui_)
    {
        return ui_->check_stop_pressed();
    }
#endif
    return false;
}

std::array<unsigned long int, 20> const MainProgram::primes1{4943,   4951,   4957,   4967,   4969,   4973,   4987,   4993,   4999,   5003,
                                                             5009,   5011,   5021,   5023,   5039,   5051,   5059,   5077,   5081,   5087};
std::array<unsigned long int, 20> const MainProgram::primes2{81031,  81041,  81043,  81047,  81049,  81071,  81077,  81083,  81097,  81101,
                                                             81119,  81131,  81157,  81163,  81173,  81181,  81197,  81199,  81203,  81223};

MainProgram::MainProgram()
{
    rand_engine_.seed(time(nullptr));

    //    startmem = get<0>(mempeak());

    init_primes();
    init_regexs();
}

int MainProgram::mainprogram(int argc, char* argv[])
{
    vector<string> args(argv, argv+argc);

    if (args.size() < 1 || args.size() > 2)
    {
        cerr << "Usage: " + ((args.size() > 0) ? args[0] : "<program name>") + " [<command file>]" << endl;
        return EXIT_FAILURE;
    }

    MainProgram mainprg;

    if (args.size() == 2 && args[1] != "--console")
    {
        string filename = args[1];
        ifstream input(filename);
        if (input)
        {
            mainprg.command_parser(input, cout, MainProgram::PromptStyle::NORMAL);
        }
        else
        {
            cout << "Cannot open file '" << filename << "'!" << endl;
        }
    }
    else
    {
        mainprg.command_parser(cin, cout, MainProgram::PromptStyle::NO_ECHO);
    }

    cerr << "Program ended normally." << endl;
    if (mainprg.test_status_ == TestStatus::DIFFS_FOUND)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

void MainProgram::init_primes()
{
    // Initialize id generator
    prime1_ = primes1[random<int>(0, primes1.size())];
    prime2_ = primes2[random<int>(0, primes2.size())];
    random_stations_added_ = 0;
    random_regions_added_ = 0;
    random_trains_added_ = 0;
}

Name MainProgram::n_to_name(unsigned long n)
{
    unsigned long int hash = prime1_*n + prime2_;
    string name;

    while (hash > 0)
    {
        auto hexnum = hash % 26;
        hash /= 26;
        name.push_back('a'+hexnum);
    }

    return name;
}

StationID MainProgram::n_to_stationid(unsigned long n)
{
 std::ostringstream ostr;
 ostr << "S" << n;
 return ostr.str();
}

RegionID MainProgram::n_to_regionid(unsigned long n)
{
    return n;
}

TrainID MainProgram::n_to_trainid(unsigned long n)
{
    std::ostringstream ostr;
    ostr << "T" << n;
    return ostr.str();
}

Coord MainProgram::n_to_coord(unsigned long n)
{
    unsigned long int hash = prime1_ * n + prime2_;
    hash = hash ^ (hash + 0x9e3779b9 + (hash << 6) + (hash >> 2)); // :-P

    return {static_cast<int>(hash % 1000), static_cast<int>((hash/1000) % 1000)};
}

void MainProgram::init_regexs()
{
    // Create regex <whitespace>(cmd1|cmd2|...)<whitespace>(.*)
    string cmds_regex_str = "[[:space:]]*(";
    bool first = true;
    for (auto& cmd : cmds_)
    {
        cmds_regex_str += (first ? "" : "|") + cmd.cmd;
        first = false;

        cmd.param_regex = regex(cmd.param_regex_str+"[[:space:]]*", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    }
    cmds_regex_str += ")(?:[[:space:]]*$|"+wsx+"(.*))";
    cmds_regex_ = regex(cmds_regex_str, std::regex_constants::ECMAScript | std::regex_constants::optimize);
    coords_regex_ = regex(coordx+"[[:space:]]?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    stationtimes_regex_ = regex(stationidx+":"+timex+"[[:space:]]?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    times_regex_ = regex(wsx+"([0-9][0-9]):([0-9][0-9]):([0-9][0-9])", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    commands_regex_ = regex("([0-9a-zA-Z_]+);?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    sizes_regex_ = regex(numx+";?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
}
