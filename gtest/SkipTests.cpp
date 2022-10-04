#include "gtest/gtest.h"
#include <vector>
#include "SkipList.hpp"


namespace{


// NOTE:  these are not intended as exhaustive tests.
	// This should get you started testing.
	// You should make your own additional tests
	// VERY IMPORTANT:  if your SkipList is not templated, or if 
	//		it is not built as a linked structure, your score for this project
	//		will be zero.  Be sure your SkipList compiles and runs 
	// 		with non-numeric data types. 


	TEST(SampleTests, CreatedBasics)
	{
		SkipList<unsigned, unsigned> sl;
		EXPECT_EQ( 2, sl.numLayers());
		EXPECT_EQ( 0, sl.size() );
		EXPECT_TRUE( sl.isEmpty() );
	}

	TEST(SampleTests, SkipListTest1)
	{
		SkipList<unsigned, unsigned> sl;
		sl.insert(3, 5);
		EXPECT_TRUE( sl.find(3) == 5 );
	}

	TEST(SampleTests, SkipListTest2)
	{
		SkipList<std::string, std::string> sl;
		sl.insert("Shindler", "ICS 46");
		EXPECT_TRUE(sl.find("Shindler") == "ICS 46");
	}



	TEST(SampleTests, SimpleHeightsTest)
	{
		SkipList<unsigned, unsigned> sl;
		std::vector<unsigned> heights;
		for (unsigned i = 0; i < 10; i++)
		{
			sl.insert(i, i);
			heights.push_back(sl.height(i));
		}

		// The coinFlip function will always return heads
		// for 255 regardless of the current layer.
		// You can use this value to test your threshold for halting
		// the insertion procedure. If this results in an infinite loop inside
		// of your insert function you have not implemented a cutoff threshold.
		unsigned const MAGIC_VAL = 255;
		sl.insert(MAGIC_VAL, MAGIC_VAL);

		heights.push_back(sl.height(MAGIC_VAL));

		// The expected height for 255 is 12 because there are fewer than 16 nodes in
		// the skip list when 12 is added.
		std::vector<unsigned> expectedHeights = {1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 12};
		EXPECT_TRUE(heights == expectedHeights);

		// At this point there should be 13 layers
		// (because the fast lane is not included in the height calculation).
		EXPECT_TRUE(sl.numLayers() == 13);
	}

	TEST(SampleTests, Capacity17Test)
	{
		SkipList<unsigned, unsigned> sl;
		std::vector<unsigned> heights;

		// First insert 16 values into the skip list [0, 15].
		for (unsigned i = 0; i < 16; i++)
		{
			sl.insert(i, i);
			heights.push_back(sl.height(i));
		}

		// Same value used in SimpleHeightsTest for testing the threshold.
		unsigned const MAGIC_VAL = 255;
		sl.insert(MAGIC_VAL, MAGIC_VAL);

		heights.push_back(sl.height(MAGIC_VAL));

		// The expected height for 255 is 15 because 3 * ceil(log_2(17)) = 15
		// meaning the maximum height of any node should be 15 for a skip list with 17 nodes.
		std::vector<unsigned> expectedHeights = {1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 15};
		EXPECT_TRUE(heights == expectedHeights);

		// At this point there should be 16 layers
		// (because the fast lane is not included in the height calculation).
		EXPECT_TRUE(sl.numLayers() == 16);
	}

	TEST(SampleTests, SimpleNextAndPrev)
	{
		SkipList<unsigned, unsigned> sl;
		for(unsigned i=0; i < 10; i++)
		{
			sl.insert(i,i);
		}
		for(unsigned i=1; i < 9; i++)
		{
			EXPECT_TRUE(sl.previousKey(i) == (i-1) and sl.nextKey(i) == (i+1) );
		}
	}

	TEST(SampleTests, SimpleFindTest)
	{
		SkipList<unsigned, unsigned> sl;
		for(unsigned i=0; i < 10; i++)
		{
			sl.insert(i, (100 + i) );
		}
		for(unsigned i=0; i < 10; i++)
		{
			EXPECT_TRUE((i+100) == sl.find(i));
		}
	}


	TEST(SampleTests, SimpleAllKeysInOrder)
	{
		SkipList<unsigned, unsigned> sl;
		std::vector<unsigned> expected;
		for(unsigned i=0; i < 10; i++)
		{
			sl.insert(i, (100 + i) );
			expected.push_back( i );
		}
		EXPECT_TRUE( expected == sl.allKeysInOrder() );
	}

	TEST(SampleTests, SimpleLargestAndSmallest)
	{
		SkipList<unsigned, unsigned> sl;
		for(unsigned i=0; i < 10; i++)
		{
			sl.insert(i, (100 + i) );
		}
		EXPECT_TRUE( sl.isSmallestKey( 0 ) and sl.isLargestKey( 9 ) );

	}


}
