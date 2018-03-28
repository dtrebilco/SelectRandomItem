
// Adjust these defines to choose between accuracy and speed
#define USE_FAST_RAND
#define USE_FAST_DISTRIBUTION

#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <numeric>

#include <vector>

static int32_t LOOPCOUNT = 1000;
static uint32_t TEST_LOOPCOUNT = 100;

static std::ofstream s_csvFile;

#define TIMER_START  std::chrono::microseconds shortDuration; \
                     for (uint32_t t = 0; t < TEST_LOOPCOUNT; t++) \
                     { \
                       std::vector<T> process(LOOPCOUNT); \
                       for (auto& i : process) \
                       { \
                         i = a_data; \
                       } \
                       auto start_time = chrono::high_resolution_clock::now();

#define TIMER_END      auto duration_time = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start_time); \
                       if (t == 0 || duration_time < shortDuration) \
                       { \
                         shortDuration = duration_time; \
                       } \
                     } \
                     cout << chrono::duration_cast<chrono::microseconds>(shortDuration).count() << "us\n"; \
                     s_csvFile << chrono::duration_cast<chrono::microseconds>(shortDuration).count() << ",";


using namespace std;


int main()
{
  s_csvFile.open("test_profile.csv");

  s_csvFile.close();

  return 0;
}

// Create a test array with random data


// Select from the array with the different methods


