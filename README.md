# Select Random Item

The below algorithm uses Reservoir Sampling https://www.youtube.com/watch?v=A1iwzSew5QY

## Introduction

Selecting a random item from a pre-defined array is typically done via randomly selecting an index based on the array size, then accessing the item at that index.

```c++
Item GetRandomItem(const std::vector<Item>& items)
{
  // Assumes at least one item - 
  // Note: This is not a good way of getting a random number in a range, but used for brevity. 
  //  see https://www.youtube.com/watch?v=LDPMpc-ENqY 
  //  or http://c-faq.com/lib/randrange.html
  return items[rand() % items.size()];
}
```
However, once you want to select a random item from a subset of an array (using a predicate) the problem gets more difficult.

For example, in a game you want to spawn a random unit of a type that matches some conditions (eg level, health range, weapon etc).

This is typically solved by one of three methods presented here. Following is a fourth method designed to overcome some limitations.

### 1) Sort array

By sorting the array using the predicate, you can then select a random item from the sub-part of the array.

```c++
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
```

**Disadvantages**
 - Re-orders the array
 - Lots of data movement
 - Cannot process data that is temporal (eg. network sourced)

### 2) Temporary Array

Copy all the items or item indices that match the predicate to a separate array, then select a random item from it.

```c++
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
```


**Disadvantages**
 - Typically allocates memory
 - May copy data if data is temporal (eg. network sourced)

### 3) Dual iterate

Iterate the data getting a count of items that match the predicate.

Next, select an index based on this count. 

Finally iterate the data again and stop when matching an item of the found index.

In some cases these count values can be pre-processed offline if doing tests on constant data with known predicates.

```c++
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
```


**Disadvantages**
 - Touching data twice
 - Cannot process data that is temporal (eg. network sourced)

## 4) Random select

On each match of the predicate, call the random number generator to determine if to keep the existing result or take the new one.

This is done by getting a random number between 0 and previous found item count, then taking the new item if equal to 0. 
(eg. probabilities of taking the new item are 100%, 50%, 33.33%, 25% ...) This is identical to selecting a random item from a known sized array.

This has the major advantages of working on temporal data and does not allocate memory.

```c++
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
```

**Disadvantages**
 - Require a call to random each time the predicate is matched - so calls to random needs to be fast.

## Conclusion

Presented was four different approaches for selected a random item from an array using a pedicate. 

The three obvious approaches show different disadvantages that can be overcome by the RandomSelect method assuming calls to your random number generator are performant. 
