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
bool scheduleHelper(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, DailySchedule& sched, std::vector<int>& shifts, size_t day);
bool isValid(const AvailabilityMatrix& avail, Worker_T worker, size_t day, const std::vector<int>& shifts, size_t maxShifts);

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
  // Add your code below
  size_t numDays = avail.size();
  size_t numWorkers = avail[0].size();

  std::vector<int> shifts(numWorkers, 0);
  sched.resize(numDays, std::vector<Worker_T>(dailyNeed, INVALID_ID));

  return scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, 0);
}

bool scheduleHelper(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, DailySchedule& sched, std::vector<int>& shifts, size_t day) {
  if (day == sched.size()) {
    return true; // Successfully scheduled all days
  }

  std::vector<Worker_T>& todaysSchedule = sched[day];
  std::vector<Worker_T> candidates;

  // Build list of candidate workers for today
  for (Worker_T worker = 0; worker < avail[day].size(); ++worker) {
    if (isValid(avail, worker, day, shifts, maxShifts)) {
      candidates.push_back(worker);
    }
  }

  // Try to schedule each candidate
  for (Worker_T worker : candidates) {
    todaysSchedule[dailyNeed - 1] = worker; // Place worker in today's schedule
    shifts[worker]++; // Increment the shift count for this worker

    if (dailyNeed > 1) {
      // More workers needed today
      if (scheduleHelper(avail, dailyNeed - 1, maxShifts, sched, shifts, day)) {
        return true;
      }
    } 
    else {
      // Move to the next day
      if (scheduleHelper(avail, sched[day].size(), maxShifts, sched, shifts, day + 1)) {
        return true;
        }
      }

    // Backtrack
    shifts[worker]--;
    todaysSchedule[dailyNeed - 1] = INVALID_ID;
  }

  return false; // No valid schedule found with current configuration
}

// Utility function to check if it's valid to schedule a worker on a given day
bool isValid(const AvailabilityMatrix& avail, Worker_T worker, size_t day, const std::vector<int>& shifts, size_t maxShifts) {
  return avail[day][worker] && shifts[worker] < maxShifts;
}