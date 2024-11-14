#include<bits/stdc++.h>
using namespace std;

// Structure to store process details
struct Process{
    int id;      // Process ID
    int at;      // Arrival time
    int bt;      // Burst time
    int wt;      // Waiting time
    int tat;     // Turnaround time
};

// Comparator function to sort processes by their ID
bool comp(Process a, Process b){
    return a.id < b.id;
}

// Function to calculate the average waiting time
float avgWT(vector<Process>& p){
    int n = p.size();
    p[0].wt = 0;   // First process has no waiting time
    float val = 0;
    // Calculate waiting time for each process
    for(int i = 1; i < n; i++){
        // Waiting time = (Previous process completion time) - Arrival time of current process
        p[i].wt = p[i-1].at + p[i-1].bt + p[i-1].wt - p[i].at;
        if(p[i].wt < 0) p[i].wt = 0;   // Waiting time cannot be negative
        val += p[i].wt;   // Accumulate the waiting times
    }
    return val / float(n);   // Return average waiting time
}

// Function to calculate the average turnaround time
float avgTAT(vector<Process>& p){
    int n = p.size();
    p[0].tat = p[0].bt;   // Turnaround time for the first process is equal to its burst time
    float val = 0;
    // Calculate turnaround time for each process
    for(int i = 1; i < n; i++){
        p[i].tat = p[i].wt + p[i].bt;   // Turnaround time = Waiting time + Burst time
        val += p[i].tat;   // Accumulate the turnaround times
    }
    return val / float(n);   // Return average turnaround time
}

int main(){
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;  // Take the number of processes as input
    vector<Process> p(n);  // Create a vector to store the processes

    cout << "Enter process details (ID Arrival Time Burst Time):\n";
    // Input process details: ID, Arrival time, and Burst time
    for(int i = 0; i < n; i++){
        cin >> p[i].id >> p[i].at >> p[i].bt;
        p[i].wt = 0;  // Initialize waiting time to 0
        p[i].tat = 0; // Initialize turnaround time to 0
    }

    // Sort processes by their ID (optional, but required to display the output in correct order)
    sort(p.begin(), p.end(), comp);

    // Display the average waiting time and turnaround time
    cout << "\nAverage Waiting Time: " << avgWT(p) << endl;
    cout << "Average Turnaround Time: " << avgTAT(p) << endl;

    // Optionally, display the individual waiting and turnaround times for each process
    for(int i = 0; i < n; i++){
        cout << "Process ID: " << p[i].id << " | Waiting Time: " << p[i].wt << " | Turnaround Time: " << p[i].tat << endl;
    }
}




























