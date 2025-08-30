#pragma once

template <typename Container>
bool DefaultConstructionTest()
{
  Container container;
  return container.size() == 0;
}


template <typename Container>
bool ReserveConstructionTest()
{
  Container container(5);
  return container.size() == 0;
}
