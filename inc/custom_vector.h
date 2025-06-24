#pragma once
#include <iterator>
#include <stdexcept>

//------------------------------------
// Контейнер - вектор, аналог std::vector, 
// реализовано через обычный массив
//------------------------------------
template<class T>
class CArray
{
  T * _data;         // Указатель на первый элемент
  size_t _capacity; // Вместимость, для резервирования
  size_t _length;   // Кол-во элементов в массиве

public:
  class Iterator
  {
    T * ptr;

  public:
    Iterator( T * p ) : ptr( p ) {}
    T * getPtr() const { return ptr; }
    // Совместимость с std::iterator_traits
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    reference operator*() const { return *ptr; }
    Iterator & operator++() { ++ptr; return *this; }
    Iterator operator++( int ) { Iterator tmp = *this; ++( *this ); return tmp; }
    Iterator & operator--() { --ptr; return *this; }
    Iterator operator--( int ) { Iterator tmp = *this; --( *this ); return tmp; }
    Iterator & operator+=( difference_type n ) { ptr += n; return *this; }
    Iterator operator+( difference_type n ) const { return Iterator( ptr + n ); }
    Iterator & operator-=( difference_type n ) { ptr -= n; return *this; }
    Iterator operator-( difference_type n ) const { return Iterator( ptr - n ); }
    difference_type operator-( const Iterator & other ) const { return ptr - other.ptr; }
    bool operator!=( const Iterator & other ) const { return ptr != other.ptr; }
    bool operator==( const Iterator & other ) const { return ptr == other.ptr; }
    bool operator<( const Iterator & other ) const { return ptr < other.ptr; }
    bool operator>( const Iterator & other ) const { return ptr > other.ptr; }
  };

  CArray() : _data( nullptr ), _capacity( 0 ), _length( 0 ) {}
  CArray( size_t capacity ) : CArray() { _capacity = capacity; }
  ~CArray() { delete[] _data; }

  void push_back( const T & value );         // Добавить эл-т в конец
  void pop_back();                           // Убрать/удалить эл-т с конца
  size_t size() const { return _length; }    // Дать кол-во элементов
  void clear() { _length = 0; }              // Очитстить массив, оставить резервирование
  void erase( size_t index );                // Удалить эл-т по индексу

  // Операторы получения по индексу
  T & operator[]( size_t index );
  const T & operator[]( size_t index ) const;

  // Операции с итератором
  Iterator begin() { return Iterator( _data ); }
  Iterator end() { return Iterator( _data + _length ); }
  Iterator erase( Iterator first, Iterator last );
  Iterator erase( Iterator pos );
  Iterator insert( Iterator pos, const T & value );

private:
  void resize( size_t capacity );
};


//---------------------------------------------
// Добавить эл-т в конец
//---------------------------------------------
template <typename T>
inline void CArray<T>::push_back( const T & value )
{
  if ( _length == _capacity )
    resize( _capacity == 0 ? 4 : _capacity * 2 );
  _data[_length++] = value;
}


//---------------------------------------------
// Удалить эл-т с конца
//---------------------------------------------
template <typename T>
inline void CArray<T>::pop_back()
{
  if ( _length == 0 )
    throw std::out_of_range( "" );
  _data[_length - 1].~T();
  --_length;
}


//---------------------------------------------
// Выдать элемент по индексу
//---------------------------------------------
template <typename T>
inline T & CArray<T>::operator[]( size_t index )
{
  if ( index >= _length )
    throw std::out_of_range( "" );
  return _data[index];
}


//---------------------------------------------
// Выдать элемент по индексу (const)
//---------------------------------------------
template <typename T>
inline const T & CArray<T>::operator[]( size_t index ) const
{
  if ( index >= _length )
    throw std::out_of_range( "" );
  return _data[index];
}


//---------------------------------------------
// Удалить элемент по индексу
//---------------------------------------------
template <typename T>
inline void CArray<T>::erase( size_t index )
{
  if ( index >= _length )
    throw std::out_of_range( "" );
  _data[index].~T();
  for ( size_t i = index; i < _length - 1; ++i )
    _data[i] = _data[i + 1];
  --_length;
}


//---------------------------------------------
// Удалить диапазон элементов
//---------------------------------------------
template <typename T>
inline typename CArray<T>::Iterator CArray<T>::erase( Iterator first, Iterator last )
{
  T * p1 = first.getPtr();
  T * p2 = last.getPtr();

  if ( p1 < _data || p2 > _data + _length || p1 > p2 )
    throw std::out_of_range( "" );

  size_t start = p1 - _data;
  size_t end = p2 - _data;
  size_t count = end - start;

  for ( size_t i = start; i < end; ++i )
    _data[i].~T();

  for ( size_t i = end; i < _length; ++i )
    _data[i - count] = _data[i];

  _length -= count;
  return Iterator( _data + start );
}


//---------------------------------------------
// Удалить один элемент по итератору
//---------------------------------------------
template <typename T>
inline typename CArray<T>::Iterator CArray<T>::erase( Iterator pos )
{
  T * p = pos.getPtr();

  if ( p < _data || p >= _data + _length )
    throw std::out_of_range( "" );

  size_t index = p - _data;
  _data[index].~T();

  for ( size_t i = index; i < _length - 1; ++i )
    _data[i] = _data[i + 1];

  --_length;
  return Iterator( _data + index );
}


//---------------------------------------------
// Вставить элемент по итератору
//---------------------------------------------
template <typename T>
inline typename CArray<T>::Iterator CArray<T>::insert( Iterator pos, const T & value )
{
  T * p = pos.getPtr();

  if ( p < _data || p > _data + _length )
    throw std::out_of_range( "" );

  size_t index = p - _data;

  if ( _length == _capacity )
    resize( _capacity == 0 ? 4 : _capacity * 2 );

  for ( size_t i = _length; i > index; --i )
    _data[i] = _data[i - 1];

  _data[index] = value;
  ++_length;

  return Iterator( _data + index );
}


//---------------------------------------------
// Изменить размер массива
//---------------------------------------------
template <typename T>
inline void CArray<T>::resize( size_t capacity )
{
  T * new_data = new T[capacity];
  for ( size_t i = 0; i < _length; ++i )
    new_data[i] = _data[i];
  delete[] _data;
  _data = new_data;
  _capacity = capacity;
}