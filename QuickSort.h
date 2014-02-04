#pragma once

#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

typedef tuple<string, int, string> MyTuple;

void QuickSort (vector<MyTuple>& myVector, int low, int high);

int partition(vector<MyTuple>& myVector, int low, int high);