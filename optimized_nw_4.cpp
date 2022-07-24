#pragma GCC optimize("O3")
#pragma GCC optimize("Ofast")

#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "helper.hpp"

using namespace std;

double needleman_wunsch(string &A, string &B, int match_penalty, int mismatch_penalty, int gap_penalty, int tileSize)
{
    int sequence1_size = A.size(), sequence2_size = B.size();
    if (sequence1_size > sequence2_size)
    {
        swap(sequence1_size, sequence2_size);
        swap(A, B);
    }
    // the size of matrix is sequence1_size + 1 rows x sequence2_size + 1 columns
    // Initialize the 2D matrix
    int **dp = new int *[sequence1_size + 1];
    for (int i = 0; i <= sequence1_size; i++)
        dp[i] = new int[sequence2_size + 1];

    double gflops = 6 * sequence1_size * sequence2_size;

    struct timeval t;
    tick(&t);

    // Fill the first row and column with gap penalties
#pragma omp parallel for
    for (int i = 0; i <= sequence1_size; i++)
        dp[i][0] = i * gap_penalty;

#pragma omp parallel for
    for (int i = 0; i <= sequence2_size; i++)
        dp[0][i] = i * gap_penalty;

    // Fill the rest of the matrix
    // start with first tile
    int horTiles = sequence1_size / tileSize;
    int verTiles = sequence2_size / tileSize;

    // the square will have min(horTiles, verTiles) tiles

    for (int DiagNum = 0; DiagNum < min(horTiles, verTiles); DiagNum++)
    {
        // i always start at 1st row
        int startI = 1;
        int startJ = 1 + DiagNum * tileSize;
        for (int tile = 0; tile <= DiagNum; tile++)
        {
            for (int i = startI; i < startI + tileSize; i++)
            {
                for (int j = startJ; j < startJ + tileSize; j++)
                {
                    int score = A[i - 1] == B[j - 1] ? match_penalty : mismatch_penalty;
                    dp[i][j] = max(dp[i - 1][j - 1] + score, max(dp[i - 1][j] + gap_penalty, dp[i][j - 1] + gap_penalty));
                }
            }
            startI += tileSize;
            startJ -= tileSize;
        }
    }
    for (int diagNum = min(horTiles, verTiles) - 1; diagNum > 0; diagNum--)
    {
        int startI = 1 + (min(horTiles, verTiles) - diagNum) * tileSize;
        int startJ = 1 + (min(horTiles, verTiles) - 1) * tileSize;
        for (int tile = 0; tile < diagNum; tile++)
        {
            for (int i = startI; i < startI + tileSize; i++)
            {
                for (int j = startJ; j < startJ + tileSize; j++)
                {
                    int score = A[i - 1] == B[j - 1] ? match_penalty : mismatch_penalty;
                    dp[i][j] = max(dp[i - 1][j - 1] + score, max(dp[i - 1][j] + gap_penalty, dp[i][j - 1] + gap_penalty));
                }
            }
            startI += tileSize;
            startJ -= tileSize;
        }
    }
    int totDiagonals = min(horTiles, verTiles);
    if (sequence1_size > totDiagonals * tileSize)
    {
        for (int i = 1 + tileSize * totDiagonals; i <= sequence1_size; i++)
        {
            for (int j = 1; j <= 1 + totDiagonals * tileSize; j++)
            {
                int score = A[i - 1] == B[j - 1] ? match_penalty : mismatch_penalty;
                dp[i][j] = max(dp[i - 1][j - 1] + score, max(dp[i - 1][j] + gap_penalty, dp[i][j - 1] + gap_penalty));
            }
        }
    }

    if (sequence2_size > totDiagonals * tileSize)
    {
        for (int i = 1; i <= sequence1_size; i++)
        {
            for (int j = 1 + tileSize * totDiagonals; j <= sequence2_size; j++)
            {
                int score = A[i - 1] == B[j - 1] ? match_penalty : mismatch_penalty;
                dp[i][j] = max(dp[i - 1][j - 1] + score, max(dp[i - 1][j] + gap_penalty, dp[i][j - 1] + gap_penalty));
            }
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
    ofstream fout("optimized_4_output.txt");

    for (int i = 1; i <= 1000; i++)
    {
        string A = generate_random_string(i), B = generate_random_string(i);

        int match_penalty = 1, mismatch_penalty = -1, gap_penalty = -2;

        double time = needleman_wunsch(A, B, match_penalty, mismatch_penalty, gap_penalty, 4);

        fout << time << " " << i << " "
             << "\n";
    }

    fout.close();
}