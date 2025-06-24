#include<iostream>
#include "custom_list.h"
#include "custom_vector.h"


template<class Array>
void PrintArray( Array & arr ){
  std::cout<<"\n Array contains:"<<std::endl;
  for (auto iter = arr.begin(); iter != arr.end(); ++iter) {
    std::cout << *iter << " ";
  }
  std::cout << "\n";
}

template <typename Container>
void TestContainer(const std::string & name) {
    std::cout << "\n" << name << "\n";

    Container container;

    // 1. Добавление 0–9
    for (int i = 0; i < 10; ++i)
        container.push_back(i);

    std::cout << "\nStep 1 (initial): ";
    PrintArray(container);
    std::cout << "Expected: 0 1 2 3 4 5 6 7 8 9";
    std::cout << "\nStep 2 (size): " << container.size() << "\n";

    // Удаление 3-го, 5-го и 7-го (по счёту, индексы меняются)

    container.erase(2);
    container.erase(3);
    container.erase(4);

    std::cout << "\nStep 3 (after erasures): ";
    PrintArray(container);
    std::cout << "Expected: 0 1 3 5 7 8 9";

    // Вставка 10 в начало
    container.insert(container.begin(), 10);

    std::cout << "\nStep 4 (insert 10 at front): ";
    PrintArray(container);
    std::cout << "\n Expected: 10 0 1 3 5 7 8 9";

    // Вставка 20 в середину
    auto it = container.begin();
    std::advance(it, 4);
    container.insert(it, 20);

    std::cout << "\nStep 5 (insert 20 in middle): ";
    PrintArray(container);
    std::cout << "\n Expected: 10 0 1 3 20 5 7 8 9";

    // Вставка 30 в конец
    container.push_back(30);

    std::cout << "\nStep 6 (insert 30 at end): ";
    PrintArray(container);
    std::cout << "\n Expected: 10 0 1 3 20 5 7 8 9 30";
}

int main()
{
  TestContainer<CArray<int>>("Custom array container test");
  TestContainer<ListContainer<int>>("List container test");
  return 0;
}