#include "UnitTimer.h"
#include <iostream>
#include <iomanip>
void UnitTimer::begin(std::string name) {
    if (unit_.find(name) == unit_.end()) {
        unit_[name] = timer_.size();
        timer_.emplace_back();
        time_.emplace_back(0.0);
    }
    timer_[unit_[name]].reset();
}

void UnitTimer::end(std::string name) {
    if (unit_.find(name) != unit_.end()) {
        int index = unit_[name];
        time_[index] += timer_[index]();
    }
}

void UnitTimer::reset() {
    alltimer_.reset();
    // for (auto& t : timer_) {
    //     t.reset();
    // }
    // std::fill(time_.begin(), time_.end(), 0.0);
    unit_.clear();
    timer_.clear();
    time_.clear();
}

void UnitTimer::report() const {
    double total_time = alltimer_();
    std::cout << "Total elapsed time: " << total_time << " seconds\n";
    for (const auto& [name, index] : unit_) {
        std::cout << std::setw(15) << name << ": " << time_[index] << " seconds\n";
    }
}
