//------------------------------------------------------------------------------
// CompiledDFA.h
// John Wehrle
// 15 March 2015
// Matches input strings based on a FiniteStateMachine in DFA format
//------------------------------------------------------------------------------
#ifndef COMPILEDDFA_H
#define COMPILEDDFA_H
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include "FiniteStateMachine.h"

using namespace std;
//------------------------------------------------------------------------------
// CompiledDFA Class
// Matches input strings based on a FiniteStateMachine in DFA format
// No defaul constructor, instead can only be constructed with a
// FiniteStateMachine - should be formatted for DFA but there is no
// validation that the FiniteStateMachine is in DFA format.
// Only one public method:
//    checkString()
// Only one private method:
//    isMatch()
// Only four members:
//    start
//    key
//    stateTransitionMap
//    goalNodes
//------------------------------------------------------------------------------
class CompiledDfa
{
   public:
      //Constructor
      CompiledDfa(FiniteStateMachine finStMch);

      //Destructor - key word 'new' is not used.
      ~CompiledDfa(){}

      //Returns value of isMatch()
      inline bool checkString(string inputString)
         {return isMatch(inputString, 0, start);}

   private:
      CompiledDfa(); //no default constructor

      int start;           //Start Node
      string key;          //Key for stateTransitionMap
      //Map of transitions
      unordered_map<string, int> stateTransitionMap;
      //set of goal nodes
      unordered_set<int> goalNodes;

      //Returns true of inputString matches the DFA, false otherwise
      bool isMatch(string inputString, unsigned posInString, int curState);
};

#endif // COMPILEDDFA_H
