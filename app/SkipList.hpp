#ifndef ___SKIP_LIST_HPP
#define ___SKIP_LIST_HPP

#include <cmath> // for log2
#include <string>
#include <vector>
#include <memory>
#include <limits>

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

template<typename Key, typename Value> class SkipList;

template<typename Key, typename Value>
class SkipNode
{
	
	friend class SkipList<Key, Value>;
	
public:
	
	Key key;
	Value val;
	SkipNode<Key, Value>* next;
	SkipNode<Key, Value>* prev;
	SkipNode<Key, Value>* down;
	SkipNode<Key, Value>* up;
	
public:
	
	SkipNode(const Key & key = Key(), const Value & val = Value()):
	key(key),
	val(val),
	next(nullptr),
	prev(nullptr),
	down(nullptr),
	up(nullptr)
	{
		
	}
	~SkipNode()
	{
		
	}
	
};

template<typename Key>
class MinLimits
{
	
public:
	
	Key operator()() const
	{
		return std::numeric_limits<Key>::min();
	}
	
};

template<>
class MinLimits<std::string>
{
	
public:
	
	std::string operator()() const
	{
		return "\0";
	}
	
};

template<typename Key>
class MaxLimits
{
	
public:
	
	Key operator()() const
	{
		return std::numeric_limits<Key>::max();
	}
	
};

template<>
class MaxLimits<std::string>
{
	
public:
	
	std::string operator()() const
	{
		return "\0";
	}
	
};

template<typename Key, typename Value>
class SkipListIterator
{
	
	friend class SkipList<Key, Value>;
	
private:
	
	SkipNode<Key, Value>* findNode(SkipNode<Key, Value>* current, const Key& target,  SkipNode<Key, Value>* currentHead) const
	{
		while(current != nullptr)
		{
			if(current->key == target)
			{
				break;
			}
			else if(target > current->key && target < current->next->key)
			{
				current = current->down;
			}
			else
			{
				current = current->next;
			}
		}
		return current;
	}
	bool isFirstParameterGreater(const Key & k1, const Key & k2) const
	{
		return k1 > k2;
	}
	
};

template<typename Value>
class SkipListIterator<std::string, Value>
{
	
	friend class SkipList<std::string, Value>;
	
private:
	
	SkipNode<std::string, Value>* findNode(SkipNode<std::string, Value>* current, const std::string& target, SkipNode<std::string, Value>* currentHead) const
	{
		currentHead = currentHead->down;
		while(current != nullptr)
		{
			if(current->key == target)
			{
				break;
			}
			else if(target.size() >= current->key.size() && current->next->next != nullptr)
			{
				current = current->next;
			}
			else 
			{
				current = currentHead;
				if(currentHead) currentHead = currentHead->down;
			}
		}
		return current;
	}
	bool isFirstParameterGreater(const std::string & k1, const std::string & k2)
	{
		return k1.size() > k2.size();
	}
	
};



template<typename Key, typename Value>
class SkipList
{
	
private:
	// private variables go here.
	SkipNode<Key, Value>* topHead;
	SkipNode<Key, Value>* topRear;
	SkipNode<Key, Value>* btmHead;
	SkipNode<Key, Value>* btmRear;
	unsigned layerCount;
	unsigned nodeCount;
	unsigned layerCapacity;
	SkipListIterator<Key, Value> itor;

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
	std::vector<Key> allKeysInOrder() const;


	// Is this the smallest key in the SkipList? Throw a RuntimeException
	// if the key *k* does not exist in the Skip List. 
	bool isSmallestKey(const Key & k) const;

	// Is this the largest key in the SkipList? Throw a RuntimeException
	// if the key *k* does not exist in the Skip List. 
	bool isLargestKey(const Key & k) const;

	// I am not requiring you to implement remove.

private:
	
	SkipNode<Key, Value>* getNodePostion(const Key & k) const;
	
	void increaseLayerCapacity();
	
	void addLayer();
	
	void clear();
	
	void print()
	{
		SkipNode<Key, Value>* current = topHead;
		SkipNode<Key, Value>* headker = topHead->down;
		int countlayer = layerCount-1;
		while(current)
		{
			std::cout << "Layer" << countlayer << ": ";
			while(current)
			{
				std::cout << current->key << " ";
				current = current->next;
			}
			std::cout << '\n';
			current = headker;
			if(headker) headker = headker->down;
			--countlayer;
		}
	}
};

template<typename Key, typename Value>
SkipList<Key, Value>::SkipList():
	topHead(new SkipNode<Key, Value>(MinLimits<Key>()())),
	topRear(new SkipNode<Key, Value>(MaxLimits<Key>()())),
	btmHead(new SkipNode<Key, Value>(MinLimits<Key>()())),
	btmRear(new SkipNode<Key, Value>(MaxLimits<Key>()())),
	layerCount(2),
	nodeCount(0),
	layerCapacity(13)
{
	topHead->next = topRear;
	topRear->prev = topHead;
	topHead->down = btmHead;
	topRear->down = btmRear;
	btmHead->next = btmRear;
	btmRear->prev = btmHead;
	btmHead->up = topHead;
	btmRear->up = topRear;
}

template<typename Key, typename Value>
SkipList<Key, Value>::~SkipList()
{
	clear();
}

