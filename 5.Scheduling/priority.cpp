#include<bits/stdc++.h>
using namespace std;

struct Process {
    int id;      // Process ID
    int at;      // Arrival Time
    int bt;      // Burst Time
    int ct;      // Completion Time
    int wt;      // Waiting Time
    int tat;     // Turnaround Time
    int priority; // Priority (lower number means higher priority)
    bool comp;   // Completion status
};

bool arrivalSort(Process a, Process b) {
    if (a.at == b.at)
        return a.priority < b.priority; // Sort by priority if arrival times are the same
    return a.at < b.at; // Otherwise, sort by arrival time
}

void priorityScheduling(vector<Process>& p) {
    int n = p.size();
    int ctm = 0; // Current time
    int cnt = 0; // Count of completed processes

    while (cnt != n) {
        int ind = -1;
        int highestPriority = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!p[i].comp && p[i].at <= ctm && p[i].priority < highestPriority) {
                ind = i;
                highestPriority = p[i].priority;
            }
        }

        if (ind == -1) {
            ctm++; // Increment time if no process is available to execute
        } else {
            // Execute the process
            p[ind].ct = ctm + p[ind].bt; // Completion time
            p[ind].tat = p[ind].ct - p[ind].at; // Turnaround time
            p[ind].wt = p[ind].tat - p[ind].bt; // Waiting time
            p[ind].comp = true; // Mark process as completed

            ctm = p[ind].ct; // Move current time to the completion time of this process
            cnt++; // Increase count of completed processes
        }
    }
}

void printResults(const vector<Process>& p) {

    cout << "---------------------------------------------------------------------------------------------\n";
    cout << "\t\t\t\tProcess Scheduling Results\n";
    cout << "---------------------------------------------------------------------------------------------\n";
    cout << "PID | Arrival Time | Burst Time | Priority | Completion Time | Waiting Time | Turnaround Time\n";
    cout << "---------------------------------------------------------------------------------------------\n";
    
    for (const auto& proc : p) {
        cout << setw(3)  << proc.id << "  | "
             << setw(12) << proc.at << " | "
             << setw(10) << proc.bt << " | "
             << setw(8)  << proc.priority << " | "
             << setw(15) << proc.ct << " | "
             << setw(12) << proc.wt << " | "
             << setw(15) << proc.tat << endl;
    }
    cout << "---------------------------------------------------------------------------------------------\n";
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;
    vector<Process> p(n);

    cout << "Enter process ID, Arrival Time, Burst Time, and Priority for each process:\n";
    for (int i = 0; i < n; i++) {
        cin >> p[i].id >> p[i].at >> p[i].bt >> p[i].priority;
        p[i].comp = false; // Initially, no process is completed
    }

    // Sort processes by arrival time, and by priority if arrival times are equal
    sort(p.begin(), p.end(), arrivalSort);

    // Apply Priority Scheduling
    priorityScheduling(p);

    // Print the results in a structured format
    printResults(p);

    return 0;
}






/*
Priority Scheduling is a non-preemptive scheduling algorithm where each process is assigned a priority, and the process with the highest priority (lowest priority number) is executed first. If two processes have the same arrival time, the one with the lower priority number (higher priority) is chosen first. The goal is to minimize waiting time and turnaround time while giving precedence to more important tasks based on the priority.

### Key Concepts:
1. **Priority**: Each process is assigned a priority value, with lower numbers indicating higher priority. The process with the smallest priority value is scheduled first.
2. **Arrival Time (AT)**: The time at which a process arrives in the ready queue.
3. **Burst Time (BT)**: The total amount of time a process requires for execution on the CPU.
4. **Completion Time (CT)**: The time at which a process finishes execution.
5. **Turnaround Time (TAT)**: The total time from the arrival of a process until its completion. It is calculated as:
   - TAT = CT - AT
6. **Waiting Time (WT)**: The time a process spends in the ready queue before being executed. It is calculated as:
   - WT = TAT - BT

### Algorithm:
1. **Sort the Processes**: First, sort the processes based on arrival time. If two processes have the same arrival time, sort them by priority (lower priority number means higher priority).
2. **Select the Process to Execute**: The process with the highest priority (lowest priority number) is selected for execution.
3. **Execute the Process**: Calculate the completion time, turnaround time, and waiting time for the selected process.
4. **Update the Current Time**: Move the current time to the completion time of the process that was just executed.
5. **Repeat Until All Processes Are Executed**: Repeat the above steps until all processes are completed.

### Example:

Consider the following input where we have 3 processes:


 Where:
 - Process 1 arrives at time 0, requires 4 units of burst time, and has priority 1.
 - Process 2 arrives at time 1, requires 3 units of burst time, and has priority 2.
 - Process 3 arrives at time 2, requires 1 unit of burst time, and has priority 3.

 ### Step-by-Step Execution:

 1. **Initial Sorting**: Sort processes by arrival time and by priority if arrival times are the same:
    - Sorted processes: Process 1 (AT=0, BT=4, Priority=1), Process 2 (AT=1, BT=3, Priority=2), Process 3 (AT=2, BT=1, Priority=3).

 2. **Time = 0**: The current time is 0, and Process 1 (priority 1) is selected for execution since it has the highest priority (priority 1). It starts at time 0 and completes after 4 units of time (completion time = 4).
    - Completion time (CT) = 0 + 4 = 4
    - Turnaround time (TAT) = CT - AT = 4 - 0 = 4
    - Waiting time (WT) = TAT - BT = 4 - 4 = 0

 3. **Time = 4**: The current time is now 4. Process 2 (priority 2) has arrived and is the next highest priority. It starts at time 4 and completes after 3 units of time (completion time = 7).
    - Completion time (CT) = 4 + 3 = 7
    - Turnaround time (TAT) = CT - AT = 7 - 1 = 6
    - Waiting time (WT) = TAT - BT = 6 - 3 = 3

 4. **Time = 7**: The current time is now 7. Process 3 (priority 3) has arrived and is the only remaining process. It starts at time 7 and completes after 1 unit of time (completion time = 8).
    - Completion time (CT) = 7 + 1 = 8
    - Turnaround time (TAT) = CT - AT = 8 - 2 = 6
    - Waiting time (WT) = TAT - BT = 6 - 1 = 5

 ### Final Output:
 After scheduling the processes, the completion time, waiting time, and turnaround time for each process are as follows:
 Process 1: Completion Time = 4, Waiting Time = 0, Turnaround Time = 4 Process 2: Completion Time = 7, Waiting Time = 3, Turnaround Time = 6 Process 3: Completion Time = 8, Waiting Time = 5, Turnaround Time = 6


 ### Summary:
 In Priority Scheduling, processes are executed based on their priority, with lower priority values indicating higher importance. The algorithm ensures that processes with higher priority (lower priority number) are completed before those with lower priority. This method can reduce waiting times for high-priority processes but may lead to starvation for low-priority processes. The output provides valuable metrics like completion time, waiting time, and turnaround time to evaluate the efficiency of the scheduling algorithm.
 
 
 
 
 
 
 Enter the number of processes: 5
 Enter process ID, Arrival Time, Burst Time, and Priority for each process:
 1 0 4 3
 2 1 3 2
 3 2 5 1
 4 3 2 4
 5 4 1 5


 */
