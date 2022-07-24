#ifndef __HELPER_HPP__
#define __HELPER_HPP__

#include <string>

using namespace std;

void tick(struct timeval *t);
double tock(struct timeval *t);
string generate_random_string(int n);

#endif