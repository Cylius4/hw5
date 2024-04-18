#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
        const AvailabilityMatrix& avail,
        const size_t dailyNeed,
        const size_t maxShifts,
        DailySchedule& sched,
        vector<size_t>& workerShifts,
        vector<vector<bool>>& occupied,
        size_t day,
        size_t slot
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }

    sched.clear();
    sched.resize(avail.size(), vector<Worker_T>(dailyNeed, INVALID_ID));
    vector<size_t> workerShifts(avail[0].size(), 0);
    vector<vector<bool>> occupied(avail.size(), vector<bool>(avail[0].size(), false));
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, occupied, 0, 0);
}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& workerShifts,
    vector<vector<bool>>& occupied,
    size_t day,
    size_t slot
){
    // Base case
    if(day == avail.size()){
        return true;
    }
    // Recursive case
    for(size_t worker = 0; worker < avail[day].size(); worker++){
        if(avail[day][worker] && workerShifts[worker] < maxShifts && !occupied[day][worker]){
            sched[day][slot] = worker;
            workerShifts[worker]++;
            occupied[day][worker] = true;
            if (slot == dailyNeed - 1){
                if(scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, occupied, day + 1, 0)){
                    return true;
                }
            } else {
                if(scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, occupied, day, slot + 1)){
                    return true;
                }
            }
            workerShifts[worker]--;
            occupied[day][worker] = false;
        }
    }
    return false;
}