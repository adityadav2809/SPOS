//round robin
#include <bits/stdc++.h>
using namespace std;

struct Process {
    int id;     // Process ID
    int at;     // Arrival Time
    int bt;     // Burst Time
    int rt;     // Remaining Time
    int ct;     // Completion Time
    int wt;     // Waiting Time
    int tat;    // Turnaround Time
};

void roundRobin(vector<Process>& p, int quantum) {
    int n = p.size();
    int ctm = 0; // Current time
    int cnt = 0; // Count of completed processes
    queue<int> q; // Queue to hold the indexes of processes
    vector<bool> inQueue(n, false); // Track if a process is in the queue

    // Sort by arrival time
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.at < b.at;
    });

    // Push the first process into the queue
    q.push(0);
    inQueue[0] = true;

    while (cnt != n) {
        if (q.empty()) {
            // If no processes are in the queue, increment time until a process arrives
            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && p[i].at <= ctm) {
                    q.push(i);
                    inQueue[i] = true;
                    break;
                }
            }
            ctm++;
            continue;
        }

        int i = q.front();
        q.pop();
        inQueue[i] = false;

        // Execute the process for a time slice (quantum)
        if (p[i].rt > quantum) {
            ctm += quantum;
            p[i].rt -= quantum;
        } else {
            ctm += p[i].rt;
            p[i].rt = 0;

            // Process is completed
            p[i].ct = ctm;
            p[i].tat = p[i].ct - p[i].at;
            p[i].wt = p[i].tat - p[i].bt;
            cnt++;
        }

        // Add newly arrived processes to the queue
        for (int j = 0; j < n; j++) {
            if (!inQueue[j] && p[j].at <= ctm && p[j].rt > 0) {
                q.push(j);
                inQueue[j] = true;
            }
        }

        // If the process is not finished, put it back into the queue
        if (p[i].rt > 0) {
            q.push(i);
            inQueue[i] = true;
        }
    }
}

int main() {
    int n, quantum;
    cout << "Enter the number of processes: ";
    cin >> n;
    cout << "Enter the time quantum: ";
    cin >> quantum;

    vector<Process> p(n);
    cout << "Enter process ID, Arrival Time, and Burst Time for each process:\n";
    for (int i = 0; i < n; i++) {
        cin >> p[i].id >> p[i].at >> p[i].bt;
        p[i].rt = p[i].bt; // Initialize remaining time with burst time
    }

    // Apply Round Robin Scheduling
    roundRobin(p, quantum);

    // Display the results
    cout << "\n+------------+--------------+------------+----------------+--------------+----------------+\n";
    cout << "| Process ID | Arrival Time | Burst Time | Completion Time | Waiting Time | Turnaround Time |\n";
    cout << "+------------+--------------+------------+----------------+--------------+----------------+\n";

    for (int i = 0; i < n; i++) {
        cout << "| " << setw(10) << p[i].id << " | "
             << setw(12) << p[i].at << " | "
             << setw(10) << p[i].bt << " | "
             << setw(14) << p[i].ct << " | "
             << setw(12) << p[i].wt << " | "
             << setw(16) << p[i].tat << " |\n";
    }

    cout << "+------------+--------------+------------+----------------+--------------+----------------+\n";

    return 0;
}





