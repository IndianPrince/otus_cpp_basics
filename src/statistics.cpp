#include <limits>
#include <cmath>
#include "statistics.h"

constexpr double NULL_EPSILON = 1E-30;

//-------------------------------------------------
// Конструктор
//---
Min::Min()
  : IStatistics()
  , m_min( std::numeric_limits<double>::max() )
{
}

//-------------------------------------------------
// Конструктор
//---
Max::Max()
  : IStatistics()
  , m_max( std::numeric_limits<double>::min() )
{
}

//-------------------------------------------------
// Конструктор
//---
Mean::Mean()
  : IStatistics()
  , m_mean ( 0.0 )
  , m_count( 0 )
{
}

void Mean::update( double next ) 
{
  double prevSum = m_mean * m_count;
  m_count++;
  m_mean = ( next + prevSum ) / m_count; // m_count не может быть 0;
}

//-------------------------------------------------
// Конструктор
//---
StdDev::StdDev()
  : Mean()
  , m_stdDev( 0.0 )
  , m_nums()
{
}

void StdDev::update( double next )
{
  m_nums.push_back( next );
  Mean::update( next );
  double d( 0.0 );
  for ( double num : m_nums ) {
    double diff = num - m_mean;
    d += ( diff * diff ) / m_count;
  }
  
  m_stdDev = sqrt( d );
}