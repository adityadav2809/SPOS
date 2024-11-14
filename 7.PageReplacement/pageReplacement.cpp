#include <bits/stdc++.h>

using namespace std;

// FIFO Page Replacement Algorithm
int fifoPageReplacement(vector<int>& pages, int frames) {
    queue<int> q; // Queue to maintain the order of pages in memory
    unordered_set<int> s; // Set to keep track of pages currently in frames
    int pageFaults = 0; // Counter for page faults

    // Iterate through the page requests
    for (int page : pages) {
        // If there is room in the frames
        if (s.size() < frames) {
            // Check if page is not already in memory
            if (s.find(page) == s.end()) {
                s.insert(page); // Insert page into memory
                q.push(page); // Add page to queue
                pageFaults++; // Increment page fault count
            }
        } else {
            // If the page is not in memory and frames are full
            if (s.find(page) == s.end()) {
                int removedPage = q.front(); // Remove the oldest page from queue
                q.pop(); // Remove from queue
                s.erase(removedPage); // Remove from set
                s.insert(page); // Insert the new page
                q.push(page); // Add the new page to queue
                pageFaults++; // Increment page fault count
            }
        }
    }
    return pageFaults;
}

// LRU Page Replacement Algorithm
int lruPageReplacement(vector<int>& pages, int frames) {
    unordered_map<int, int> pageMap; // Map to store pages and their last used indexes
    int pageFaults = 0; // Counter for page faults

    // Iterate through each page request
    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];

        // If there is room in frames
        if (pageMap.size() < frames) {
            // If the page is not in memory, increase page fault count
            if (pageMap.find(page) == pageMap.end()) {
                pageFaults++;
            }
            pageMap[page] = i; // Update the last used index of page
        } else {
            // If the page is not in memory and frames are full
            if (pageMap.find(page) == pageMap.end()) {
                int lruPage = -1, lruIndex = i;
                // Find the least recently used page
                for (auto it : pageMap) {
                    if (it.second < lruIndex) {
                        lruIndex = it.second;
                        lruPage = it.first;
                    }
                }
                pageMap.erase(lruPage); // Remove LRU page
                pageMap[page] = i; // Insert the new page and update index
                pageFaults++; // Increment page fault count
            } else {
                pageMap[page] = i; // Update the last used index of page
            }
        }
    }
    return pageFaults;
}

// Optimal Page Replacement Algorithm
int optimalPageReplacement(vector<int>& pages, int frames) {
    unordered_set<int> s; // Set to keep track of pages currently in frames
    int pageFaults = 0; // Counter for page faults

    // Iterate through each page request
    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];

        // If there is room in frames
        if (s.size() < frames) {
            if (s.find(page) == s.end()) { // If the page is not in memory
                s.insert(page); // Insert page into memory
                pageFaults++; // Increment page fault count
            }
        } else {
            // If the page is not in memory and frames are full
            if (s.find(page) == s.end()) {
                int farthest = i, victimPage = -1;
                // Find the page to be replaced based on future use
                for (int pg : s) {
                    int nextUse = find(pages.begin() + i + 1, pages.end(), pg) - pages.begin();
                    if (nextUse > farthest) {
                        farthest = nextUse;
                        victimPage = pg;
                    }
                }
                s.erase(victimPage); // Remove the page to be replaced
                s.insert(page); // Insert the new page
                pageFaults++; // Increment page fault count
            }
        }
    }
    return pageFaults;
}

int main() {
    // Sample pages and frame count
    vector<int> pages = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};
    int frames = 3;

    // Display results for each page replacement algorithm
    cout << "FIFO Page Faults: " << fifoPageReplacement(pages, frames) << endl;
    cout << "LRU Page Faults: " << lruPageReplacement(pages, frames) << endl;
    cout << "Optimal Page Faults: " << optimalPageReplacement(pages, frames) << endl;

    return 0;
}












