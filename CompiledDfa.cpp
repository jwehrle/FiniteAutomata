//------------------------------------------------------------------------------
// CompiledDFA.h
// John Wehrle
// 15 March 2015
// Implementation for CompiledDfa.h
// Matches input strings based on a FiniteStateMachine in DFA format
// Contains implementation for:
//    Constructor
//    isMatch()
//------------------------------------------------------------------------------
#include "CompiledDfa.h"

//------------------------------------------------------------------------------
// Constructs a CompiledDfa based on a FiniteStateMachine formatted for DFA.
// Key/value pairs must be made for the stateTransitionMap otherwise every
// member is initialized in the initialization list.
//------------------------------------------------------------------------------
CompiledDfa::CompiledDfa(FiniteStateMachine finStMch)
: start(finStMch.startNode), key(""), goalNodes(finStMch.goalNodes)
{
   for(Transition transition : finStMch.transitions)
   {
      key = to_string(transition.source) + transition.transitionChar;
      stateTransitionMap.emplace(key, transition.destination);
   }
}

//------------------------------------------------------------------------------
// Recursive function for comparing an inputString against the DFA.
// Base cases:
//    1. key is not in stateTransitionMap
//    2. posInString is at the end of inputString - check whether curState
//    is a goal node.
//------------------------------------------------------------------------------
bool CompiledDfa::isMatch(string inputString, unsigned posInString, int curState)
{
   key = to_string(curState) + inputString[posInString];

   auto getNextState = stateTransitionMap.find(key);
   if(getNextState == stateTransitionMap.end())
   {
      return false;
   }

   if(posInString == inputString.length() - 1)
   {
      auto node = goalNodes.find(getNextState->second);
      return (node == goalNodes.end()) ? false : true;
   }
   else
   {
      return isMatch(inputString, posInString + 1, getNextState->second);
   }
}





















