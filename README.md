<h1 align="center">Multithreaded Sorting Algorithm</h1>

<p align="center">
  <img src=/msort.jpeg>
</p>

This project is based on Project 2 of Chapter 4 of the book "Operating Systems - 9th Edition". The aim of the project is to sort an array of integer (size n) using multithreading. The pthreads library and a Linux VM will be used in the development of this project.
- To set up the software for this project use the following resource [http://cs.westminstercollege.edu/~greg/osc10e/vm/index.html](http://cs.westminstercollege.edu/~greg/osc10e/vm/index.html)
- VirtualBox, WinSCP, and Ubuntu 18.04.5 were used in this project

<br>
**Goals**
The following assumptions will be made for the development of this project:
- Populate the array of integers with random numbers - _Main thread should accomplish this_
- Use the standard "sort" in UNIX to sort the segment of the array assigned to each thread in the first step or sort function available
- Develop a merge function that will merge two sorted segments of the array
- After sorting is complete, the main thread writes the numbers in an increasing order to an output file that is specified in the input arguement
- n is the size of the array and it will be a power of 2
- m is the number of threads that will be used and will also be a power of 2 where m < n
- Parameters n, m, and the output filename are all specified from the argument list (e.g. mysort n m out.txt)

**Part 1**
For this part of the projecte a program with M threads to sort and merge segments of the array in a reverse tree structure, for example:
- Level 0: All threads sort their segment of the array (n/m)
- Level 1: The even threads merge their segment with their odd successor threads (i.e., 0 & 1, 2 & 3)
- Level 2: Thread 0 merges its segment from the previous step with that of thread 2. _Sorting should be complete at this step_

**Part 2**
The second leg of this project development of a global synchronization mechanism (called barriers) will need to be developed. When the threads call the barrier, they will not be able to proceed until all threads reach the barrier
- A barrier function will be developed using semaphores
- Use this function between the levels described above to implement the sorting algorithm. _Pthread barrier functionality will not be used_
