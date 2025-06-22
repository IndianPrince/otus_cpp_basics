#include<iostream>
#include "containers.h"


template<class Array>
void PrintArray( Array & arr ){
  std::cout<<"Array contains:"<<std::endl;
  for (auto iter = arr.begin(); iter != arr.end(); ++iter) {
    std::cout<<*iter<<" ";
  }
  std::cout << "\n";
}

template <typename Container>
void TestContainer(const std::string& name) {
    std::cout << "\n Тест" << name << " ===\n";

    Container container;

    // 1. Добавление 0–9
    for (int i = 0; i < 10; ++i)
        container.push_back(i);

    std::cout << "Step 3 (initial): ";
    PrintArray(container);

    std::cout << "Step 4 (size): " << container.size() << " (expected 10)\n";

    // Удаление 3-го, 5-го и 7-го (по счёту, индексы меняются)
    auto it = container.begin();
    std::advance(it, 2);
    container.erase(it);

    it = container.begin();
    std::advance(it, 3);
    container.erase(it);

    it = container.begin();
    std::advance(it, 4);
    container.erase(it);

    std::cout << "Step 6 (after erasures): ";
    PrintArray(container); // expected: 0 1 3 5 7 8 9

    // Вставка 10 в начало
    container.insert(container.begin(), 10);

    std::cout << "Step 8 (insert 10 at front): ";
    PrintArray(container); // expected: 10 0 1 3 5 7 8 9

    // Вставка 20 в середину (на позицию 5)
    it = container.begin();
    std::advance(it, 4);
    container.insert(it, 20);

    std::cout << "Step 10 (insert 20 in middle): ";
    PrintArray(container); // expected: 10 0 1 3 20 5 7 8 9

    // Вставка 30 в конец
    container.push_back(30);

    std::cout << "Step 12 (insert 30 at end): ";
    PrintArray(container); // expected: 10 0 1 3 20 5 7 8 9 30
}

int main()
{
  TestContainer<CArray<int>>("CArray test");
  return 0;
}