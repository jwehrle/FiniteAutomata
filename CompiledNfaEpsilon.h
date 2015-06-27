//------------------------------------------------------------------------------
// CompiledNfaEpsilon.h
// John Wehrle
// 15 March 2015
// Matches input strings based on a FiniteStateMachine in NFA format
// or translates a FiniteStateMachine in NFA format into a
// FiniteStateMachine in DFA format.
//------------------------------------------------------------------------------
#ifndef COMPILEDNFAEPSILON_H
#define COMPILEDNFAEPSILON_H
#include <iostream>
#include <unordered_set>
#include "FiniteStateMachine.h"
#include <list>
#include <queue>

using namespace std;

//------------------------------------------------------------------------------
// CompiledNfaEpsilon Class
// Matches input strings based on a FiniteStateMachine in NFA format
// or translates a FiniteStateMachine in NFA format into a
// FiniteStateMachine in DFA format.
// No defaul constructor, instead can only be constructed with a
// FiniteStateMachine - should be formatted for NFA-EPSILON but there is no
// validation that the FiniteStateMachine is in NFA-EPSILON format.
// Only two pbulic methods:
//    checkString()
//    translateToDFA()
// Many private helper functions:
//    isMatch()
//    buildDestinationSetWithTran()
//    setUnion()
//    makeNewDfaTransition()
//    depthFirstSearchEpsilon()
//    fillEpsilonClosure()
//    fillDestSetFromSrcSet()
//    fillDestSetFromTransitions()
//    checkIfSetContainsGoalNode()
//    makeLanguage()
//    isGoalNode()
//    translateTransition()
//    makeTranslation()
//    depthFirstSearchEpsilon()
//    initializeTranslator()
//    initializeDestinationSet()
// Members
//    start
//    goalNodes
//    transitions
//    translator
//       nfae
//       dfa
//       nodeMapQueue
//          unmappedNodeSets
//          unmappedNodes
//------------------------------------------------------------------------------
class CompiledNfaEpsilon
{
   public:
      //Constructor
      CompiledNfaEpsilon(FiniteStateMachine fsm)
      : start(fsm.startNode), goalNodes(fsm.goalNodes),
      transitions(fsm.transitions), fsmNFA(fsm) {}

      //Destructor - key word 'new' was not used in this class.
      ~CompiledNfaEpsilon(){}

   //public methods
      //Returns isMatch() bool value
      inline bool checkString(string inputString)
         {return isMatch(inputString, 0, start);}

      //Translates an NFAE passed as an argument
      FiniteStateMachine translateToDFA(FiniteStateMachine nfae);

      //Translates the member NFAE
      FiniteStateMachine translateToDFA(void);

   private:
      CompiledNfaEpsilon();   //No public default constructor

   //private members
      int start;                       //Start Node
      unordered_set<int> goalNodes;    //Goal Nodes
      list<Transition> transitions;    //List of Transitions
      FiniteStateMachine fsmNFA;       //NFAE FiniteStateMachine
//------------------------------------------------------------------------------
// struct NodeMappingQueue
// Helper struct to associate queues of sets of nodes with queues of nodes.
// Contains helper methods:
//    push()  - pushes arguments onto both queues
//    pop() - pops front on both queues
//    empty()  - returns true if either queue is empty, false otherwise.
//------------------------------------------------------------------------------
      struct NodeMappingQueue
      {
         queue<unordered_set<int>> unmappedNodeSets;
         queue<int> unmappedNodes;
         inline void push(unordered_set<int>& nodeSet, int& node)
            {unmappedNodeSets.push(nodeSet); unmappedNodes.push(node);}
         inline void pop(void)
            {unmappedNodeSets.pop(); unmappedNodes.pop();}
         inline bool empty(void) {return unmappedNodeSets.empty() ||
            unmappedNodes.empty();}
      };

//------------------------------------------------------------------------------
// struct Translator
// helper struct to associate a NFA-EPSILON and DFA FiniteStateMachines.
// Contains a NodeMappingQueue nodeMapQueue and two FiniteStateMachine;
// nfae and dfa.
//------------------------------------------------------------------------------
      struct Translator
      {
         NodeMappingQueue nodeMapQueue;
         FiniteStateMachine nfae;
         FiniteStateMachine dfa;
      };

      Translator translator;

   //private metods
   //Extended documentation for these methods contained in CompiledNfaEpsilon.h

      //Checks whether inputString matches the FiniteStateMachine
      bool isMatch(string inputString, unsigned posInString, int curState);

      //Fills destination node set with destination nodes
      void buildDestinationSetWithTran(unordered_set<int>& destSet, char& symbol);

      //Returns the union of setA and setB
      unordered_set<int>& setUnion(unordered_set<int>& setA, unordered_set<int>& setB);

      //Creates a DFA Transition and adds it to the dfa
      void makeNewDfaTransition(char& symbol, unordered_set<int> destSet, int& dest);

      //fills the epsilcon closure via a depth first search
      void depthFirstSearchEpsilon(list<Transition>& transitions, int& curDest,
         unordered_set<int>& closure);

      //Calls depthFirstSearchEpsilon
      void fillEpsilonClosure(int& curState, unordered_set<int>& closure);

      //Fills destination node set from source node set and transition char
      void fillDestSetFromSrcSet(unordered_set<int>& sourceNodeSet, char& inputChar,
         unordered_set<int>& destinationNodeSet);

      //Fills destination node set from Transition list and transition char
      void fillDestSetFromTransitions(int& curState, char& inputChar,
         unordered_set<int>& destinationNodeSet);

      //Returns true of nodeSet contains a goal node, false otherwise
      bool checkIfSetContainsGoalNode(unordered_set<int>& nodeSet);

      //Fills a char set with all unique char in nfae
      unordered_set<char> makeLanguage(FiniteStateMachine& nfae);

      //Returns true if destinationSet contains a goal node, false otehrwise
      bool isGoalNode(unordered_set<int>& destinationSet, FiniteStateMachine& nfae);

      //Translates a set of nfae Transitions to one dfa Transition
      void translateTransition(int& dest, char& symbol);

      //Calls helper functions to create a dfa translation
      void makeTranslation(unordered_set<char>& language);

      //Another version of a depth first search for filling the epsilon closure
      void depthFirstSearchEpsilon(int& node, unordered_set<int>& closure);

      //Initializes the Translator
      void initializeTranslator(FiniteStateMachine nfae);

      //Fills the destination set
      unordered_set<int> initializeDestinationSet(char& inputChar, int& curState);
};

#endif // COMPILEDNFAEPSILON_H