template<typename Key, typename Value>
size_t SkipList<Key, Value>::size() const noexcept
{
	return nodeCount;
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::isEmpty() const noexcept
{
	return btmHead->next == btmRear;
}

template<typename Key, typename Value>
unsigned SkipList<Key, Value>::numLayers() const noexcept
{
	return layerCount;
}

template<typename Key, typename Value>
unsigned SkipList<Key, Value>::height(const Key & k) const
{
	SkipNode<Key, Value>* current = getNodePostion(k);
	if(!current) throw RuntimeException("key is not in the Skip List");
	unsigned currLayer = 1;
	while(current->down != nullptr)
	{
		current = current->down;
		++currLayer;
	}
	return currLayer;
}

template<typename Key, typename Value>
Key SkipList<Key, Value>::nextKey(const Key & k) const
{
	SkipNode<Key, Value>* current = getNodePostion(k);
	if(!current) throw RuntimeException("key is not in the Skip List");
	if(!current->next) throw RuntimeException("There is no subsequent key");
	if(current->key == MaxLimits<Key>()()) throw RuntimeException("k is the largest key in the Skip List.");
	return current->next->k;
}

template<typename Key, typename Value>
Key SkipList<Key, Value>::previousKey(const Key & k) const
{
	SkipNode<Key, Value>* current = getNodePostion(k);
	if(!current) throw RuntimeException("key is not in the Skip List");
	if(!current->prev) throw RuntimeException("There is no subsequent key");
	if(current->key == MinLimits<Key>()()) throw RuntimeException("k is the smallest key in the Skip List.");
	return current->prev->key;
}

template<typename Key, typename Value>
Value & SkipList<Key, Value>::find(const Key & k)
{
	SkipNode<Key, Value>* current = getNodePostion(k);
	if(!current) throw RuntimeException("key is not in the Skip List");
	return current->val;
}

template<typename Key, typename Value>
const Value & SkipList<Key, Value>::find(Key k) const
{
	SkipNode<Key, Value>* current = getNodePostion(k);
	if(!current) throw RuntimeException("key is not in the Skip List");
	const Value result = current->val;
	return result;
}

template<typename Key, typename Value>
SkipNode<Key, Value>* SkipList<Key, Value>::getNodePostion(const Key & k) const
{
	if(isEmpty()) return nullptr;
	SkipNode<Key, Value>* current = topHead->down;
	return itor.findNode(current, k, current);
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::insert(const Key & k, const Value & v)
{
	if(getNodePostion(k)) return false;
	SkipNode<Key, Value>* newNode = nullptr;
	SkipNode<Key, Value>* currentHead = btmHead;
	SkipNode<Key, Value>* newNodeBtm = nullptr;
	SkipNode<Key, Value>* current = nullptr;
	int flipCoinCount = 0;
	increaseLayerCapacity();
	while(flipCoin(k, flipCoinCount) && flipCoinCount < layerCapacity-layerCount-1)
	{
		++flipCoinCount;
	}
	if(flipCoinCount >= layerCount-1)
	{
		int newLayerCount = flipCoinCount - (layerCount-1) + 1;
		for(int i = 0; i < newLayerCount; ++i)
		{
			addLayer();
		}
	}
	//print();
	for(unsigned i = 0; i <= flipCoinCount; ++i)
	{
		current = currentHead->next;
		newNode = new SkipNode<Key, Value>(k, v);
		while(current->next != nullptr && itor.isFirstParameterGreater(k, current->key))
		{
			current = current->next;
		}
		newNode->next = current;
		current->prev->next = newNode;
		newNode->prev = current->prev;
		current->prev = newNode;
		newNode->down = newNodeBtm;
		if(newNodeBtm) newNodeBtm->up = newNode;
		currentHead = currentHead->up;
		newNodeBtm = newNode;
	}
	++nodeCount;
	return true;
}

template<typename Key, typename Value>
std::vector<Key> SkipList<Key, Value>::allKeysInOrder() const
{
	// you are allowed to use a std::vector in this function.
	if(isEmpty()) return {};
	std::vector<Key> r;
	r.reserve(nodeCount);
	SkipNode<Key, Value>* current = btmHead->next;
	while(current != btmRear)
	{
		r.push_back(current->key);
		current = current->next;
	}
	return r;
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::isSmallestKey(const Key & k) const
{
	if(!getNodePostion(k)) throw RuntimeException("key is not in the Skip List");
	if(btmHead->next->key == k) return true;
	return false;
	
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::isLargestKey(const Key & k) const
{
	if(!getNodePostion(k)) throw RuntimeException("key is not in the Skip List");
	if(btmRear->prev->key == k) return true;
	return false;
}

template<typename Key, typename Value>
void SkipList<Key, Value>::increaseLayerCapacity()
{
	if(nodeCount <= 16) return;
	layerCapacity = 3 * std::ceil(std::log2(nodeCount)) + 1;
}

template<typename Key, typename Value>
void SkipList<Key, Value>::addLayer()
{
	SkipNode<Key, Value>* newLayerHead = new SkipNode<Key, Value>(MinLimits<Key>()());
	SkipNode<Key, Value>* newLayerRear = new SkipNode<Key, Value>(MaxLimits<Key>()());
	newLayerHead->next = newLayerRear;
	newLayerRear->prev = newLayerHead;
	topHead->down->up = newLayerHead;
	newLayerHead->down = topHead->down;
	topHead->down = newLayerHead;
	newLayerHead->up = topHead;
	topRear->down->up = newLayerRear;
	newLayerRear->down = topRear->down;
	topRear->down = newLayerRear;
	newLayerRear->up = topRear;
	++layerCount;
}

template<typename Key, typename Value>
void SkipList<Key, Value>::clear()
{
	if(isEmpty()) return;
	while(topHead != nullptr)
	{
		SkipNode<Key, Value>* current = nullptr;
		SkipNode<Key, Value>* topHeadDown = topHead->down;
		while(topHead != nullptr)
		{
			current = topHead;
			topHead = current->next;
			delete current;
			current = nullptr;
		}
		topHead = topHeadDown;
		if(topHeadDown) topHeadDown = topHeadDown->down;
	}
}

#endif
