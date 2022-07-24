# Needleman-Wunsch Algorithm

Team Members:

1. <a href="https://github.com/coniferousdyer">Arjun Muraleedharan</a>
2. <a href="https://github.com/keyurchd11">Keyur Chaudhari</a>

# Introduction

The Needleman-Wunsch algorithm is an algorithm that is used in bioinformatics to align DNA or protein sequences. It involves the use of dynamic programming to compare biological sequences. The Needleman-Wunsch algorithm is used to find the optimal global alignment, and essentially divides a larger problem into a series of smaller problems to find this optimal alignment.

## Task

We attempt to make improvements upon the baseline performance of the algorithm by parallelizing it and optimizing it to achieve the peak performance possible.

## Performance Evaluation Framework

The main performance metrics were ``Execution Time`` and ``Throughput``.

## How to Run

The desired version can be compiled using the given Makefile as follows.
```bash
make <VERSION>
```

`VERSION` can be any of the following:

1. Brute force: `brute_nw`
2. Cache locality: `optimized_nw_1`
3. Compiler optimizations: `optimized_nw_2`
4. Anti-diagonal parallelization: `optimized_nw_3`
5. Submatrix Tiling: `optimized_nw_4`

To then run the generated executable, run
```bash
./<VERSION>
```

This then generates a text file containing the output of the form
```bash
A1 B1
A2 B2
...
...
...
```
where A1, A2, etc. are the execution times for particular lengths of the strings and B1, B2, etc. are the corresponding lengths of the strings.

A plot of execution time vs length of the string can be generated for all the output text files by running
```bash
python3 plot.py
```

## Brute Force

The code for the brute force, unoptimized version of the Needleman-Wunsch algorithm is present in `brute_nw.cpp`.

## Optimization Techniques Used

The following are the approaches used to optimize the algorithm. For a detailed report of how the algorithms work, the compute configuration of the system on which the optimizations were done and the results obtained, refer to `report.pdf`.

### 1. Exploiting Cache Locality

Accessing data row-wise gave better results than accessing colum-wise. This difference is intuitive since 2D matrices are stored in row-major form. The code is present in `optimized_nw_1.cpp`.

### 2. Compiler Optimizations

We tested the code by adding various compiler flags and on various compilers (both ICC and GCC). The code is present in `optimized_nw_2.cpp`.

### 3. Parallelization along the Anti-Diagonal

By analyzing the dependency graph in states of the dynamic programming problem, we concluded that traversing along anti-diagonals would reduce the number of dependencies at each step, thus enabling higher parallelization. The code is present in `optimized_nw_3.cpp`.

### 4. Tiling using Submatrices

Although the anti-diagonal optimization allows higher parallelism, it doesn't let us exploit cache locality. Thus, we used the ``Tiling Technique``. iling is a technique wherein the original DP matrix is divided into square submatrices. We then fill each submatrix with the corresponding values just like we do for the normal matrix, using row traversal. We proceed in this manner, filling all the submatrices and filling the leftover cells using brute force, iterating through all of them and filling them based on the values of the previous cells. It allows us to make use of both parallelism, and cache locality. The code is present in `optimized_nw_4.cpp`.