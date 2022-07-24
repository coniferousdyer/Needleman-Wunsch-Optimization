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