#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>

using namespace std;

void tick(struct timeval *t)
{
    gettimeofday(t, NULL);
}

double tock(struct timeval *t)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return (double)(now.tv_sec - t->tv_sec) +
           ((double)(now.tv_usec - t->tv_usec) / 1000000.);
}

string generate_random_string(int n)
{
    srand(time(NULL));

    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", res;

    for (int i = 0; i < n; i++)
        res += alphabet[rand() % 26];

    return res;
}