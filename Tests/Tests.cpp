
// Adjust these defines to choose between accuracy and speed
#define USE_FAST_RAND_AND_DISTRIBUTION

#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#ifdef USE_FAST_RAND_AND_DISTRIBUTION

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
  return XorShift32() % (i_max + 1); // i_max cannot be uint32 limit (note that using % does not give an even distribution of ints)
}

#else // !USE_FAST_RAND_AND_DISTRIBUTION

// Use http://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
static std::mt19937 s_gen(1234); //Standard mersenne_twister_engine seeded with 1234
inline uint32_t RandRange(uint32_t i_max)
{
  std::uniform_int_distribution<uint32_t> dis(0, i_max);
  return dis(s_gen); // Perhaps faster to use the same "dis" each time and pass the parameters for range?
}

#endif // !USE_FAST_RAND_AND_DISTRIBUTION


///////////////////////////////////////////////////////////////////////////////
template<class T, class P>
bool RandomItemSortArray(std::vector<T>& items, P test, T& returnItem)
{
  // Put all the matching items at the end of the array
  auto first = std::find_if(items.begin(), items.end(), test);
  if (first != items.end())
  {
    for (auto i = first; ++i != items.end(); )
    {
      if (!test(*i))
      {
        std::swap(*first, *i);
        *first++;
      }
    }
  }
    
  auto itemCount = std::distance(first, items.end());
  if (itemCount == 0)
  {
    return false;
  }

  // Select a random index
  uint32_t matchIndex = RandRange((uint32_t)itemCount - 1);
  returnItem = *(first + matchIndex);

  return true;
}

///////////////////////////////////////////////////////////////////////////////
template<class T, class P>
bool RandomItemTempArray(const std::vector<T>& items, P test, T& returnItem)
{
  std::vector<uint32_t> indices;
  for (uint32_t i = 0; i < items.size(); i++)
  {
    // If matching the test
    if (test(items[i]))
    {
      indices.push_back(i);
    }
  }

  if (indices.size() == 0)
  {
    return false;
  }

  // Select a random index
  uint32_t matchIndex = RandRange((uint32_t)indices.size() - 1);
  returnItem = items[indices[matchIndex]];

  return true;
}

///////////////////////////////////////////////////////////////////////////////
template<class T, class P>
bool RandomItemDualIterate(const std::vector<T>& items, P test, T& returnItem)
{
  uint32_t matchingCount = 0;
  for (const T& i : items)
  {
    // If matching the test
    if (test(i))
    {
      matchingCount++;
    }
  }

  if (matchingCount == 0)
  {
    return false;
  }

  // Select a random index
  uint32_t matchIndex = RandRange(matchingCount - 1);
  matchingCount = 0;
  for (const T& i : items)
  {
    // If matching the test
    if (test(i))
    {
      // If matching the index
      if (matchingCount == matchIndex)
      {
        returnItem = i;
        break;
      }
      matchingCount++;
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
template<class T, class P>
bool RandomItemSelect(const std::vector<T>& items, P test, T& returnItem)
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


#define PerformTest(test)                                                             \
{                                                                                     \
  std::string foundItem;                                                              \
  std::cout << "Testing " << #test << "\n";                                           \
  std::vector<uint32_t> totals(26);                                                   \
  for (uint32_t i = 0; i < 10000; i++)                                                \
  {                                                                                   \
    if (!test(testData, [](const std::string& i) { return i[0] == 'a'; }, foundItem)) \
    {                                                                                 \
      std::cout << "Unexpected not found!\n";                                         \
    }                                                                                 \
    else                                                                              \
    {                                                                                 \
      totals[foundItem[1] - 'a']++;                                                   \
    }                                                                                 \
  }                                                                                   \
  std::cout << "Results: \n";                                                         \
  for (uint32_t i = 0; i < 26; i++)                                                   \
  {                                                                                   \
    std::cout << "  " << (char)('a' + i) << " = " << totals[i] << "\n";               \
  }                                                                                   \
                                                                                      \
  if (test(testData, [](const std::string& i) { return i[0] == 'A'; }, foundItem))    \
  {                                                                                   \
    std::cout << "Unexpected found A!\n";                                             \
  }                                                                                   \
}


int main()
{
  // Create a test array with random data
  std::vector<std::string> testData;
  for (uint32_t i = 0; i < 100000; i++)
  {
    std::string newString;
    newString.append(1, 'a' + RandRange(25));
    newString.append(1, 'a' + RandRange(25));
    newString.append(1, 'a' + RandRange(25));
    newString.append(1, 'a' + RandRange(25));
    newString.append(1, 'a' + RandRange(25));
    testData.push_back(newString);
  }

  // Select from the array with the different methods
  PerformTest(RandomItemDualIterate);
  PerformTest(RandomItemTempArray);
  PerformTest(RandomItemSelect);
  PerformTest(RandomItemSortArray); // Done last as it re-orders the array

  return 0;
}