/*
    Round Robin Scheduling Algorithm:

    The Round Robin (RR) scheduling algorithm is a pre-emptive scheduling algorithm where each process is assigned a fixed time slice or time quantum to execute. The key feature of this algorithm is that processes are executed in a circular order, meaning after one process uses its time quantum, it is placed at the end of the ready queue, and the next process gets the CPU. If a process does not finish in its time quantum, it is put back into the queue with the remaining burst time.

    **Steps:**
    1. Processes arrive in the ready queue based on their arrival time (AT).
    2. The CPU scheduler picks the first process in the queue and allows it to run for a time quantum.
    3. If the process finishes within the quantum, it is removed from the queue, and its completion time is recorded.
    4. If the process doesn't finish, it is placed back into the queue with its remaining burst time.
    5. The process continues this cycle until all processes are completed.
    6. After the completion of all processes, we calculate:
        - **Completion Time (CT):** Time when a process finishes.
        - **Waiting Time (WT):** Time spent waiting in the ready queue before execution. It’s calculated as:
          `WT = Turnaround Time (TAT) - Burst Time (BT)`
        - **Turnaround Time (TAT):** Total time from arrival to completion. It’s calculated as:
          `TAT = Completion Time (CT) - Arrival Time (AT)`

    **Example:**

    Let’s consider an example with 3 processes:

    | Process ID | Arrival Time (AT) | Burst Time (BT) |
    |------------|------------------|-----------------|
    | P1         | 0                | 5               |
    | P2         | 1                | 3               |
    | P3         | 2                | 8               |

    **Time Quantum = 4**

    **Step-by-Step Execution:**

    - **Time 0 to 4:** Process P1 starts and executes for 4 units (since time quantum is 4). After 4 units, the remaining burst time for P1 is 1 unit. Process P1 is put back into the queue.
    - **Time 4 to 7:** Process P2 starts and executes for 3 units (its burst time). Process P2 completes at time 7.
    - **Time 7 to 8:** Process P3 starts and executes for 4 units. After 4 units, the remaining burst time for P3 is 4 units. Process P3 is put back into the queue.
    - **Time 8 to 9:** Process P1, which has 1 unit left, executes and completes at time 9.
    - **Time 9 to 13:** Process P3 executes for the remaining 4 units and completes at time 13.

    **Completion, Turnaround, and Waiting Times:**

    - **For P1:**
        - Completion Time (CT) = 9
        - Turnaround Time (TAT) = 9 - 0 = 9
        - Waiting Time (WT) = 9 - 5 = 4

    - **For P2:**
        - Completion Time (CT) = 7
        - Turnaround Time (TAT) = 7 - 1 = 6
        - Waiting Time (WT) = 6 - 3 = 3

    - **For P3:**
        - Completion Time (CT) = 13
        - Turnaround Time (TAT) = 13 - 2 = 11
        - Waiting Time (WT) = 11 - 8 = 3

    **Output:**

    | Process ID | Arrival Time | Burst Time | Completion Time | Waiting Time | Turnaround Time |
    |------------|--------------|------------|-----------------|--------------|-----------------|
    | P1         | 0            | 5          | 9               | 4            | 9               |
    | P2         | 1            | 3          | 7               | 3            | 6               |
    | P3         | 2            | 8          | 13              | 3            | 11              |

    **Conclusion:**
    - Round Robin scheduling is simple and ensures fair CPU allocation by giving each process a chance to execute in a cyclic manner.
    - However, the algorithm's performance can be affected by the time quantum:
        - If the quantum is too large, processes may end up waiting too long.
        - If the quantum is too small, processes may be context-switched too frequently, leading to overhead.

    This algorithm works best for scenarios where processes are interactive and require quick responses.
 
 Explanation of the Example:
 Processes:

 P1 arrives at time 0 with a burst time of 5 units.
 P2 arrives at time 1 with a burst time of 3 units.
 P3 arrives at time 2 with a burst time of 8 units.
 Execution Flow:

 At time 0, P1 is the first process to arrive and gets executed for 4 units (quantum). After this, P1 still needs 1 unit of time to complete.
 At time 1, P2 arrives and starts executing for its full burst time of 3 units.
 At time 2, P3 arrives and starts executing for 4 units (its quantum).
 The process continues with each process being given a turn based on the quantum, and the remaining burst time of unfinished processes is updated.
 Completion, Turnaround, and Waiting Time Calculation:

 Completion Time (CT) is when the process finishes execution.
 Turnaround Time (TAT) is the total time taken from arrival to completion.
 Waiting Time (WT) is the total time a process waits in the ready queue (turnaround time minus burst time).
 This example illustrates how processes are handled in Round Robin scheduling, the time quantum impacts their execution, and how the metrics like waiting and turnaround times are calculated.








 5
 1 0 4 3
 2 1 3 2
 3 2 5 1
 4 3 2 4
 5 4 1 5
 
 
 
*/
