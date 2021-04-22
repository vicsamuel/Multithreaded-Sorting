<h1 align="center">Multithreaded Sorting with Multithreading</h1>

<p align="center">
  <img src=/msort.PNG>
</p>

This project is based on Project 2 of Chapter 4 of the book "Operating Systems - 9th Edition". The aim of the project is to sort an array of integer (size n) using multithreading. The pthreads library and a Linux VM will be used in the development of this project.
- To set up the software for this project use the following resource [http://cs.westminstercollege.edu/~greg/osc10e/vm/index.html](http://cs.westminstercollege.edu/~greg/osc10e/vm/index.html)
- VirtualBox [https://www.virtualbox.org/](https://www.virtualbox.org/)
- WinSCP [https://winscp.net/eng/index.php](https://winscp.net/eng/index.php)
- Ubuntu 18.04.5 [https://releases.ubuntu.com/18.04/](https://releases.ubuntu.com/18.04/) (or downloadable from the Windows Store)

**Goals**
<br>The following assumptions were made for the development of this project:
- Populates the array of integers with random numbers (main thread should accomplish this)
- Uses the standard "sort" in UNIX to sort the segment of the array assigned to each thread in the first step or sort function available
- Developed a merge function that will merge two sorted segments of the array
- After sorting is complete, the main thread writes the numbers in an increasing order to an output file that is specified in the input arguement
- n is the size of the array and it will be a power of 2
- m is the number of threads that will be used and will also be a power of 2 where m < n
- Parameters n, m, and the output filename are all specified from the argument list (e.g. mysort n m out.txt)

**Part 1**
<br>For this part of the projecte a program with M threads to sort and merge segments of the array in a reverse tree structure, for example: </p>
- Level 0: All threads sort their segment of the array (n/m)</br>
- Level 1: The even threads merge their segment with their odd successor threads (i.e., 0 & 1, 2 & 3)
- Level 2: Thread 0 merges its segment from the previous step with that of thread 2. _Sorting should be complete at this step_

**Part 2**
<br>The second leg of this project development of a global synchronization mechanism (called barrier) were developed. When the threads call the barrier, they can not proceed until all threads reach the barrier</br>
- A barrier function was developed using semaphores
- This function was used between the levels described above to implement the sorting algorithm. _Pthread library barrier function was not used as our own method was developed using semaphores_
