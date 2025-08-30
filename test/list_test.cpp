#include "custom_list.h"
#include "container_test_funcs.h"
#include <gtest/gtest.h>

struct ListFixture : public testing::Test 
{
  const size_t cnt = 10;
  ListContainer<size_t> container;

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


TEST( ListContainer, constructionTest ) {
  EXPECT_TRUE( DefaultConstructionTest<ListContainer<size_t>>() );
}

TEST_F( ListFixture, addElementBack ) {
  container.push_back( 100 );
  EXPECT_EQ( cnt+1, container.size() );
  EXPECT_EQ( 100, container.back() );
}

TEST_F(ListFixture, addElementFront ) {
  container.insert( container.begin(), 100 );
  EXPECT_EQ( cnt+1, container.size() );
  EXPECT_EQ( 100, container.front() );
}

TEST_F( ListFixture, addElementMid ) {
  size_t mid = cnt / 2;
  auto it = container.begin();
  std::advance( it, mid );
  container.insert( it, 100 );
  EXPECT_EQ( cnt+1, container.size() );
  EXPECT_EQ( 100, container[mid] );
}

TEST_F( ListFixture, eraseElementBack ) {
  size_t previous = container[cnt-2];
  container.pop_back();
  EXPECT_EQ( cnt-1, container.size() );
  EXPECT_EQ( previous, container.back() );
}

TEST_F( ListFixture, eraseElementFront ) {
  size_t second = container[1];
  container.erase( container.begin() );
  EXPECT_EQ( cnt-1, container.size() );
  EXPECT_EQ( second, container.front() );
}

TEST_F( ListFixture, eraseElementMid ) {
  size_t mid = cnt / 2;
  size_t next = mid + 1; 
  container.erase( mid );
  EXPECT_EQ( cnt - 1, container.size() );
  EXPECT_EQ( next, container[mid] );
}

TEST_F( ListFixture, getElementByIndex ) {
  EXPECT_EQ( cnt-3, container[cnt-3] );
}

TEST_F( ListFixture, getContainerSize ) {
  EXPECT_EQ( cnt, container.size() );
}

TEST_F( ListFixture, copyConstructorTest ) {
  ListContainer<size_t> container2( container );
  EXPECT_EQ( container.size(), container2.size() );
}