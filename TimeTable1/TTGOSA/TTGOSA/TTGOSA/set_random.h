#pragma once

#include <vector>
#include <unordered_map>

/* Class for a data structure supporting following operations 
 * Insert 
 * Delete
 * Find
 * Get Random
*/
class Set_random
{

private: 
    // A resizable array
    std::vector <int> values;

    // A hash where keys are array elements and values are
    // indexes in arr[]
    std::unordered_map <int, int> reverse_map;

public:
    // A Theta(1) function to add an element to MyDS
    // data structure
    void insert(int x);

    // function to remove a number to DS in O(1)
    void erase(int x);

    // Returns index of element if element is present, otherwise -1
    int find(int x);

    // Returns a random element from myStructure
    int get_random();

    // Returns the number of elements in the set
    int size();

    std::vector<int>& get_values();
};