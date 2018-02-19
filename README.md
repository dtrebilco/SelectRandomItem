# Select Random Item

## Introduction

Selecting a random item from a pre-defined array is typically done via randomly selecting an index based on the array size, then accessing the item at that index.

However, once you want to select a random item from a subset of an array (using a predicate) the problem gets more difficult.

For example, in a database, selecting a random row that matches a condition.

This is typically solved by one of three methods presented here. Following is a fourth method designed to overcome some limitations.

### 1) Sort array

By sorting the array using the predicate, you can then select a random item from the sub-part of the array.


**Disadvantages**
 - Re-orders the array
 - Lots of data movement
 - Cannot process data that is temporal (eg. network sourced)

### 2) Temporary Array

Copy all the items that match the predicate to a separate array, then select a random item from it.

**Disadvantages**
 - May allocate memory
 - Copies data

### 3) Two iterate

Iterate the data getting a count of items that match the predicate. Then select an index based on this count. 

Then iterate the data again and stop when matching an item of the found index.


**Disadvantages**
 - Touching data twice
 - Cannot process data that is temporal (eg. network sourced)

## 4) Random select

On each match of the predicate, call the random number generator to determine if to keep the existing result or take the new one.



**Disadvantages**
 - Requires many calls to random