/*
 Sure! Let’s enhance the understanding of the **First-Come-First-Serve (FCFS)** scheduling algorithm by using a real-world example along with the process scheduling.

 ---

 ### **Real-World Example: First-Come-First-Serve Scheduling**

 Imagine a **bank** where customers come to the counter to be served. The customers arrive at different times, and they need to wait in line based on the order of their arrival. The bank staff serves each customer one by one, with no interruptions (this is like **non-preemptive scheduling**).

 In this case:

 - Each customer is a **process**.
 - The **arrival time (AT)** is the time when a customer arrives at the bank.
 - The **burst time (BT)** is the amount of time the customer requires to be served by the bank teller.
 - The **waiting time (WT)** is the time a customer waits before their service starts.
 - The **turnaround time (TAT)** is the total time from when a customer arrives until they leave the bank.

 ### **Bank Example with Customers:**

 Let’s say we have 5 customers in a bank with the following arrival times and service durations (burst times):

 | Customer ID | Arrival Time (AT) | Service Time (BT) |
 |-------------|-------------------|-------------------|
 | C1          | 0                 | 4                 |
 | C2          | 1                 | 3                 |
 | C3          | 2                 | 5                 |
 | C4          | 3                 | 2                 |
 | C5          | 4                 | 1                 |

 These customers arrive at the bank and will be served in the order they arrive (First-Come-First-Serve). So, **C1** will be served first, then **C2**, and so on.

 ---

 ### **Step-by-Step Process Execution:**

 1. **C1** arrives at time 0, and the bank starts serving C1 immediately (waiting time = 0).
    - **Waiting Time for C1**: 0
    - **Turnaround Time for C1** = Service Time (4) = 4

 2. **C2** arrives at time 1, but C1 is still being served. After C1 finishes at time 4, C2 starts.
    - **Waiting Time for C2** = Time when service starts (4) - Arrival Time (1) = 3
    - **Turnaround Time for C2** = Waiting Time (3) + Service Time (3) = 6

 3. **C3** arrives at time 2, but C1 and C2 are being served. C3 starts after C2 finishes at time 7.
    - **Waiting Time for C3** = Time when service starts (7) - Arrival Time (2) = 5
    - **Turnaround Time for C3** = Waiting Time (5) + Service Time (5) = 10

 4. **C4** arrives at time 3, but C1, C2, and C3 are in the queue. C4 starts after C3 finishes at time 12.
    - **Waiting Time for C4** = Time when service starts (12) - Arrival Time (3) = 9
    - **Turnaround Time for C4** = Waiting Time (9) + Service Time (2) = 11

 5. **C5** arrives at time 4, but C1, C2, C3, and C4 are still being served. C5 starts after C4 finishes at time 14.
    - **Waiting Time for C5** = Time when service starts (14) - Arrival Time (4) = 10
    - **Turnaround Time for C5** = Waiting Time (10) + Service Time (1) = 11

 ---

 ### **Detailed Table:**

 | Customer ID | Arrival Time (AT) | Service Time (BT) | Waiting Time (WT) | Turnaround Time (TAT) |
 |-------------|-------------------|-------------------|-------------------|-----------------------|
 | C1          | 0                 | 4                 | 0                 | 4                     |
 | C2          | 1                 | 3                 | 3                 | 6                     |
 | C3          | 2                 | 5                 | 5                 | 10                    |
 | C4          | 3                 | 2                 | 9                 | 11                    |
 | C5          | 4                 | 1                 | 10                | 11                    |

 ---

 ### **Average Waiting Time (AWT) Calculation:**

 \[
 \text{AWT} = \frac{0 + 3 + 5 + 9 + 10}{5} = \frac{27}{5} = 5.4 \, \text{units}
 \]

 ---

 ### **Average Turnaround Time (ATAT) Calculation:**

 \[
 \text{ATAT} = \frac{4 + 6 + 10 + 11 + 11}{5} = \frac{42}{5} = 8.4 \, \text{units}
 \]

 ---

 ### **Real-World Insights:**

 In a real-world scenario, this **First-Come-First-Serve (FCFS)** scheduling algorithm works similarly to how we expect customers to be served in a queue. It’s simple and easy to implement, but it has certain limitations:

 1. **Convoy Effect**: If a customer with a large service time (like C3 in this example) arrives early, it causes all subsequent customers to wait longer than they would if the customers were prioritized based on their service time. This is the **convoy effect**, where long processes delay the shorter ones.

 2. **Inefficiency**: If a customer arrives later but requires less service time, they still have to wait behind all the previous customers, even if they could have been served faster. For instance, **C5**, despite having a service time of only 1 unit, has to wait a long time because of the earlier customers.

 ---

 ### **Conclusion:**

 The **FCFS algorithm** is a **non-preemptive** and **fair** approach, ensuring that customers (or processes) are served in the order they arrive. However, in practical applications like the bank, it can lead to inefficiencies and longer waiting times, especially if there’s a wide variation in service times. In such cases, other scheduling algorithms like **Shortest Job First (SJF)** or **Round Robin (RR)** may provide better performance by reducing waiting times and improving system throughput.
 */
