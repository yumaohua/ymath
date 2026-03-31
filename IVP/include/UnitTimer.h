#ifndef UNIT_TIMER_H
#define UNIT_TIMER_H

#include <chrono>
#include <map>
#include <string>
#include <vector>
#include <iostream>

struct CPUTimer {
    using HRC = std::chrono::high_resolution_clock; 
    HRC::time_point start;

    CPUTimer() { reset(); }

    void reset() { start = HRC::now(); } 

    double operator()() const {
        HRC::duration e = HRC::now() - start; 
        return 1e-9 * std::chrono::duration_cast<std::chrono::nanoseconds>(e).count(); 
    }
};


class UnitTimer {
private:
    CPUTimer alltimer_; 
    std::map<std::string, int> unit_;
    std::vector<CPUTimer> timer_; 
    std::vector<double> time_;

public:
    static UnitTimer &getInstance() {
        static UnitTimer singleton;
        return singleton;
    }

public:
    void begin(std::string name);
    void end(std::string name); 
    void reset(); 
    void report() const;
    double getTime(std::string name) const {
    if (unit_.find(name) != unit_.end()) {
        int index = unit_.at(name);
        return time_[index];
    }
    std::cerr << "Task '" << name << "' does not exist.\n";
    return 0.0;
}

private:
    UnitTimer() = default; 
    UnitTimer(const UnitTimer&) = default; 
    UnitTimer& operator = (const UnitTimer&) = default; 
    ~UnitTimer() = default; 
};

#endif
