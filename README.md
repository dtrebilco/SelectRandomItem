# Select Random Item

## Introduction

Selecting a random item from a pre-defined array is typically done via randomly selecting an index based on the array size, then accessing the item at that index.

```c++
Item GetRandomItem(const std::vector<Item>& items)
{
  // Assumes at least one item - 
  // Note: This is not a good way of gettng a random number in a range, but used for brevity. 
  //  see https://www.youtube.com/watch?v=LDPMpc-ENqY 
  //  or http://c-faq.com/lib/randrange.html
  return items[rand() % items.size()];
}
```
However, once you want to select a random item from a subset of an array (using a predicate) the problem gets more difficult.

For example, in a game you spawn a random unit of a type that matches some conditions (eg level, health range, weapon etc).

This is typically solved by one of three methods presented here. Following is a fourth method designed to overcome some limitations.

### 1) Sort array

By sorting the array using the predicate, you can then select a random item from the sub-part of the array.


**Disadvantages**
 - Re-orders the array
 - Lots of data movement
 - Cannot process data that is temporal (eg. network sourced)

### 2) Temporary Array

Copy all the items or item indices that match the predicate to a separate array, then select a random item from it.

**Disadvantages**
 - Typically allocates memory
 - May copy data if data is temporal (eg. network sourced)

### 3) Dual iterate

Iterate the data getting a count of items that match the predicate. Then select an index based on this count. 

Then iterate the data again and stop when matching an item of the found index.

In some cases these count values can be pre-processed offline if doing tests on constant data with known predicates.

**Disadvantages**
 - Touching data twice
 - Cannot process data that is temporal (eg. network sourced)

## 4) Random select

On each match of the predicate, call the random number generator to determine if to keep the existing result or take the new one.

This is done by getting a random number between 0 and previous found item count, then taking the new item if equal to 0. 
(eg. probabilities of taking the new item are 100%, 50%, 33.33%, 25% ...) This is identical to selecting a random item from a known sized array.

This has the major advantages of working on temporal data and does not allocate memory.

```c++
template<class P> 
bool RandomSelect(const std::vector<Item>& items, P test, Item& returnItem)
{
  uint32_t matchingCount = 0;
  for (const Item& i : items)
  {
    // If matching the test
    if (test(i))
    {
      // Check if selected
      matchingCount++;
      if (rand() % matchingCount == 0)
      {
        returnItem = i;
      }
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
