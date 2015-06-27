//------------------------------------------------------------------------------
// FiniteStateMachine.h
// Implemented by John Wehrle based on Instructions by Mickey Phoenix
// 15 March 2015
// Struct for implementing both determined finite automata and non-determined
// finite state automata.
//------------------------------------------------------------------------------
#ifndef FINITESTATEMACHINE_H_INCLUDED
#define FINITESTATEMACHINE_H_INCLUDED
#include <unordered_set>
#include <list>

using namespace std;

const char EPSILON = '\x0';   //used for epsilon transitions

//------------------------------------------------------------------------------
// Stores basic information about a transition.
// Contains constructors and an assignment operator overload - added by John
// Wehrle for convenience in the CompiledNfaEpsilon class.
//------------------------------------------------------------------------------
struct Transition
{
   int source;
   char transitionChar;
   int destination;
   Transition() :  source(0), transitionChar('0'), destination(0) {}
   Transition(int from, char with, int to) :
      source(from), transitionChar(with), destination(to) {}
   Transition& operator=(const Transition& other)
      {source = other.source; transitionChar = other.transitionChar;
       destination = other.destination; return *this;}
};

//------------------------------------------------------------------------------
// Stores basic parts of either a determined finite automata and non-determined
// finite state automata.
//------------------------------------------------------------------------------
struct FiniteStateMachine
{
   unordered_set<int> nodes;
   int startNode;
   unordered_set<int> goalNodes;
   list<Transition> transitions;
};


#endif // FINITESTATEMACHINE_H_INCLUDED
