#pragma GCC optimize("O3")
#pragma GCC optimize("Ofast")

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <omp.h>

#include "helper.hpp"

using namespace std;

double needleman_wunsch(string &A, string &B, int match_penalty, int mismatch_penalty, int gap_penalty)
{
    int sequence1_size = A.size(), sequence2_size = B.size();

    // Initialize the 2D matrix
    int **dp = new int *[sequence1_size + 1];
    for (int i = 0; i <= sequence1_size; i++)
        dp[i] = new int[sequence2_size + 1];

    double gflops = 6 * sequence1_size * sequence2_size;

    struct timeval t;
    tick(&t);

    if (sequence1_size > sequence2_size)
    {
        swap(sequence1_size, sequence2_size);
        swap(A, B);
    }

    // Fill the first row and column with gap penalties
#pragma omp parallel for
    for (int i = 0; i <= sequence1_size; i++)
        dp[i][0] = i * gap_penalty;

#pragma omp parallel for
    for (int i = 0; i <= sequence2_size; i++)
        dp[0][i] = i * gap_penalty;

    for (int i = 1; i <= sequence2_size; i++)
    {
        int num_iterations = min(i, sequence1_size + 1);

#pragma omp parallel for
        for (int j = 0; j < num_iterations; j++)
        {
            if (j == 0 || i == j)
                continue;

            char sequence_at_iA = A[j - 1];
            char sequence_at_jB = B[i - j - 1];
            int diagonal_value = dp[j - 1][i - j - 1] + (sequence_at_iA == sequence_at_jB ? match_penalty : mismatch_penalty);
            int gap_A = dp[j - 1][i - j] + gap_penalty;
            int gap_B = dp[j][i - j - 1] + gap_penalty;
            dp[j][i - j] = max(max(gap_A, gap_B), diagonal_value);
        }
    }

    for (int i = 1; i <= sequence1_size; i++)
    {
        int num_iterations = sequence1_size + 1 - i;

#pragma omp parallel for
        for (int j = 0; j < num_iterations; j++)
        {
            if (i == -j || sequence2_size == j)
                continue;

            char sequence_at_iA = A[i + j - 1];
            char sequence_at_jB = B[sequence2_size - j - 1];
            int diagonal_value = dp[i + j - 1][sequence2_size - j - 1] + (sequence_at_iA == sequence_at_jB ? match_penalty : mismatch_penalty);
            int gap_A = dp[i + j - 1][sequence2_size - j] + gap_penalty;
            int gap_B = dp[i + j][sequence2_size - j - 1] + gap_penalty;
            dp[i + j][sequence2_size - j] = max(max(gap_A, gap_B), diagonal_value);
        }
    }

    double time = tock(&t);

    // cout << "Execution Time: " << time * 1000 << "\n";
    // cout << "Operational Intensity: " << (double)(gflops) / (4 * sequence1_size * sequence2_size) << "\n";
    // cout << "GIPS: " << gflops / (time * 1e9) << "\n";
    return gflops / (time * 1e9);
}

int main()
{
    ofstream fout("optimized_3_output.txt");

    for (int i = 1; i <= 1000; i++)
    {
        string A = generate_random_string(i), B = generate_random_string(i);

        int match_penalty = 1, mismatch_penalty = -1, gap_penalty = -2;

        double time = needleman_wunsch(A, B, match_penalty, mismatch_penalty, gap_penalty);

        fout << time << " " << i << " " << "\n";
    }

    fout.close();
}