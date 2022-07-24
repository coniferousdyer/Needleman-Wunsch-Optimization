#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "helper.hpp"

using namespace std;

double needleman_wunsch(string &A, string &B, int match_penalty, int mismatch_penalty, int gap_penalty)
{
    int sequence1_size = A.size(), sequence2_size = B.size();

    // Initialize the 2D matrix
    vector<vector<int>> dp(sequence1_size + 1, vector<int>(sequence2_size + 1, 0));

    double gflops = 6 * sequence1_size * sequence2_size;

    struct timeval t;
    tick(&t);

    if (sequence1_size > sequence2_size)
    {
        swap(sequence1_size, sequence2_size);
        swap(A, B);
    }

    // Fill the first row and column with gap penalties
    for (int i = 0; i <= sequence1_size; i++)
        dp[i][0] = i * gap_penalty;

    for (int i = 0; i <= sequence2_size; i++)
        dp[0][i] = i * gap_penalty;

    for (int i = 1; i <= sequence1_size; i++)
        for (int j = 1; j <= sequence2_size; j++)
        {
            int score = A[i - 1] == B[j - 1] ? match_penalty : mismatch_penalty;
            dp[i][j] = max(dp[i - 1][j - 1] + score, max(dp[i - 1][j] + gap_penalty, dp[i][j - 1] + gap_penalty));
        }

    double time = tock(&t);

    // cout << "Execution Time: " << time * 1000 << "\n";
    // cout << "Operational Intensity: " << (double)(gflops) / (4 * sequence1_size * sequence2_size) << "\n";
    // cout << "GIPS: " << gflops / (time * 1e9) << "\n";
    return gflops / (time * 1e9);
}

int main()
{
    ofstream fout("optimized_1_output.txt");

    for (int i = 1; i <= 1000; i++)
    {
        string A = generate_random_string(i), B = generate_random_string(i);

        int match_penalty = 1, mismatch_penalty = -1, gap_penalty = -2;

        double time = needleman_wunsch(A, B, match_penalty, mismatch_penalty, gap_penalty);

        fout << time << " " << i << " "
             << "\n";
    }

    fout.close();
}