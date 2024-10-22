#ifndef PROCESS
#define PROCESS

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using std::cout;
using std::cin;
using std::vector;
using std::priority_queue;

#define endl '\n'

enum SchedulingAlgorithm {
    FCFS,
    SJF_PREEMPTIVE,
    SJF_NON_PREEMPTIVE,
    PRIORITY_PREEMPTIVE,
    PRIORITY_NON_PREEMPTIVE,
    RR  
};

class process {
public:
    int arrival_time;
    int burst_time;
    int priority;
    int entery_number;
    int waiting_time;
    int start_time;
    int terminate_time;
    int turnaround_time;
    float normalized_turnaround_time;

    bool operator<(const process &b) const {
        return arrival_time > b.arrival_time;
    }
};

int wating_t(int t_t, int a_t, int b_t) {
    return t_t - a_t - b_t;
}

int turnaround_t(int b_t, int w_t) {
    return b_t + w_t;
}

float normalized_t_t(int t_a_t, int b_t) {
    return static_cast<float>(t_a_t) / static_cast<float>(b_t);
}

void printing_pq(const vector<process>& processes) {
    for (const auto& proc : processes) {
        cout << "Process: " << proc.entery_number << endl;
        cout << "Arrival Time: " << proc.arrival_time << endl;
        cout << "Burst Time: " << proc.burst_time << endl;
        cout << "Priority: " << proc.priority << endl;
        cout << "Termination Time: " << proc.terminate_time << endl;
        cout << "Waiting Time: " << proc.waiting_time << endl;
        cout << "Turnaround Time: " << proc.turnaround_time << endl;
        cout << "Normalized Turnaround Time: " << proc.normalized_turnaround_time << endl;
    }
}

#endif
