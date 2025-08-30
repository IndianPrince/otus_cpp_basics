#include "custom_vector.h"
#include"container_test_funcs.h"
#include <gtest/gtest.h>

struct CArrayFixture : public testing::Test 
{
  const size_t cnt = 10;
  CArray<size_t> container;

  void SetUp() override 
  {
    for ( size_t i = 0; i < cnt; i++ )
      container.push_back( i );
  }

  void TearDown() override 
  {
    container.clear();
  }

};


TEST( CArray, constructionTest ) {
  EXPECT_TRUE( DefaultConstructionTest<CArray<size_t>>() );
  EXPECT_TRUE( ReserveConstructionTest<CArray<size_t>>() );
}

TEST_F( CArrayFixture, addElementBack ) {
  container.push_back( 100 );
  EXPECT_EQ( cnt+1, container.size() );
  EXPECT_EQ( 100, *(container.end() - 1) );
}

TEST_F( CArrayFixture, addElementFront ) {
  container.insert( container.begin(), 100 );
  EXPECT_EQ( cnt+1, container.size() );
  EXPECT_EQ( 100, container[0] );
}

TEST_F( CArrayFixture, addElementMid ) {
  size_t mid = cnt / 2;
  container.insert( container.begin() + mid, 100 );
  EXPECT_EQ( cnt+1, container.size() );
  EXPECT_EQ( 100, container[mid] );
}

TEST_F( CArrayFixture, eraseElementBack ) {
  size_t previous = container[cnt-2];
  container.pop_back();
  EXPECT_EQ( cnt-1, container.size() );
  EXPECT_EQ( previous, *(container.end() - 1) );
}

TEST_F( CArrayFixture, eraseElementFront ) {
  size_t second = container[1];
  container.erase( container.begin() );
  EXPECT_EQ( cnt-1, container.size() );
  EXPECT_EQ( second, *container.begin() );
}

TEST_F( CArrayFixture, eraseElementMid ) {
  size_t mid = cnt / 2;
  size_t next = mid + 1; 
  container.erase( mid );
  EXPECT_EQ( cnt - 1, container.size() );
  EXPECT_EQ( next, container[mid] );
}

TEST_F( CArrayFixture, getElementByIndex ) {
  EXPECT_EQ( cnt-3, container[cnt-3] );
}

TEST_F( CArrayFixture, getContainerSize ) {
  EXPECT_EQ( cnt, container.size() );
}

TEST_F( CArrayFixture, copyConstructorTest ) {
  CArray<size_t> container2( container );
  EXPECT_EQ( container.size(), container2.size() );
  
}