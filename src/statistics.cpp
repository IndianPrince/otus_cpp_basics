#include <limits>
#include <cmath>
#include "statistics.h"


//-------------------------------------------------
// Конструктор
//---
Min::Min()
  : m_min( std::numeric_limits<double>::max() )
{}


//-------------------------------------------------
// Конструктор
//---
Max::Max()
  : m_max( std::numeric_limits<double>::min() )
{}


//-------------------------------------------------
// Конструктор
//---
Mean::Mean()
  : m_mean ( 0.0 )
  , m_count( 0 )
{}


//-------------------------------------------------
// Обновить значение среднего
//---
void Mean::update( double next ) 
{
  double prevSum = m_mean * m_count;
  m_count++;
  m_mean = ( next + prevSum ) / m_count; // m_count не может быть 0;
}


//-------------------------------------------------
// Обновить данные
//---
void StdDev::update( double next )
{
  m_nums.push_back( next );
  Mean::update( next );
}


//-------------------------------------------------
// Рассчитать стандартное отклонение
//---
double StdDev::eval() const {
  double disp( 0.0 );

  for ( double num : m_nums ) {
    double diff = num - m_mean;
    disp += ( diff * diff ) / m_count;
  }

  return sqrt( disp );
}