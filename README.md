
This program demonstrates a solution to the classical "readers-writers problem" using mutexes and semaphores. It is a multi-threaded program that simulates the interaction between readers and writers accessing a shared resource, represented here as a counter.

The program includes the following libraries: `pthread.h`, `semaphore.h`, `stdio.h`, `ncurses.h`, and `stdlib.h`. **Program is written for Linux OS** 

The main components of the program are:

    
1.  Writer thread function (`writer`): This function represents the behavior of a writer thread. It uses a semaphore (`wrt`) to ensure exclusive access to the shared resource (counter). The writer multiplies the counter by 2 and displays the updated value on the console using `ncurses` library functions.
    
2.  Reader thread function (`reader`): This function represents the behavior of a reader thread. It uses a mutex (`mutex`) to ensure mutual exclusion when updating the `numerCzytelnika` variable. The first reader locks the semaphore (`wrt`) to block writers. After reading the counter, the last reader releases the semaphore to allow writers to proceed.
    
3.  `main` function: The main function initializes `ncurses` library, creates reader and writer threads, waits for their completion using `pthread_join`, and performs necessary clean-up operations. At the end, it displays a message and waits for user input before terminating.
    

The program demonstrates synchronization and coordination between multiple readers and writers to ensure data consistency and prevent conflicts. 
