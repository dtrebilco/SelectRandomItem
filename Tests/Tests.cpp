
// Adjust these defines to choose between accuracy and speed
#define USE_FAST_RAND
#define USE_FAST_DISTRIBUTION

// Use http://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution

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


// Fast random from https://en.wikipedia.org/wiki/Xorshift
static uint32_t s_rndState = 1234;
inline uint32_t XorShift32()
{
  uint32_t x = s_rndState;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 15;
  s_rndState = x;
  return x;
}

inline uint32_t RandRange(uint32_t i_max)
{
  return XorShift32() % (i_max + 1); // i_max cannot be uint32 limit
}


template<class T, class P>
bool RandomSelect(const std::vector<T>& items, P test, T& returnItem)
{
  uint32_t matchingCount = 0;
  for (const T& i : items)
  {
    // If matching the test
    if (test(i))
    {
      // Check if selected
      if (RandRange(matchingCount) == 0)
      {
        returnItem = i;
      }
      matchingCount++;
    }
  }

  return matchingCount > 0;
}

int main()
{
  s_csvFile.open("test_profile.csv");

  s_csvFile.close();

  return 0;
}

// Create a test array with random data


// Select from the array with the different methods