//Introduction to Page Replacement Algorithms
//Page replacement algorithms are used in operating systems to manage the contents of physical memory when it is fully occupied. When a new page (chunk of data) needs to be loaded into memory but all frames (slots in memory) are occupied, the system must decide which page to remove to make space for the new one. The goal is to minimize page faults, which occur when a program tries to access data that isn’t currently in memory, requiring the data to be fetched from disk (a time-consuming operation).
//
//1. FIFO (First-In-First-Out) Page Replacement Algorithm
//Concept: The FIFO algorithm removes the page that has been in memory the longest. This is similar to a queue, where pages are added at the end and removed from the front.
//Working: When a page needs to be loaded, if there is a free frame, it is loaded directly. If there are no free frames, the oldest page in memory is removed to make room for the new page.
//Pros: Simple to implement since it only requires tracking the oldest page.
//Cons: May not perform well in terms of minimizing page faults, as it does not consider which pages are likely to be used again soon.
//Example:
//Suppose the page reference string is [7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2] and there are 3 frames.
//As each page is accessed, the FIFO algorithm evicts the oldest page in memory once the frames are full, which may not always be optimal.
//2. LRU (Least Recently Used) Page Replacement Algorithm
//Concept: LRU removes the page that has not been used for the longest period of time, based on the assumption that pages used recently are likely to be used again soon.
//Working: For each page access, LRU checks if the page is already in memory. If the page is not in memory and all frames are occupied, the least recently used page is removed to make space for the new page.
//Pros: Generally provides better performance than FIFO by making more informed decisions about which page to remove.
//Cons: More complex to implement than FIFO, as it requires keeping track of the order in which pages are accessed.
//Example:
//With the same page reference string [7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2] and 3 frames, LRU will remove the page that was least recently used whenever a new page needs to be loaded.
//This requires updating the last-used timestamp for each page or storing page accesses in a way that allows quick identification of the least recently used page.
//3. Optimal Page Replacement Algorithm
//Concept: The Optimal algorithm replaces the page that will not be used for the longest period in the future. It makes use of future knowledge of page references, which makes it the most efficient of the three.
//Working: For each page access, if the page is not in memory and frames are full, the page that won’t be needed for the longest time in the future is removed.
//Pros: Results in the fewest possible page faults, making it theoretically the best algorithm.
//Cons: Cannot be implemented in practice, as it requires future knowledge of page references. Often used as a benchmark to compare other algorithms.
//Example:
//For the reference string [7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2] with 3 frames, Optimal calculates which page will not be used for the longest time and evicts it. This guarantees the least number of page faults, but it is only feasible to simulate rather than implement.
//Comparing the Algorithms
//Page Faults:
//
//FIFO: Tends to have more page faults than Optimal and can perform poorly due to not considering future use.
//LRU: Usually results in fewer page faults than FIFO as it uses past access history to make decisions.
//Optimal: Has the least page faults since it makes perfect replacement choices based on future page requests.
//Complexity:
//
//FIFO: Simple to implement; it only needs a queue to track the order of page arrivals.
//LRU: More complex due to the need to track recent access history, often implemented with additional data structures (e.g., counters or lists).
//Optimal: Computationally intensive in simulation due to the need to predict future page usage.
//Practicality:
//
//FIFO and LRU: Commonly implemented in real systems, with LRU often preferred due to its lower page fault rate.
//Optimal: Theoretically optimal but impractical, as it requires knowledge of future references.
//Use Cases
//FIFO is often used in simpler systems where minimal overhead is required.
//LRU is used in operating systems and applications that can afford slightly higher overhead to minimize page faults.
//Optimal serves as an ideal benchmark but is typically only used in theoretical analysis.
//Key Points to Remember
//FIFO focuses on the oldest page, LRU on the least recently used page, and Optimal on future use.
//LRU generally performs better than FIFO but is more complex.
//Optimal is the best in terms of page faults but is not practically implementable.
//Each of these algorithms has trade-offs, and understanding their characteristics can help in choosing the right one based on system requirements and resource availability.
