#ifndef MAINPROGRAM_HH
#define MAINPROGRAM_HH


#ifdef QT_CORE_LIB
#ifndef NO_GRAPHICAL_GUI
#define GRAPHICAL_GUI
#endif
#endif


#include <string>
#include <random>
#include <regex>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <array>
#include <functional>
#include <utility>
#include <variant>
#include <bitset>
#include <cassert>

#include "datastructures.hh"

class MainWindow; // In case there's UI

class MainProgram
{
public:
    MainProgram();


    class Stopwatch;

    enum class PromptStyle { NORMAL, NO_ECHO, NO_NESTING };
    enum class TestStatus { NOT_RUN, NO_DIFFS, DIFFS_FOUND };

    bool command_parse_line(std::string input, std::ostream& output);
    void command_parser(std::istream& input, std::ostream& output, PromptStyle promptstyle);

    void setui(MainWindow* ui);

    void flush_output(std::ostream& output);
    bool check_stop() const;

    static int mainprogram(int argc, char* argv[]);

private:
    Datastructures ds_;
    MainWindow* ui_ = nullptr;

    static std::string const PROMPT;

    std::minstd_rand rand_engine_;

    static std::array<unsigned long int, 20> const primes1;
    static std::array<unsigned long int, 20> const primes2;
    unsigned long int prime1_ = 0; // Will be initialized to random value from above
    unsigned long int prime2_ = 0; // Will be initialized to random value from above
    unsigned long int random_stations_added_ = 0; // Counter for random stations added
    unsigned long int random_regions_added_ = 0; // Counter for random regions added
    unsigned long int random_trains_added_ = 0; // Counter for random trains added
    void init_primes();
    Name n_to_name(unsigned long int n);
    StationID n_to_stationid(unsigned long int n);
    RegionID n_to_regionid(unsigned long int n);
    TrainID n_to_trainid(unsigned long int n);
    Coord n_to_coord(unsigned long int n);


    enum class StopwatchMode { OFF, ON, NEXT };
    StopwatchMode stopwatch_mode = StopwatchMode::OFF;

    enum class ResultType { NOTHING, IDLIST, ROUTE, TRAINS };
    using CmdResultIDs = std::pair<std::vector<RegionID>, std::vector<StationID>>;
    using CmdResultTrains = std::vector<std::tuple<TrainID, StationID, StationID, Time>>;
    using CmdResultRoute = std::vector<std::tuple<TrainID, StationID, StationID, Time, Distance>>;
    using CmdResult = std::pair<ResultType, std::variant<CmdResultIDs, CmdResultTrains, CmdResultRoute>>;
    CmdResult prev_result;
    bool view_dirty = true;

    TestStatus test_status_ = TestStatus::NOT_RUN;

    using MatchIter = std::smatch::const_iterator;
    struct CmdInfo
    {
        std::string cmd;
        std::string info;
        std::string param_regex_str;
        CmdResult(MainProgram::*func)(std::ostream& output, MatchIter begin, MatchIter end);
        void(MainProgram::*testfunc)();
        std::regex param_regex = {};
    };
    static std::vector<CmdInfo> cmds_;
    // Regex objects and their initialization
    std::regex cmds_regex_;
    std::regex coords_regex_;
    std::regex stationtimes_regex_;
    std::regex times_regex_;
    std::regex commands_regex_;
    std::regex sizes_regex_;
    void init_regexs();


