#include "table.h"
#include <iostream>
#include <fstream>
#include <algorithm>


//-----------------------------------------------------------------------
// Read table from a file.
//---
void ReadTable( const std::string path, NameScorePairs & table )
{
  table.clear();

  std::ifstream inStream{ path };

  if ( !inStream.is_open() ) {
    std::cout << "Could not open file: " << path << "!" << std::endl;
    return;
  }

  std::string line;

  while ( std::getline( inStream, line ) ) {
    std::string name;
    std::string scoreStr;
    int score( 0 );
    if ( !line.empty() ) {
      auto delIt = std::find( line.begin(), line.end(), '\t' );
      if ( delIt != line.end() ) {
        name.insert( name.end(), line.begin(), delIt );
        scoreStr.insert( scoreStr.end(), delIt + 1, line.end() );
        score = std::stoi( scoreStr );
        table.emplace_back( name, score );
      }
    }
  }
}


//-----------------------------------------------------------------------
// Append result to a table. Checks for duplicates.
//---
void AddResult( const std::string name, const int score, NameScorePairs & table )
{
  auto it = std::find_if( table.begin(), table.end(), [name]( std::pair<std::string, int> & val ) { return name == val.first; } );

  if ( it == table.end() ) {
    table.emplace_back( name, score );
  }
  else if ( it->second > score )
    it->second = score;
}


//-----------------------------------------------------------------------
// Write score table to a file
//---
void WriteTable( std::string path, const NameScorePairs & table )
{
  std::ofstream out{ path };

  if ( !out.is_open() ) {
    std::cout << "Could not open file: " << path << "!" << std::endl;
    return;
  }

  for ( const auto & entry : table ) {
    out << entry.first << '\t';
    out << entry.second;
    out << std::endl;
  }

  out.close();
}


//-----------------------------------------------------------------------
// Print score table to console
//---
void PrintTable( NameScorePairs & table )
{
  std::cout << "Scores table:" << std::endl;

  std::sort( table.begin(), table.end(),
            []( const std::pair<std::string, int> & val1, const std::pair<std::string, int> & val2 )
              { return val1.second < val2.second; } );

  for ( auto & entry : table )
    std::cout << entry.first << '\t' << entry.second << std::endl;
}