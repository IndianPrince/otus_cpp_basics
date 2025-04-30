#include <iostream>
#include <random>
#include <ctime>
#include <string>
#include "table.h"


//-----------------------------------------------------------------------
// Check max value for a guess a number game.
//---
bool IsValidMaxVal( const int max ){ return max > 0; }


//-----------------------------------------------------------------------
// Read value from input, check if int and reiterate if not.
//---
void ReadIntInput( int & val )
{
  bool valid = true;
  do {
    std::cin >> val;
    valid = true;
    if ( std::cin.fail() || std::cin.get() != '\n' ) {
      std::cout << "Error. Please enter an integer value" << std::endl;
      std::cin.clear();
      std::cin.ignore( 256, '\n' );
      valid = false;
    }
    else {
      valid = true;
    }
  } while ( valid == false );
}


//-----------------------------------------------------------------------
// Guess a number game loop.
//---
int GameLoop( const int max )
{
  std::srand( static_cast<unsigned int>( std::time( nullptr ) ) );
  int targetVal = std::rand() % ( max + 1 );
  int currentGuess( -1 ), result( 0 );

  while ( targetVal != currentGuess ) {
    std::cout << "Enter your guess:" << std::endl;
    ReadIntInput( currentGuess );
    result++;
    if ( currentGuess > targetVal )
      std::cout << " less than " << currentGuess << std::endl;
    else if ( currentGuess < targetVal )
      std::cout << " grater than " << currentGuess << std::endl;
  }

  std::cout << "You win! Attempts : = " << result << std::endl;

  return result;
}

//-----------------------------------------------------------------------
// 
//---
int main( int argc, char * argv[] )
{
  const std::string resultsFilePath = "high_scores.txt";
  const int defaultMax( 100 );
  int maxVal( defaultMax );
  NameScorePairs table; // score table
  bool startLoop = true;

  // Expected arguments are -table and -max "int value"
  // -max "value" should be divided by space " "
  if ( argc == 2 ) {
    std::string arg = std::string( argv[1] );
    if ( arg == "-table" ) {
      startLoop = false;
      ReadTable( resultsFilePath, table );
      PrintTable( table );
    }
    else {
      // Split line, find max value if -max
      if ( !arg.empty() ) { 
        std::string val1, val2;
        int tmpVal( 0 );
        auto delIt = std::find( arg.begin(), arg.end(), ' ');
        if ( delIt != arg.end() ) {
          val1.insert( val1.end(), arg.begin(), delIt );
          if ( val1 == "-max" ) {
            val2.insert( val2.end(), delIt + 1, arg.end() );
            tmpVal = std::stoi( val2 );
          }
        }
        if ( IsValidMaxVal( tmpVal ) )
          maxVal = tmpVal;
      }
    }
  }

  if ( startLoop ) {
    std::cout << "Hi! Enter your name, please:" << std::endl;
    std::string name;
    std::cin >> name;
    std::cout << argc << std::endl;
    int result = GameLoop( maxVal );
    ReadTable( resultsFilePath, table );
    AddResult( name, result, table );
    WriteTable( resultsFilePath, table );
    PrintTable( table );
  }

  return 0;
}