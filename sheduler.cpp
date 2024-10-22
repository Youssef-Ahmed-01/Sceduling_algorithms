#include "process.hpp"

vector<process> terminated;
priority_queue<process> process_ready_q;

void get_Num_of_processes(int& n_o_p) {
    cout << "Enter the number of processes: ";
    cin >> n_o_p;
    if (n_o_p <= 0) {
        cout << "Invalid Input" << endl;
        exit(1);
    }
}

void predefine() {
    cout << "Choose the algorithm you want to use" << endl
         << "1 - First come first served _FCFS_" << endl
         << "2 - Shortest job first _SJF_" << endl
         << "3 - Priority" << endl
         << "4 - Round Robin _RR_" << endl;
}

void define(int& algo_number) {
    short int x, y;
    predefine();
    cout << "Number: ";
    cin >> x;

    if (x == 1 || x == 4) {
        y = 0; // No preemption for FCFS and RR
    } else if (x == 2 || x == 3) {
        cout << "(0) preemptive \n(1) non-preemptive " << endl;
        cout << "Number: ";
        cin >> y;
        if (y != 0 && y != 1) {
            cout << "Invalid input " << endl;
            exit(1);
        }
    } else {
        cout << "Invalid inputs " << endl;
        exit(1);
    }

    algo_number = (x - 1) * 2 + y; // calculate algorithm number
}

void init_processes(priority_queue<process>& procs, bool is_priority, int number_of_processes) {
    int a_t, b_t, pr = 0;
    for (int i = 0; i < number_of_processes; i++) {
        process p;
        cout << "Enter Arrival Time, Burst Time" << (is_priority ? ", Priority" : "") << ": " << endl;
        cin >> a_t >> b_t;
        if (is_priority) cin >> pr;

        p.entery_number = i;
        p.arrival_time = a_t;
        p.burst_time = b_t;
        p.priority = is_priority ? pr : 0;
        procs.push(p);
    }
}

void scheduling(priority_queue<process> prq, int alg_numb, int number_of_processes) {
    priority_queue<process> temp = prq;
    int cycle = 0;
    vector<process> ready_queue;

    // FCFS
    if (alg_numb == 0) {
        while (!temp.empty()) {
            process p = temp.top();
            temp.pop();
            if (cycle < p.arrival_time) cycle = p.arrival_time; // idle time
            cycle += p.burst_time;
            p.terminate_time = cycle;
            p.waiting_time = p.terminate_time - p.arrival_time - p.burst_time;
            p.turnaround_time = p.burst_time + p.waiting_time;
            p.normalized_turnaround_time = normalized_t_t(p.turnaround_time, p.burst_time);
            terminated.push_back(p);
        }
    }
    
    // SJF Preemptive
    else if (alg_numb == 1) {
        while (true) {
            while (!temp.empty() && temp.top().arrival_time <= cycle) {
                ready_queue.push_back(temp.top());
                temp.pop();
            }
            if (ready_queue.empty()) {
                cycle++;
                continue;
            }
            sort(ready_queue.begin(), ready_queue.end(), [](const process& a, const process& b) {
                return a.burst_time < b.burst_time; // sort by burst time
            });
            process& current = ready_queue.front(); // get shortest job
            current.burst_time--;
            for (auto& p : ready_queue) {
                if (&p != &current) p.waiting_time++;
            }
            if (current.burst_time == 0) {
                current.terminate_time = ++cycle;
                current.turnaround_time = current.terminate_time - current.arrival_time;
                current.normalized_turnaround_time = normalized_t_t(current.turnaround_time, current.burst_time);
                terminated.push_back(current);
                ready_queue.erase(ready_queue.begin());
            } else {
                cycle++; // Process time slice consumed
            }
        }
    }

    // SJF Non-Preemptive
    else if (alg_numb == 2) {
        while (!temp.empty() || !ready_queue.empty()) {
            while (!temp.empty() && temp.top().arrival_time <= cycle) {
                ready_queue.push_back(temp.top());
                temp.pop();
            }
            if (ready_queue.empty()) {
                cycle++;
                continue;
            }
            sort(ready_queue.begin(), ready_queue.end(), [](const process& a, const process& b) {
                return a.burst_time < b.burst_time; // Sort by burst time
            });
            process& current = ready_queue.front();
            cycle += current.burst_time;
            current.terminate_time = cycle;
            current.waiting_time = current.terminate_time - current.arrival_time - current.burst_time;
            current.turnaround_time = current.burst_time + current.waiting_time;
            current.normalized_turnaround_time = normalized_t_t(current.turnaround_time, current.burst_time);
            terminated.push_back(current);
            ready_queue.erase(ready_queue.begin());
        }
    }

    // Priority Preemptive
    else if (alg_numb == 3) {
        while (number_of_processes > 0) {
            while (!temp.empty() && temp.top().arrival_time <= cycle) {
                ready_queue.push_back(temp.top());
                temp.pop();
            }
            if (ready_queue.empty()) {
                cycle++;
                continue;
            }
            sort(ready_queue.begin(), ready_queue.end(), [](const process& a, const process& b) {
                return a.priority < b.priority; // Sort by priority
            });
            process& current = ready_queue.front();
            current.burst_time--;
            for (auto& p : ready_queue) {
                if (&p != &current) p.waiting_time++;
            }
            if (current.burst_time == 0) {
                current.terminate_time = ++cycle;
                current.turnaround_time = current.terminate_time - current.arrival_time;
                current.normalized_turnaround_time = normalized_t_t(current.turnaround_time, current.burst_time);
                terminated.push_back(current);
                ready_queue.erase(ready_queue.begin());
                number_of_processes--; // Decrease the count of remaining processes
            } else {
                cycle++; // Process time slice consumed
            }
        }
    }

    // Priority Non-Preemptive
    else if (alg_numb == 4) {
        while (number_of_processes > 0) {
            while (!temp.empty() && temp.top().arrival_time <= cycle) {
                ready_queue.push_back(temp.top());
                temp.pop();
            }
            if (ready_queue.empty()) {
                cycle++;
                continue;
            }
            sort(ready_queue.begin(), ready_queue.end(), [](const process& a, const process& b) {
                return a.priority < b.priority; // Sort by priority
            });
            process& current = ready_queue.front();
            cycle += current.burst_time; // Process the whole burst time
            current.terminate_time = cycle;
            current.waiting_time = current.terminate_time - current.arrival_time - current.burst_time;
            current.turnaround_time = current.burst_time + current.waiting_time;
            current.normalized_turnaround_time = normalized_t_t(current.turnaround_time, current.burst_time);
            terminated.push_back(current);
            ready_queue.erase(ready_queue.begin());
            number_of_processes--; // Decrease the count of remaining processes
        }
    }
}

int main(int argc, char* argv[]) {
    int algo_number = 0;
    int number_of_processes = 0;

    define(algo_number);
    get_Num_of_processes(number_of_processes);
    init_processes(process_ready_q, (algo_number == 3 || algo_number == 4), number_of_processes);
    scheduling(process_ready_q, algo_number, number_of_processes);
    printing_pq(terminated);

    return 0;
}
