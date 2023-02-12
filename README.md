## skip_list
When you insert a key/value pair into your Skip List and flip a coin to determine higher levels, use the function flipCoin. This is described at the top of SkipList.hpp This returns true if the coin flip returned heads (meaning add a layer). The “bubbling up” procedure should be halted if the addition of a new layer would violate any of the following conditions:
■ For a skip list with 16 or fewer elements (including the element that was just added in the bottom-most lane) the maximum number of layers is 13. This includes the “fast” lane (Sh, where h is the height of the skip list) and the “base” lane S0 , see section 9.4 in the textbook for further elaboration on this distinction. See SimpleHeightsTest in SkipTests.cpp for a concrete example.
■ For a skip list with more than 16 elements the maximum number of layers is 3 * ceil(log2(n)) + 1. In other words, the skip list should have a height of 3 * ceil(log2(n)) in the worst case. The library cmath has a function for logarithms and ceiling, and you may use this

○ The functions height(), nextKey(), previousKey(), find(),
isSmallestKey(), and isLargestKey() should throw a RunimeException (provided in runtimeexcept.hpp) if the skip list does not contain the passed key k. More details are provided in SkipList.hpp for the implementations of these functions.
