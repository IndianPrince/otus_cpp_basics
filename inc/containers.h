#pragma once

template<class T>
class CArray
{
private:
  T * _data;
  size_t _capacity;
  size_t _length;

public:

  // Итератор
  class Iterator
  {
  private:
    T * ptr;
  public:
  // Для совместимости
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = T;
    using difference_type   = std::ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;

    Iterator( T * p ) : ptr( p ) {}
    T * getPtr() const { return ptr; }

    // Для совместимости
    reference operator*() const { return *ptr; }
    Iterator& operator++() { ++ptr; return *this; }
    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
    Iterator& operator--() { --ptr; return *this; }
    Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

    Iterator& operator+=(difference_type n) { ptr += n; return *this; }
    Iterator operator+(difference_type n) const { return Iterator(ptr + n); }

    Iterator& operator-=(difference_type n) { ptr -= n; return *this; }
    Iterator operator-(difference_type n) const { return Iterator(ptr - n); }

    difference_type operator-(const Iterator& other) const { return ptr - other.ptr; }

    bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    bool operator==(const Iterator& other) const { return ptr == other.ptr; }
    bool operator<(const Iterator& other) const { return ptr < other.ptr; }
    bool operator>(const Iterator& other) const { return ptr > other.ptr; }
  };
  // Конструктор по умолчанию.
  CArray() : _data( nullptr ), _capacity( 0 ), _length( 0 ) {}
  // Конструктор с резервированием.
  CArray( size_t capacity ) : CArray() { _capacity = capacity };
  // Деструктор
  ~CArray() { delete[] _data; }

  // Записать элемент в конец
  void push_back( const T & value )
  {
    if ( _length == _capacity )
      resize( _capacity == 0 ? 4 : _capacity * 2 );
    _data[_length++] = value;
  }

  // Выдать уопию элемента с конца и удалить его
  T pop_back()
  {
    if ( _length == 0 )
      throw std::out_of_range();

    T value = _data[_length - 1];
    _data[_length - 1].~T();
    --_length;
    return value;
  }

  // Выдать элемент по индексу
  T & operator[]( size_t index )
  {
    if ( index >= _length ) 
      throw std::out_of_range();

    return _data[index];
  }

  // Выдать элемент по индексу
  const T & operator[]( size_t index ) const
  {
    if ( index >= _length ) 
      throw std::out_of_range();

    return _data[index];
  }

    // Выдать количество элементов массива
  size_t size() const { return _length; }

  // Очистить массив, оставить резервирование
  void clear() { _length = 0; }

  // Удалить элемент по индексу
  void erase( size_t index )
  {
    if ( index >= length )
      throw std::out_of_range();

    _data[index].~T();

    for ( size_t i = index; i < length - 1; ++i ) {
      _data[i] = _data[i + 1];
    }

    --length;
  }

  // Итератор операции
  CArray::Iterator begin() { return CArray::Iterator( _data ); }
  CArray::Iterator end() { return CArray::Iterator( _data + _length ); }
  CArray::Iterator erase( CArray::Iterator first, CArray::Iterator last )
  {
    T * p1 = first.getPtr();
    T * p2 = last.getPtr();

    if ( p1 < _data || p2 > _data + length || p1 > p2 )
      throw std::out_of_range();

    size_t start = p1 - _data;
    size_t end = p2 - _data;
    size_t count = end - start;

    for ( size_t i = start; i < end; ++i )
      _data[i].~T();

    for ( size_t i = end; i < length; ++i )
      _data[i - count] = _data[i];

    length -= count;
    return Iterator( _data + start );
  }

  CArray::Iterator erase( CArray::Iterator pos )
  {
    T * p = pos.getPtr();

    if ( p < _data || p >= _data + _length )
      throw std::out_of_range();

    size_t index = p - _data;

    _data[index].~T();

    for ( size_t i = index; i < _length - 1; ++i )
      _data[i] = _data[i + 1];

    --_length;

    return Iterator( _data + index );
  }

  CArray::Iterator insert( CArray::Iterator pos, const T & value )
  {
    T * p = pos.getPtr();

    if ( p < _data || p > _data + _length )
      throw std::out_of_range();

    size_t index = p - _data;

    if ( _length == _capacity )
      resize( _capacity == 0 ? 4 : _capacity * 2 );

    for ( size_t i = _length; i > index; --i )
      _data[i] = _data[i - 1];

    _data[index] = value;
    ++_length;

    return CArray::Iterator( _data );
  }

private:
  void resize( size_t capacity )
  {
    T * new_data = new T[capacity];

    for ( size_t i = 0; i < _length; ++i )
      new_data[i] = _data[i];

    delete[] _data;
    _data = new_data;
    _capacity = capacity;
  }
};



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
  private:
    Node * node;
  public:
    Iterator( Node * n ) : node( n ) {}

    T & operator*() const { return node->value; }

    Iterator & operator++()
    {
      if ( node ) 
        node = node->next;

      return *this;
    }

    Iterator operator++( int )
    {
      Iterator tmp = *this;
      ++( *this );

      return tmp;
    }

    Iterator & operator--()
    {
      if ( node ) 
        node = node->prev;

      return *this;
    }

    bool operator!=( const Iterator & other ) const { return node != other.node; }

    bool operator==( const Iterator & other ) const { return node == other.node; }

    Node * getNode() const { return node; }
  };

  ListContainer() : head( nullptr ), tail( nullptr ), length( 0 ) {}
  ~ListContainer() { clear(); }

  void push_back( const T & value )
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

  void pop_back()
  {
    if ( !tail ) 
      throw std::out_of_range();

    Node * to_delete = tail;
    tail = tail->prev;

    if ( tail ) 
      tail->next = nullptr;
    else 
      head = nullptr;

    delete to_delete;
    --length;
  }

  void clear()
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

  size_t size() const
  {
    return length;
  }

  // Итератор операции
  Iterator begin() { return Iterator( head ); }
  Iterator end() { return Iterator( nullptr ); }
  Iterator erase( Iterator first, Iterator last )
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
  Iterator insert( Iterator pos, const T & value )
  {
    Node * curr = pos.getNode();
    Node * new_node = new Node( value );

    if ( !curr ) {
      new_node->prev = tail;
      if ( tail ) tail->next = new_node;
      else head = new_node;
      tail = new_node;
    }
    else {
      new_node->next = curr;
      new_node->prev = curr->prev;
      if ( curr->prev ) curr->prev->next = new_node;
      else head = new_node;
      curr->prev = new_node;
    }

    ++length;
    return Iterator( new_node );
  }
};