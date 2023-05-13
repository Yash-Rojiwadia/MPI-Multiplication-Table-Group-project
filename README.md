# MPI Programming for Multiplication Tables
### CP431 – Parallel Programming 
## Group Members :
Yash Rojiwadia | 203039360 | roji9360@mylaurier.ca

Heba Adi | 201653820 | adix3820@mylaurier.ca

Maham Farooq | 200498180 | faro8180@mylaurier.ca

Majed Hadida | 200436110 | hadi6110@mylaurier.ca

Pranav Gangwani | 193097120 | gang7120@mylaurier.ca

## Introduction
This program is designed to find the total number of unique elements in the multiplication table of a given number "n" using parallel computing. It uses the MPI library for inter-process communication.

## Prerequisites
To run this program, you need to have the MPI library installed on your system.

## Running the program
To run the program, you need to execute the following command in your terminal:

`mpiexec -n <number of processes> ./main <n>`

Here, `<number of processes>` is the number of processes you want to use for parallel computing, and `<n>` is the number for which you want to find the multiplication table.

For example, if you want to find the multiplication table of 10 using 4 processes, you should execute the following command:

`mpiexec -n 4 ./main 10`

## Output
The program outputs the following information:

- Number of cores used for parallel computing
- The input number for which the multiplication table is computed
- The total number of unique elements in the multiplication table
- Time elapsed in seconds for the computation
