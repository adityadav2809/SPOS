import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class RW {
    // Lock for readers to manage mutual exclusion for reader count
    private final Lock readLock = new ReentrantLock(); 

    // Semaphore for writer to ensure exclusive access
    private final Semaphore writeLock = new Semaphore(1); 

    // Shared reader count to track active readers
    private static int rc = 0;

    // Reader function representing reader process
    private void Reader(int id) {
        try {
            // Acquire lock to safely update reader count
            readLock.lock(); 
            
            // First reader acquires write lock to block writers
            if (++rc == 1) { 
                writeLock.acquire();
            }

            // Release read lock as reader count update is done
            readLock.unlock(); 

            // Simulate reading process
            System.out.println("Reader " + id + " is reading.");
            Thread.sleep(1500); // Simulated reading time
            System.out.println("Reader " + id + " finished reading.");

            // Re-acquire lock to safely update reader count after reading
            readLock.lock(); 

            // Last reader releases the write lock, allowing writers to proceed
            if (--rc == 0) { 
                writeLock.release();
            }

            // Release read lock
            readLock.unlock(); 

        } catch (InterruptedException e) {
            System.out.println(e.getMessage());
        }
    }

    // Writer function representing writer process
    private void Writer(int id) {
        try {
            // Acquire write lock for exclusive write access
            writeLock.acquire(); 

            // Simulate writing process
            System.out.println("Writer " + id + " is writing.");
            Thread.sleep(1000); // Simulated writing time
            System.out.println("Writer " + id + " finished writing.");

            // Release write lock after writing is done
            writeLock.release(); 

        } catch (InterruptedException e) {
            System.out.println(e.getMessage());
        }
    }

    // Main function for input and process management
    public static void main(String[] args) {
        RW rw = new RW();
        Scanner sc = new Scanner(System.in);

        // Input number of processes
        System.out.print("Enter the number of processes: ");
        int n = sc.nextInt();

        // Input sequence of processes (1 for Reader, 2 for Writer)
        int[] seq = new int[n];
        System.out.println("Enter the sequence of processes (1 for Reader, 2 for Writer):");
        for (int i = 0; i < n; i++) {
            seq[i] = sc.nextInt();
        }

        // List to keep track of all threads
        List<Thread> threads = new ArrayList<>();
        
        // Create and start each thread based on process type
        for (int i = 0; i < n; i++) {
            int id = i + 1;
            Thread t = (seq[i] == 1) 
                    ? new Thread(() -> rw.Reader(id))  // Create reader thread
                    : new Thread(() -> rw.Writer(id)); // Create writer thread
            t.start();  // Start the thread
            threads.add(t); // Add to the list of threads
        }

        // Wait for all threads to finish before program ends
        for (Thread t : threads) {
            try {
                t.join();
            } catch (InterruptedException e) {
                System.out.println(e.getMessage());
            }
        }

        sc.close(); // Close the scanner to release resources
    }
}
/*
The **Readers-Writers Problem** is a classic synchronization problem in computer science that illustrates issues of concurrent resource access in a multi-threaded environment. This problem is commonly encountered when multiple processes or threads need to read from and write to a shared resource (e.g., a file or database). The solution aims to ensure **data consistency** and **prevent conflicts** between reading and writing operations.

### Problem Explanation

In this problem:
- **Readers** are threads that read data from a shared resource but do not modify it.
- **Writers** are threads that modify the shared resource, potentially changing the data that readers access.

The goal is to design a system where:
1. Multiple readers can read the resource simultaneously (since reading does not modify data).
2. Writers have exclusive access when writing, meaning no other readers or writers can access the resource during a write operation.

This code implements the **Readers-Writers solution** using **locks and semaphores** to coordinate the access between readers and writers.

### Key Synchronization Components

1. **Read Lock (`readLock`)**:
   - This lock ensures mutual exclusion when modifying the **reader count** (`rc`), which keeps track of the number of active readers.
   - It prevents race conditions when multiple readers access or update the `rc` variable simultaneously.

2. **Write Lock (`writeLock`)**:
   - This semaphore allows only one writer to access the shared resource at any time.
   - When a writer holds this lock, all readers and other writers are blocked until the writer completes its operation.

3. **Reader Count (`rc`)**:
   - The `rc` variable keeps track of the number of active readers.
   - The first reader to start reading acquires the `writeLock`, blocking writers until there are no more active readers.
   - The last reader to finish releases the `writeLock`, allowing writers to proceed.

### Implementation Details

The implementation consists of three primary methods:
- `Reader(int id)`: Simulates a reader accessing the resource.
- `Writer(int id)`: Simulates a writer modifying the resource.
- `main()`: Manages process input, sequence setup, and thread management.

### Detailed Workflow

#### Reader Function (`Reader(int id)`)

1. **Entering the Critical Section**:
   - When a reader thread starts, it first acquires the `readLock`. This is a mutual exclusion lock that ensures the safe modification of `rc` (the reader count).
   - **Increment Reader Count (`rc`)**: The reader increments `rc` by 1 to indicate an active reader. If `rc` becomes 1 (i.e., this is the first active reader), it acquires the `writeLock`. This prevents writers from modifying the resource while readers are active.

2. **Simulating Reading**:
   - The reader releases the `readLock`, allowing other readers to increment the count if necessary.
   - The reader thread simulates a reading operation by printing a message and waiting (`Thread.sleep`) to represent the reading process.

3. **Exiting the Critical Section**:
   - After finishing reading, the reader acquires the `readLock` again to update `rc` safely.
   - **Decrement Reader Count**: The reader decrements `rc` to signal it has finished reading. If `rc` becomes 0 (i.e., no more active readers), it releases the `writeLock`, allowing writers to proceed.
   - The reader then releases the `readLock`.

#### Writer Function (`Writer(int id)`)

1. **Exclusive Access with Write Lock**:
   - A writer thread must acquire the `writeLock` before starting, ensuring exclusive access to the shared resource.
   - As `writeLock` is a semaphore initialized to 1, only one writer can hold it at a time, blocking other readers and writers.

2. **Simulating Writing**:
   - After acquiring the lock, the writer simulates the writing operation by printing a message and waiting (`Thread.sleep`) to represent the time taken to write.

3. **Releasing the Write Lock**:
   - After writing, the writer releases the `writeLock`, allowing other readers or writers to access the resource.

### Main Method

1. **Input and Setup**:
   - The main method gathers user input for the number of processes and their types (reader or writer) using a scanner.
   - It sets up the sequence of processes based on user input.

2. **Creating and Starting Threads**:
   - For each process in the input sequence, the main method creates a `Thread` object corresponding to either a reader or writer. Each thread is started immediately and stored in a list to track its progress.

3. **Joining Threads**:
   - To ensure the main program waits for all reader and writer threads to complete, each thread is joined. This prevents the program from exiting prematurely.

### Example Execution

**Example Input**:
```
Number of processes: 5
Sequence of processes (1 for Reader, 2 for Writer): 1 1 2 1 2
```

**Example Output**:
```
Reader 1 is reading.
Reader 1 finished reading.
Reader 2 is reading.
Reader 2 finished reading.
Writer 1 is writing.
Writer 1 finished writing.
Reader 3 is reading.
Reader 3 finished reading.
Writer 2 is writing.
Writer 2 finished writing.
```

### Key Points on Synchronization

- **Readers First**: In this code, readers are given priority over writers. As long as there is at least one active reader, writers are blocked. This is handled by the `writeLock` semaphore, which is acquired by the first reader and released by the last reader.
  
- **Data Consistency**: By ensuring that only one writer can access the resource at a time, and that all readers complete their operations before a writer can proceed, this approach maintains data consistency.

- **Fairness and Deadlock Prevention**: Since `readLock` and `writeLock` are independent, the code avoids deadlock and ensures fair access, but it does not guarantee that readers or writers are prioritized in any particular order beyond the rule that all readers must complete before any writer proceeds.

### Conclusion

This implementation of the Readers-Writers problem provides a safe and effective way to handle concurrent read/write operations on a shared resource, ensuring data consistency and preventing race conditions. Using semaphores and locks:
- Multiple readers can access the resource simultaneously without conflicts.
- Writers gain exclusive access to prevent data inconsistency during modifications.

This solution is efficient for scenarios where reading is more frequent than writing, such as database query systems or file-reading applications where data is rarely modified but frequently accessed.


### Real-World Example: Library Database System

Let's apply the **Readers-Writers problem** to a **library management system**. In this case, the shared resource is the **library's database**.

#### Scenario:
- **Readers**: People who want to **view books** (search, check availability, read book details, etc.).
- **Writers**: Library staff who want to **update the database** (add new books, update book details, remove books, etc.).

In this scenario:
1. **Multiple readers (users)** can simultaneously view the books in the library without any issues because reading doesn't alter the data.
2. **Only one writer** (library staff) should be able to modify the database at a time. While the database is being updated (e.g., adding a new book), no other writer or reader should access the database simultaneously to maintain data integrity.

### Example of the Application:

#### Library Database System

- The library database holds information about all available books. Each book has details like title, author, year of publication, genre, and availability (whether the book is checked out or available).
- **Readers** are patrons of the library who only view the available books or search for a particular book.
- **Writers** are the library staff who can update the database by adding new books, deleting old ones, or modifying book details (such as availability when a book is checked in or out).

### Use Case:

#### Case 1: **Multiple Readers** (Library Patrons)

- Multiple library patrons (readers) can simultaneously search for books in the library's online catalog. Each patron is merely reading the database to check if a book is available or to find a book they want to borrow.
- The library's database can handle many patrons accessing the system at the same time. As long as no one is modifying the database, multiple users can read and search the information without conflict.

#### Case 2: **Single Writer** (Library Staff)

- Suppose the library staff needs to **add new books** to the catalog, **update a book's availability**, or **remove outdated books** from the database.
- The writer (library staff member) needs **exclusive access** to the database. No readers should be allowed to access the database while a writer is making updates to prevent inconsistencies. For example, if a patron tries to check out a book while it's being removed from the database, they might encounter an incorrect status.

#### Solution Using the Readers-Writers Problem

- **Read Locks**: Readers (patrons) can access the database at the same time, so long as there are no writers. Each reader locks the `readLock`, and if it's the first reader, they block writers by acquiring the `writeLock`. All other readers can continue to read, but no writers can modify the database while they do.
  
- **Write Locks**: Writers (staff) need exclusive access to the database. If a writer wants to modify the database, they need to acquire the `writeLock`, which blocks both other writers and readers until the writer finishes the task.

### Application in Code (Library Management Example):

In our case, the **Reader** thread would represent a patron searching for or viewing books, and the **Writer** thread would represent a library staff member adding a new book or updating a book’s status.

#### Detailed Explanation of the Code with Real-World Context:

1. **Reader** (`Reader(int id)`):
   - This thread simulates a patron searching the library's catalog. When a reader starts, it checks whether it's the first reader to access the library's database. If it is, it blocks any writers from accessing the database. Multiple readers can work in parallel without any issues.
   - Example: A patron searches for "Harry Potter" or views the availability of books online.

2. **Writer** (`Writer(int id)`):
   - This thread simulates a library staff member adding, updating, or deleting books from the library’s catalog. It ensures that no other readers or writers can modify the database while a writer is working on it.
   - Example: A librarian adds a new book, updates the status of a book (checked out/available), or removes outdated books from the system.

### Practical Example of Code Execution:

#### Example Input:
```
Enter the number of processes: 5
Enter the sequence of processes (1 for Reader, 2 for Writer):
1 2 1 2 1
```


### Real-World Behavior:
- **Step 1**: Two patrons (Readers 1 and 2) access the library database simultaneously to search for books, without interfering with each other.
- **Step 2**: After both readers finish, a librarian (Writer 1) comes in to update the catalog (e.g., add new books, update availability), blocking any further reads or writes until done.
- **Step 3**: After Writer 1 finishes, Reader 3 is allowed to access the catalog.
- **Step 4**: Finally, Writer 2 updates the catalog again (e.g., remove outdated books), again blocking any further readers until the update is finished.

### Key Takeaways:

- **Readers** can access the database simultaneously, as they do not modify it.
- **Writers** require exclusive access to modify the database, and no other writers or readers are allowed to access the resource during this time.
- The system ensures that **readers don't block other readers** but also ensures that **writers have exclusive access** when making changes.
- The **use of locks** ensures that data consistency is maintained, and no conflicts occur between read and write operations.

This problem and its solution are crucial in systems where multiple threads or users access shared resources, such as databases, file systems, and any system that requires the safe reading and writing of data in a concurrent environment.
*/