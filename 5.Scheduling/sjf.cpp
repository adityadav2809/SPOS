#include<bits/stdc++.h>
using namespace std;

// Structure to represent each process with necessary fields
struct Process {
    int id;        // Process ID
    int at;        // Arrival time
    int bt;        // Burst time (total time required for execution)
    int rt;        // Remaining time for execution (used in preemptive SJF)
    int ct;        // Completion time
    int wt;        // Waiting time
    int tat;       // Turnaround time (wt + bt)
    bool comp;     // Boolean flag to check if process is completed
};

// Comparator function to sort processes by arrival time initially
bool comp(Process a, Process b) {
    return a.at < b.at; // Return true if process a arrives before process b
}

// Preemptive Shortest Job First (SJF) scheduling function
void premptiveSJF(vector<Process>& p) {
    int n = p.size();    // Number of processes
    int cnt = 0;         // Counter to track number of completed processes
    int ctm = 0;         // Current time of the system (keeps track of simulation time)
    
    // Loop until all processes are completed
    while(cnt != n) {
        int ind = -1;      // Index of the process to be executed
        int mn = INT_MAX;  // Minimum remaining time (to choose the shortest job)

        // Find the process with the shortest remaining time that is ready to execute
        for(int i = 0; i < n; i++) {
            // Check if the process is not completed, it has arrived, and it has the shortest remaining time
            if(!p[i].comp && p[i].at <= ctm && p[i].rt < mn) {
                ind = i;  // Select this process for execution
                mn = p[i].rt;  // Update the minimum remaining time
            }
        }

        // If no process is ready to execute (i.e., all are waiting), increment the time
        if(ind == -1) {
            ctm++;  // Increment the current time
        } else {
            // Decrease the remaining time of the selected process
            p[ind].rt--;
            if(p[ind].rt == 0) {  // If the remaining time is zero, process is completed
                p[ind].comp = true;  // Mark the process as completed
                p[ind].ct = ctm + 1; // Completion time is the current time + 1
                p[ind].wt = p[ind].ct - p[ind].at - p[ind].bt; // Waiting time = Completion time - Arrival time - Burst time
                p[ind].tat = p[ind].wt + p[ind].bt; // Turnaround time = Waiting time + Burst time
                cnt++;  // Increment the completed process counter
            }
            ctm++;  // Increment the current time after each cycle
        }
    }
}

