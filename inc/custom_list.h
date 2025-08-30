#pragma once
#include <iterator>
#include <stdexcept>

//------------------------------------
// Контейнер - список (аналог std::list)
//------------------------------------
template <typename T>
class ListContainer
{
private:
  struct Node
  {
    T value;
    Node * prev;
    Node * next;
    Node( const T & v ) : value( v ), prev( nullptr ), next( nullptr ) {}
  };

  Node * head;
  Node * tail;
  size_t length;

public:
  class Iterator
  {
    Node * node;

  public:
    Iterator( Node * n ) : node( n ) {}
    Node * getNode() const { return node; }
    // Совместимость
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    reference operator*() const { return node->value; }
    Iterator & operator++() { node = node->next; return *this; }
    Iterator operator++( int ) { Iterator tmp = *this; ++( *this ); return tmp; }
    Iterator & operator--() { node = node->prev; return *this; }
    Iterator operator--( int ) { Iterator tmp = *this; --( *this ); return tmp; }
    bool operator!=( const Iterator & other ) const { return node != other.node; }
    bool operator==( const Iterator & other ) const { return node == other.node; }
  };

  ListContainer();                                  // Конструктор по умолчанию
  ListContainer( const ListContainer & other );
  ~ListContainer();                                 // Деструктор

  void push_back( const T & value );                // Добавить эл-т в конец
  void pop_back();                                  // Удалить эл-т с конца
  void erase( size_t index );                       // Удалить эл-т по индексу
  void clear();                                     // Очистить список
  size_t size() const;                              // Дать кол-во элементов
  // Оператор получения по индексу
  T & operator[]( size_t index );
  const T & operator[]( size_t index ) const;

  Iterator begin();                                 // Начало списка
  Iterator end();                                   // Конец списка (nullptr)
  Iterator erase( Iterator first, Iterator last );  // Удалить диапазон элементов
  Iterator erase( Iterator pos );                   // Удалить один элемент по итератору
  Iterator insert( Iterator pos, const T & value ); // Вставить эл-т перед позицией
};


//---------------------------------------------
// Конструктор копирования
//---------------------------------------------
template <typename T>
inline ListContainer<T>::ListContainer( const ListContainer & other ) 
  : head( other.head )
  , tail( other.tail )
  , length( other.length ) 
  {}


//---------------------------------------------
// Конструктор по умолчанию
//---------------------------------------------
template <typename T>
inline ListContainer<T>::ListContainer() : head( nullptr ), tail( nullptr ), length( 0 ) {}


//---------------------------------------------
// Деструктор
//---------------------------------------------
template <typename T>
inline ListContainer<T>::~ListContainer()
{
  clear();
}


//---------------------------------------------
// Выдать элемент по индексу
//---------------------------------------------
template <typename T>
inline T & ListContainer<T>::operator[]( size_t index )
{
  if ( index >= length )
    throw std::out_of_range( "Index out of bounds" );

  Node* current = head;
  for (size_t cnt = 0; cnt < index; ++cnt)
    current = current->next();

  return current->value;
}


//---------------------------------------------
// Выдать элемент по индексу
//---------------------------------------------
template <typename T>
const inline T & ListContainer<T>::operator[]( size_t index ) const
{
  if ( index >= length )
    throw std::out_of_range( "Index out of bounds" );

  Node* current = head;
  for (size_t cnt = 0; cnt < index; ++cnt)
    current = current->next();

  return current->value;
}

//---------------------------------------------
// Добавить элемент в конец
//---------------------------------------------
template <typename T>
inline void ListContainer<T>::push_back( const T & value )
{
  Node * node = new Node( value );
  if ( !tail ) {
    head = tail = node;
  }
  else {
    tail->next = node;
    node->prev = tail;
    tail = node;
  }
  ++length;
}


//---------------------------------------------
// Удалить элемент с конца
//---------------------------------------------
template <typename T>
inline void ListContainer<T>::pop_back()
{
  if ( !tail )
    throw std::out_of_range( "" );

  Node * to_delete = tail;
  tail = tail->prev;

  if ( tail )
    tail->next = nullptr;
  else
    head = nullptr;

  delete to_delete;
  --length;
}


//---------------------------------------------
// Удалить элемент по индексу
//---------------------------------------------
template <typename T>
inline void ListContainer<T>::erase( size_t index )
{
  if ( index >= length )
    throw std::out_of_range( "" );

  size_t currentInd = 0;
  Node * current = head;
  while ( currentInd != index ) {
    current = current->next;
    ++currentInd;
  }

  Node * prevTmp = current->prev;
  Node * nextTmp = current->next;

  if ( prevTmp )
    prevTmp->next = nextTmp;
  else
    head = nextTmp;

  if ( nextTmp )
    nextTmp->prev = prevTmp;
  else
    tail = prevTmp;

  delete current;
  --length;
}


//---------------------------------------------
// Очистить список
//---------------------------------------------
template <typename T>
inline void ListContainer<T>::clear()
{
  Node * current = head;
  while ( current ) {
    Node * next = current->next;
    delete current;
    current = next;
  }
  head = tail = nullptr;
  length = 0;
}


//---------------------------------------------
// Вернуть количество элементов
//---------------------------------------------
template <typename T>
inline size_t ListContainer<T>::size() const
{
  return length;
}


//---------------------------------------------
// Получить итератор на начало
//---------------------------------------------
template <typename T>
inline typename ListContainer<T>::Iterator ListContainer<T>::begin()
{
  return Iterator( head );
}


//---------------------------------------------
// Получить итератор на конец
//---------------------------------------------
template <typename T>
inline typename ListContainer<T>::Iterator ListContainer<T>::end()
{
  return Iterator( nullptr );
}


//---------------------------------------------
// Удалить диапазон итераторов
//---------------------------------------------
template <typename T>
inline typename ListContainer<T>::Iterator ListContainer<T>::erase( Iterator first, Iterator last )
{
  Node * startNode = first.getNode();
  Node * endNode = last.getNode();

  while ( startNode != endNode ) {
    Node * next = startNode->next;

    if ( startNode->prev )
      startNode->prev->next = startNode->next;
    else
      head = startNode->next;

    if ( startNode->next )
      startNode->next->prev = startNode->prev;
    else
      tail = startNode->prev;

    delete startNode;
    --length;
    startNode = next;
  }

  return Iterator( endNode );
}


//---------------------------------------------
// Удалить один элемент по итератору
//---------------------------------------------
template <typename T>
inline typename ListContainer<T>::Iterator ListContainer<T>::erase( Iterator pos )
{
  Node * curr = pos.getNode();
  if ( !curr )
    throw std::out_of_range( "" );

  Node * next = curr->next;

  if ( curr->prev )
    curr->prev->next = curr->next;
  else
    head = curr->next;

  if ( curr->next )
    curr->next->prev = curr->prev;
  else
    tail = curr->prev;

  delete curr;
  --length;

  return Iterator( next );
}


//---------------------------------------------
// Вставить элемент перед позицией
//---------------------------------------------
template <typename T>
inline typename ListContainer<T>::Iterator ListContainer<T>::insert( Iterator pos, const T & value )
{
  Node * curr = pos.getNode();
  Node * new_node = new Node( value );

  if ( !curr ) {
    new_node->prev = tail;
    if ( tail )
      tail->next = new_node;
    else
      head = new_node;
    tail = new_node;
  }
  else {
    new_node->next = curr;
    new_node->prev = curr->prev;
    if ( curr->prev )
      curr->prev->next = new_node;
    else
      head = new_node;
    curr->prev = new_node;
  }

  ++length;
  return Iterator( new_node );
}
