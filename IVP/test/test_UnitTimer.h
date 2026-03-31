#pragma once
#include "UnitTimer.h"
#include <thread>
#include <iostream>

void simulate_work(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int test_UnitTimer() {
    std::cout<<"test_UnitTimer:"<<std::endl;
    UnitTimer& timer = UnitTimer::getInstance();

    timer.begin("Task1");
    simulate_work(100); 
    timer.end("Task1");

    timer.begin("Task2");
    simulate_work(200); 
    timer.end("Task2");

    timer.begin("Task1");
    simulate_work(150); 
    timer.end("Task1");

    timer.report();
    timer.reset();

    timer.begin("Task3");
    simulate_work(50);
    timer.end("Task3");

    timer.report();
    std::cout<<"-------------------------------------------------"<<std::endl;
    return 0;
}
