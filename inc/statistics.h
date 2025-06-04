#pragma once
#include <vector>

class IStatistics
{
public:
  virtual ~IStatistics() {}

  virtual void update( double next ) = 0;
  virtual double eval() const = 0;
  virtual const char * name() const = 0;
}; // IStatistics


class Min : public IStatistics
{
  double m_min; // текущее мин значение

public:
  Min();

  void update( double next ) override
  {
    if ( next < m_min )
      m_min = next;
  }

  double eval() const override { return m_min; }

  const char * name() const override { return "min"; }
}; // Min


class Max : public IStatistics
{
  double m_max; // текущее макс значение

public:
  Max();

  void update( double next ) override
  {
    if ( next > m_max )
      m_max = next;
  }

  double eval() const override { return m_max; }

  const char * name() const override { return "max"; }

}; // Max


class Mean : public IStatistics
{
protected:
  double m_mean; // текущее среднее значение
  size_t m_count;  // кол-во значений

public:
  Mean();

  void update( double next ) override;

  double eval() const override { return m_mean; }

  const char * name() const override { return "mean"; }

}; // Mean


class StdDev : public Mean
{
  double m_stdDev;
  std::vector<double> m_nums;

public:
  StdDev();

  void update( double next ) override;

  double eval() const override { return m_stdDev; }

  const char * name() const override { return "StdDev"; }

}; // Mean