


#ifndef ___SKIP_LIST_HPP
#define ___SKIP_LIST_HPP

#include <cmath> // for log2
#include <string>
#include <vector>

#include "runtimeexcept.hpp"

// If we are inserting unsigned integer x into a skip list,
// and have made i previous coin flips on this particular insertion
// this will return the result of a coin flip.
// A return of true indicates heads and false means tails.
static bool flipCoin(unsigned x, unsigned i)
{
	char c;
	unsigned first8Bits = (x & 0xFF000000) / 0x01000000 ;
	unsigned next8Bits =  (x & 0x00FF0000) / 0x00010000;
	unsigned andThen =	 (x & 0x0000FF00) / 0x00000100;
	unsigned lastBits =   (x & 0x000000FF);
	c = first8Bits ^ next8Bits ^ andThen ^ lastBits;
	i = i % 8;
	return ( c & (1 << i) ) != 0;
}

// If we are inserting a std::string into the skip list, we use this instead.
static bool flipCoin(std::string s, unsigned i)
{
	char c = s[0];
	for(unsigned j = 1; j < s.length(); j++)
	{
		c = c ^ s[j];
	}
	i = i % 8;
	return ( c & (1 << i) ) != 0;
}



template<typename Key, typename Value>
class SkipList
{


private:
	// private variables go here.


public:


	SkipList();

	// You DO NOT need to implement a copy constructor or an assignment operator.

	~SkipList();

	// How many distinct keys are in the skip list?
	size_t size() const noexcept;

	// Does the Skip List contain zero keys?
	bool isEmpty() const noexcept;

	// How many layers are in the skip list?
	// Note that an empty Skip List has two layers by default,
	// the "base" lane S_0 and the "fast" lane S_1.
	//
	// [S_1: Fast lane] -inf ------> inf
	// [S_0: Base lane] -inf ------> inf
	//
	// This "empty" Skip List has two layers and a height of one.
	unsigned numLayers() const noexcept;

	// What is the height of this key, assuming the "base" lane S_0
	// contains keys with a height of 1?
	// For example, "0" has a height of 1 in the following skip list.
	//
	// [S_1]  -inf --------> inf
	// [S_0]  -inf --> 0 --> inf
	//
	// Throw an exception if this key is not in the Skip List.
	unsigned height(const Key & k) const;


	// If this key is in the SkipList and there is a next largest key
	// return the next largest key.
	// This function should throw a RuntimeException if either the key doesn't exist
	// or there is no subsequent key. 
	// A consequence of this is that this function will
	// throw a RuntimeException if *k* is the *largest* key in the Skip List.
	Key nextKey(const Key & k) const;

	// If this key is in the SkipList and a next smallest key exists,
	// return the next smallest key.
	// This function should throw a RuntimeException if either the key doesn't exist
	// or there is no previous key. 
	// A consequence of this is that this function will
	// throw a RuntimeException if *k* is the *smallest* key in the Skip List.
	Key previousKey(const Key & k) const;


	// These return the value associated with the given key.
	// Throw a RuntimeException if the key does not exist.
	Value & find(const Key & k);
	const Value & find(Key k) const;

	// Return true if this key/value pair is successfully inserted, false otherwise.
	// See the project write-up for conditions under which the key should be "bubbled up"
	// to the next layer.
	// If the key already exists, do not insert one -- return false.
	bool insert(const Key & k, const Value & v);


	// Return a vector containing all inserted keys in increasing order.
	std::vector<Key> allKeysInOrder() const
	{
		// you are allowed to use a std::vector in this function.
		std::vector<Key> r;

		return r;
	}


	// Is this the smallest key in the SkipList? Throw a RuntimeException
	// if the key *k* does not exist in the Skip List. 
	bool isSmallestKey(const Key & k) const;

	// Is this the largest key in the SkipList? Throw a RuntimeException
	// if the key *k* does not exist in the Skip List. 
	bool isLargestKey(const Key & k) const;

	// I am not requiring you to implement remove.


};

#endif