    CmdResult cmd_station_count(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_clear_all(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_all_stations(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_add_station(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_station_info(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_find_station_with_coord(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_change_station_coord(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_add_departure(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_remove_departure(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_station_departures_after(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_add_region(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_all_regions(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_region_info(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_add_subregion_to_region(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_add_station_to_region(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_station_in_regions(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_all_subregions_of_region(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_stations_closest_to(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_remove_station(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_common_parent_of_regions(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_add_train(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_next_stations_from(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_train_stations_from(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_clear_trains(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_route_any(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_route_least_stations(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_route_with_cycle(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_route_shortest_distance(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_route_earliest_arrival(std::ostream& output, MatchIter begin, MatchIter end);

    CmdResult help_command(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_randseed(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_random_stations(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_random_trains(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_read(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_testread(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_stopwatch(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_perftest(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_comment(std::ostream& output, MatchIter begin, MatchIter end);

    void test_all_stations();
    void test_get_functions(StationID id);
    void test_station_info();
    void test_find_station_with_coord();
    void test_change_station_coord();
    void test_add_departure();
    void test_remove_departure();
    void test_station_departures_after();
    void test_region_info();
    void test_station_in_regions();
    void test_all_subregions_of_region();
    void test_stations_closest_to();
    void test_remove_station();
    void test_common_parent_of_regions();
    void test_next_stations_from();
    void test_train_stations_from();
    void test_route_any();
    void test_route_least_stations();
    void test_route_with_cycle();
    void test_route_shortest_distance();
    void test_route_earliest_arrival();
    void test_random_stations();
    void test_random_trains();

    void add_random_stations_regions(unsigned int size, Coord min = {1,1}, Coord max = {10000, 10000});
    void add_random_trains(unsigned int n);
    Distance calc_distance(Coord c1, Coord c2);
    std::string print_station(StationID id, std::ostream& output, bool nl = true);
    std::string print_station_brief(StationID id, std::ostream& output, bool nl = true);
    std::string print_region(RegionID id, std::ostream& output, bool nl = true);
    std::string print_station_name(StationID id, std::ostream& output, bool nl = true);
    std::string print_coord(Coord coord, std::ostream& output, bool nl = true);
    std::string print_train(TrainID id, std::ostream& output, bool nl = true);
    std::string print_time(Time time, std::ostream& output, bool nl = true);

    template <typename Type>
    Type random(Type start, Type end);
    template <typename To>
    static To convert_string_to(std::string from);
    template <typename From>
    static std::string convert_to_string(From from);

    template<StationID(Datastructures::*MFUNC)()>
    CmdResult NoParStationCmd(std::ostream& output, MatchIter begin, MatchIter end);

    template<std::vector<StationID>(Datastructures::*MFUNC)()>
    CmdResult NoParListCmd(std::ostream& output, MatchIter begin, MatchIter end);

    template<StationID(Datastructures::*MFUNC)()>
    void NoParStationTestCmd();

    template<std::vector<StationID>(Datastructures::*MFUNC)()>
    void NoParListTestCmd();

    friend class MainWindow;
};

template <typename Type>
Type MainProgram::random(Type start, Type end)
{
    auto range = end-start;
    assert(range != 0 && "random() with zero range!");

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine_);

    return static_cast<Type>(start+num);
}

template <typename To>
To MainProgram::convert_string_to(std::string from)
{
    std::istringstream istr(from);
    To result;
    istr >> std::noskipws >> result;
    if (istr.fail() || !istr.eof())
    {
        throw std::invalid_argument("Cannot convert string to required type");
    }
    return result;
}

template <typename From>
std::string MainProgram::convert_to_string(From from)
{
    std::ostringstream ostr;
    ostr << from;
    if (ostr.fail())
    {
        throw std::invalid_argument("Cannot convert type to string");
    }
    return ostr.str();
}

template<StationID(Datastructures::*MFUNC)()>
MainProgram::CmdResult MainProgram::NoParStationCmd(std::ostream& /*output*/, MatchIter /*begin*/, MatchIter /*end*/)
{
    auto result = (ds_.*MFUNC)();
    return {ResultType::IDLIST, CmdResultIDs{{}, {result}}};
}

template<std::vector<StationID>(Datastructures::*MFUNC)()>
MainProgram::CmdResult MainProgram::NoParListCmd(std::ostream& /*output*/, MatchIter /*begin*/, MatchIter /*end*/)
{
    auto result = (ds_.*MFUNC)();
    return {ResultType::IDLIST, CmdResultIDs{{}, result}};
}

template<StationID(Datastructures::*MFUNC)()>
void MainProgram::NoParStationTestCmd()
{
    (ds_.*MFUNC)();
}

template<std::vector<StationID>(Datastructures::*MFUNC)()>
void MainProgram::NoParListTestCmd()
{
    (ds_.*MFUNC)();
}


#ifdef USE_PERF_EVENT
extern "C"
{
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
    int ret;

    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
                   group_fd, flags);
    return ret;
}
}
#endif

class MainProgram::Stopwatch
{
public:
    using Clock = std::chrono::high_resolution_clock;

    Stopwatch(bool use_counter = false) : use_counter_(use_counter)
    {
#ifdef USE_PERF_EVENT
        if (use_counter_)
        {
            memset(&pe_, 0, sizeof(pe_));
            pe_.type = PERF_TYPE_HARDWARE;
            pe_.size = sizeof(pe_);
            pe_.config = PERF_COUNT_HW_INSTRUCTIONS;
            pe_.disabled = 1;
            pe_.exclude_kernel = 1;
            pe_.exclude_hv = 1;

            fd_ = perf_event_open(&pe_, 0, -1, -1, 0);
            if (fd_ == -1) {
                throw "Couldn't open perf events!";
            }
        }
#endif
        reset();
    }

    ~Stopwatch()
    {
#ifdef USE_PERF_EVENT
        if (use_counter_)
        {
            close(fd_);
        }
#endif
    }

    void start()
    {
        running_ = true;
        starttime_ = Clock::now();
#ifdef USE_PERF_EVENT
        if (use_counter_)
        {
            ioctl(fd_, PERF_EVENT_IOC_RESET, 0);
            read(fd_, &startcount_, sizeof(startcount_));
            ioctl(fd_, PERF_EVENT_IOC_ENABLE, 0);
        }
#endif
    }

    void stop()
    {
        running_ = false;
#ifdef USE_PERF_EVENT
        if (use_counter_)
        {
            ioctl(fd_, PERF_EVENT_IOC_DISABLE, 0);
            long long c; read(fd_, &c, sizeof(c));
            counter_ += (c - startcount_);
        }
#endif
        elapsed_ += (Clock::now() - starttime_);
    }

    void reset()
    {
        running_ = false;
#ifdef USE_PERF_EVENT
        if (use_counter_)
        {
            ioctl(fd_, PERF_EVENT_IOC_DISABLE, 0);
            ioctl(fd_, PERF_EVENT_IOC_RESET, 0);
            counter_ = 0;
        }
#endif
        elapsed_ = elapsed_.zero();
    }

    double elapsed()
    {
        if (!running_)
        {
            return static_cast<std::chrono::duration<double>>(elapsed_).count();
        }
        else
        {
            auto total = elapsed_ + (Clock::now() - starttime_);
            return static_cast<std::chrono::duration<double>>(total).count();
        }
    }

#ifdef USE_PERF_EVENT
    long long count()
    {
        if (use_counter_)
        {
            if (!running_)
            {
                return counter_;
            }
            else
            {
                long long c;
                read(fd_, &c, sizeof(c));
                return counter_ + (c - startcount_);
            }
        }
        else
        {
            assert(!"perf_event not enabled during StopWatch creation!");
        }
    }
#endif

private:
    std::chrono::time_point<Clock> starttime_;
    Clock::duration elapsed_ = Clock::duration::zero();
    bool running_ = false;

    bool use_counter_;
#ifdef USE_PERF_EVENT
    struct perf_event_attr pe_;
    long long startcount_ = 0;
    long long counter_ = 0;
    int fd_ = 0;
#endif
};


#endif // MAINPROGRAM_HH
