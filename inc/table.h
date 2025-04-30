#pragma once

#include<vector>
#include<string>

typedef std::vector<std::pair<std::string, int>> NameScorePairs; // Results table

// Results table operations
void ReadTable( const std::string path, NameScorePairs & table );
void AddResult( const std::string name, const int score, NameScorePairs & table );
void WriteTable( std::string path, const NameScorePairs & table );
void PrintTable( NameScorePairs & table );