int main() {
    int n;
    cin >> n;  // Input number of processes
    vector<Process> p(n);  // Vector to hold all processes

    // Input the details for each process: ID, arrival time, and burst time
    for(int i = 0; i < n; i++) {
        cin >> p[i].id >> p[i].at >> p[i].bt;
        p[i].rt = p[i].bt;  // Initialize remaining time to burst time (before execution)
        p[i].comp = false;   // Initially, no process is completed
    }

    // Sort processes by arrival time before scheduling
    sort(p.begin(), p.end(), comp);

    // Call the preemptive SJF scheduling function
    premptiveSJF(p);

    // Sort processes by their IDs before outputting the result
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.id < b.id;  // Sort by process ID
    });

    // Improved output format with headers for the table
    cout << "\n============================================================================================\n";
    cout << "Process ID | Arrival Time | Burst Time | Completion Time | Waiting Time | Turnaround Time | \n";
    cout << "--------------------------------------------------------------------------------------------\n";
    
    // Output the results for each process in a formatted manner
    for(int i = 0; i < n; i++){
        cout << setw(11) << p[i].id    // Process ID
             << " | " << setw(12) << p[i].at  // Arrival Time
             << " | " << setw(10) << p[i].bt  // Burst Time
             << " | " << setw(15) << p[i].ct  // Completion Time
             << " | " << setw(12) << p[i].wt  // Waiting Time
             << " | " << setw(16) << p[i].tat // Turnaround Time
             << " | " << endl;
    }

    cout << "============================================================================================\n";

    return 0;
}
/*
 ### Theory of Shortest Job First (SJF) Scheduling Algorithm

 **Shortest Job First (SJF)** is a type of CPU scheduling algorithm that selects the process with the smallest burst time (the time required to execute the process) for execution next. It can be classified into two types:

 1. **Non-preemptive SJF (NP-SJF)**
 2. **Preemptive SJF (PSJF)**

 In **Non-preemptive SJF**, once a process starts executing, it cannot be preempted, even if a new process with a shorter burst time arrives. In contrast, **Preemptive SJF** (also called **Shortest Remaining Time First (SRTF)**) allows a currently executing process to be preempted if a new process with a shorter burst time arrives.

 ### Key Concepts:
 1. **Burst Time (BT)**: The amount of time a process needs for execution (CPU time).
 2. **Completion Time (CT)**: The time at which a process finishes its execution.
 3. **Waiting Time (WT)**: The amount of time a process spends waiting in the ready queue before execution begins. It is calculated as:
    \[
    \text{Waiting Time (WT)} = \text{Completion Time (CT)} - \text{Arrival Time (AT)} - \text{Burst Time (BT)}
    \]
 4. **Turnaround Time (TAT)**: The total time taken by a process to complete from the time of arrival to its completion. It is calculated as:
    \[
    \text{Turnaround Time (TAT)} = \text{Waiting Time (WT)} + \text{Burst Time (BT)} = \text{Completion Time (CT)} - \text{Arrival Time (AT)}
    \]

 ### Working of SJF Scheduling Algorithm:

 1. **Process Arrival**: Each process has an arrival time (AT) and a burst time (BT). The scheduler considers these factors when selecting a process for execution.
    
 2. **Preemption (in Preemptive SJF)**: In the case of preemptive SJF, the process with the shortest remaining burst time is selected for execution. If a new process arrives with a shorter burst time than the currently executing process, the current process will be preempted and the new process will start executing.

 3. **Non-preemptive Selection**: In non-preemptive SJF, once a process is selected for execution, it will run to completion. Other processes with shorter burst times that arrive while the process is executing will have to wait until the current process finishes.

 4. **Completion Time**: The process continues execution until its burst time reaches zero, after which it is considered complete, and the next process is selected for execution based on its burst time.

 5. **Scheduling Order**: The processes are sorted by burst time, and the scheduler picks the process with the shortest burst time that has arrived and is ready to execute.

 6. **Completion of all Processes**: The process continues until all processes have completed their execution.

 ### Example Walkthrough:
 Consider the following processes:

 | Process ID | Arrival Time (AT) | Burst Time (BT) |
 |------------|-------------------|-----------------|
 | P1         | 0                 | 4               |
 | P2         | 1                 | 3               |
 | P3         | 2                 | 5               |
 | P4         | 3                 | 2               |
 | P5         | 4                 | 1               |

 1. **Step 1**: At time 0, only P1 has arrived, so it starts executing.
 2. **Step 2**: At time 1, P2 arrives with a burst time of 3. But since P1 is already executing, and P1's remaining burst time is shorter than P2's burst time, P1 continues execution.
 3. **Step 3**: At time 2, P3 arrives with a burst time of 5. Again, P1 continues because its remaining burst time is the shortest.
 4. **Step 4**: At time 3, P4 arrives with a burst time of 2, but since P1 still has the shortest remaining burst time, it completes its execution first at time 4.
 5. **Step 5**: Now P2 (which has the shortest burst time of 3) executes next.
 6. **Step 6**: At time 4, P5 arrives with a burst time of 1, and since it has the shortest burst time, it executes next.
 7. **Step 7**: After P5 completes, the next process with the shortest burst time is P4, followed by P3.

 ### Advantages of SJF:
 1. **Optimal in terms of average waiting time**: SJF is optimal in terms of minimizing the average waiting time, as it always picks the shortest process to execute next, reducing the amount of time a process has to wait.
    
 2. **Simple to implement**: It is relatively straightforward to implement SJF if you know the burst times of all processes beforehand.

 ### Disadvantages of SJF:
 1. **Starvation**: Shorter processes may continue to preempt longer processes, causing the longer processes to wait indefinitely, leading to starvation.
    
 2. **Knowledge of burst time required**: In real-world systems, burst times are often not known in advance, and estimating them can be difficult. In such cases, the algorithm might not be practical without some approximation.

 ### Performance Metrics:

 1. **Average Waiting Time**: Since SJF minimizes the waiting time, it generally results in the best performance when compared to other scheduling algorithms (such as FCFS and Round Robin).
    
 2. **Average Turnaround Time**: SJF also minimizes the turnaround time, resulting in faster process completion overall.

 ### Conclusion:
 SJF is an efficient CPU scheduling algorithm, especially when processes are predictable and burst times are known ahead of time. It is best suited for systems where burst times are constant or can be easily estimated. However, it may not be practical in systems with dynamic process behavior or where preemption is not allowed.
 */
