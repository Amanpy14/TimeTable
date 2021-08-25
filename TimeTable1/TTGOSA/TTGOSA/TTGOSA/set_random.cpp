#include "set_random.h"

void Set_random::insert(int x)
{
    // If element is already present, then nothing to do
    if (reverse_map.find(x) != reverse_map.end())
        return;

    // Else put element at the end of arr[]
    int index = values.size();
    values.push_back(x);

    // and hashmap also
    reverse_map.insert(std::pair<int, int>(x, index));
}


void Set_random::erase(int x) 
{
    // element not found then return
    if (reverse_map.find(x) == reverse_map.end())
        return;

    // remove element from map
    int index = reverse_map.at(x);
    reverse_map.erase(x);

    // swap with last element in arr
    // then remove element at back
    int last = values.size() - 1;
    std::swap(values[index], values[last]);
    values.pop_back();

    // Update hash table for new index of last element
    if (index != last) reverse_map.at(values[index]) = index;
}

int Set_random::find(int x) 
{
    if (reverse_map.find(x) != reverse_map.end())
        return reverse_map.at(x);
    return -1;
}

int Set_random::get_random() 
{
    // Find a random index from 0 to size - 1
    int random_index = rand() % values.size();

    // Return element at randomly picked index
    return values.at(random_index);
}

int Set_random::size()
{
    return values.size();
}

std::vector<int>& Set_random::get_values()
{
    return values;
}
