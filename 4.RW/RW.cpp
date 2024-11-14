#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

class RW {
    // Mutex for readers to manage mutual exclusion for reader count
    mutex readMutex;

    // Condition variable for writer to ensure exclusive access
    condition_variable writeCV;

    // Reader count to track active readers
    static int rc;

    // Mutex for writers to prevent multiple writers
    mutex writeMutex;

public:
    // Reader function representing reader process
    void Reader(int id) {
        try {
            // Acquire lock to safely update reader count
            unique_lock<mutex> lock(readMutex);

            // First reader acquires write mutex to block writers
            if (++rc == 1) {
                writeMutex.lock();
            }

            // Release the lock for others to update the reader count
            lock.unlock();

            // Simulate reading process
            cout << "Reader " << id << " is reading." << endl;
            this_thread::sleep_for(chrono::milliseconds(1500)); // Simulated reading time
            cout << "Reader " << id << " finished reading." << endl;

            // Acquire lock to safely update reader count after reading
            lock.lock();

            // Last reader releases the write mutex, allowing writers to proceed
            if (--rc == 0) {
                writeMutex.unlock();
            }

        } catch (const exception& e) {
            cout << e.what() << endl;
        }
    }

    // Writer function representing writer process
    void Writer(int id) {
        try {
            // Acquire write mutex for exclusive write access
            unique_lock<mutex> lock(writeMutex);

            // Simulate writing process
            cout << "Writer " << id << " is writing." << endl;
            this_thread::sleep_for(chrono::milliseconds(1000)); // Simulated writing time
            cout << "Writer " << id << " finished writing." << endl;

        } catch (const exception& e) {
            cout << e.what() << endl;
        }
    }

    // Main function for input and process management
    static void run() {
        RW rw;
        int n;

        // Input number of processes
        cout << "Enter the number of processes: ";
        cin >> n;

        // Input sequence of processes (1 for Reader, 2 for Writer)
        vector<int> seq(n);
        cout << "Enter the sequence of processes (1 for Reader, 2 for Writer):" << endl;
        for (int i = 0; i < n; i++) {
            cin >> seq[i];
        }

        // List to keep track of all threads
        vector<thread> threads;

        // Create and start each thread based on process type
        for (int i = 0; i < n; i++) {
            int id = i + 1;
            if (seq[i] == 1) {
                // Create reader thread
                threads.push_back(thread(&RW::Reader, &rw, id));
            } else {
                // Create writer thread
                threads.push_back(thread(&RW::Writer, &rw, id));
            }
        }

        // Wait for all threads to finish before program ends
        for (auto& t : threads) {
            t.join();
        }
    }
};

// Initialize static reader count
int RW::rc = 0;

int main() {
    RW::run();
    return 0;
}

