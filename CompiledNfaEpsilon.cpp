//------------------------------------------------------------------------------
// CompiledNfaEpsilon.cpp
// John Wehrle
// 15 March 2015
// Implementation for CompiledNfaEpsilon.h
// Matches input strings based on a FiniteStateMachine in NFA format
// or translates a FiniteStateMachine in NFA format into a
// FiniteStateMachine in DFA format.
// Contains Implementations for:
//    isMatch()
//    initializeDestinationSet() x2
//    translateToDFA()  x2
//    initializeTranslator()
//    depthFirstSearchEpsilon()
//    makeLanguage()
//    buildDestinationSetWithTran()
//    makeTranslation()
//    translateTransition()
//    makeNewDfaTransition()
//    setUnion()
//    isGoalNode()
//    fillEpsilonClosure()
//    fillDestSetFromSrcSet()
//    fillDestSetFromTransitions()
//    checkIfSetContainsGoalNode()
//------------------------------------------------------------------------------
#include "CompiledNfaEpsilon.h"

//------------------------------------------------------------------------------
// isMatch(string inputString, unsigned posInString, int curState)
// Declares and assigns inputChar and destinationNodeSet.
// If posInString is at the end of inputString, return true if
// destinationNodeSet contains a goal node - false otherwise.
// If posInString is not at the end of inputString, check each node in
// destinationNodeSet recursively.  If no node in destinationNodeSet
// is a match, return false.
// Calls:
//    initializeDestinationSet()
//    checkIfSetContainsGoalNode()
//    isMatch()
//------------------------------------------------------------------------------
bool CompiledNfaEpsilon::isMatch(string inputString, unsigned posInString,
   int curState)
{
   char inputChar = inputString[posInString];
   unordered_set<int> destinationNodeSet =
      initializeDestinationSet(inputChar, curState);

   if(posInString == inputString.length() - 1)
   {
      return checkIfSetContainsGoalNode(destinationNodeSet);
   }

   for(int node : destinationNodeSet)
   {
      if(isMatch(inputString, posInString + 1, node))
      {
         return true;
      }
   }
   return false;
}

//------------------------------------------------------------------------------
// initializeDestinationSet(char& inputChar, int& curState)
// Declare and Assign epsilon closure and destinationNodeSet.
// Return destinationNodeSet
// Calls:
//    fillEpsilonClosure()
//    fillDestSetFromSrcSet()
//    fillDestSetFromTransitions()
//------------------------------------------------------------------------------
unordered_set<int> CompiledNfaEpsilon::initializeDestinationSet(char& inputChar,
   int& curState)
{
   unordered_set<int> epsilonClosure;
   fillEpsilonClosure(curState, epsilonClosure);
   unordered_set<int> destinationNodeSet;
   fillDestSetFromSrcSet(epsilonClosure, inputChar, destinationNodeSet);
   fillDestSetFromTransitions(curState, inputChar, destinationNodeSet);
   return destinationNodeSet;
}

//------------------------------------------------------------------------------
// depthFirstSearchEpsilon(list<Transition>& transitions, int& node,
// unordered_set<int>& closure)
// Conducts a depth first search in the transitions list for nods reachable
// with on epsilon.
// NB: This version of the function does NOT use the member translator.
// Calls:
//    depthFirstSearchEpsilon()
//------------------------------------------------------------------------------
void CompiledNfaEpsilon::depthFirstSearchEpsilon(list<Transition>& transitions,
   int& node, unordered_set<int>& closure)
{
   for(auto tran : transitions)
   {
      if(tran.source == node && tran.transitionChar == EPSILON)
      {
         closure.emplace(tran.destination);
         depthFirstSearchEpsilon(transitions, tran.destination, closure);
      }
   }
}

//------------------------------------------------------------------------------
// depthFirstSearchEpsilon(int& node, unordered_set<int>& closure)
// Conducts a depth first search in the transitions list for nods reachable
// with on epsilon.
// NB: This version of the function DOES use the member translator.
// Calls:
//    depthFirstSearchEpsilon()
//------------------------------------------------------------------------------
void CompiledNfaEpsilon::depthFirstSearchEpsilon(int& node,
   unordered_set<int>& closure)
{
   for(Transition tran : translator.nfae.transitions)
   {
      if(tran.source == node && tran.transitionChar == EPSILON)
      {
         closure.emplace(tran.destination);
         depthFirstSearchEpsilon(tran.destination, closure);
      }
   }
}

//------------------------------------------------------------------------------
// translateToDFA(FiniteStateMachine nfae)
// // Translates the parameter nfae
// Initializes translator
// Derives language and epsilon closure
// Associates epsilon closure with start node
// Translates the nfae to a dfa
// Returns dfa
// Calls:
//    initializeTranslator()
//    makeLanguage()
//    makeTranslation()
//------------------------------------------------------------------------------
FiniteStateMachine CompiledNfaEpsilon::translateToDFA(FiniteStateMachine nfae)
{
   initializeTranslator(nfae);
   unordered_set<char> language = makeLanguage(nfae);
   makeTranslation(language);
   return translator.dfa;
}

//------------------------------------------------------------------------------
// translateToDFA(void)
// Translates the member nfae
// Initializes translator
// Derives language and epsilon closure
// Associates epsilon closure with start node
// Translates the nfae to a dfa
// Returns dfa
// Calls:
//    initializeTranslator()
//    makeLanguage()
//    makeTranslation()
//------------------------------------------------------------------------------
FiniteStateMachine CompiledNfaEpsilon::translateToDFA(void)
{
   initializeTranslator(fsmNFA);
   unordered_set<char> language = makeLanguage(fsmNFA);
   makeTranslation(language);
   return translator.dfa;
}

//------------------------------------------------------------------------------
// initializeTranslator(FiniteStateMachine nfae)
// Initializes members of translator from nfae.
// Calls:
//    depthFirstSearchEpsilon()
//------------------------------------------------------------------------------
void CompiledNfaEpsilon::initializeTranslator(FiniteStateMachine nfae)
{
   translator.nfae = nfae;
   translator.dfa.startNode = nfae.startNode;
   auto nodeIt = nfae.goalNodes.find(nfae.startNode);
   if(nodeIt != nfae.goalNodes.end())
   {
      translator.dfa.goalNodes.emplace(nfae.startNode);
   }
   translator.dfa.nodes.emplace(nfae.startNode);
   unordered_set<int> epsilonClosure ({nfae.startNode});
   depthFirstSearchEpsilon(nfae.startNode, epsilonClosure);
   translator.nodeMapQueue.push(epsilonClosure, nfae.startNode);
}

//------------------------------------------------------------------------------
// makeLanguage(FiniteStateMachine& nfae)
// Collects all unique transitionChar from nfae and returns the resulting
// unordered_set<char>
//------------------------------------------------------------------------------
unordered_set<char> CompiledNfaEpsilon::makeLanguage(FiniteStateMachine& nfae)
{
   unordered_set<char> tmpLang;
   for(Transition transition : nfae.transitions)
   {
      tmpLang.emplace(transition.transitionChar);
   }
   return tmpLang;
}

//------------------------------------------------------------------------------
// buildDestinationSetWithTran(unordered_set<int>& destSet, char& symbol)
// Conducts a breadth first search for nodes reachable on transitionChar symbol.
//------------------------------------------------------------------------------
void CompiledNfaEpsilon::buildDestinationSetWithTran(unordered_set<int>& destSet,
   char& symbol)
{
   for(auto source : translator.nodeMapQueue.unmappedNodeSets.front())
   {
      for(auto transition : translator.nfae.transitions)
      {
         if(transition.source == source && transition.transitionChar == symbol)
         {
            destSet.emplace(transition.destination);
         }
      }
   }
}

//------------------------------------------------------------------------------
// makeTranslation(unordered_set<char>& language)
// Translates, breadth first, nfae to dfa.
// Calls:
//    translateTransition()
//------------------------------------------------------------------------------
void CompiledNfaEpsilon::makeTranslation(unordered_set<char>& language)
{
   int dest = 0;
   while(!translator.nodeMapQueue.empty())
   {
      for(char symbol : language)
      {
         translateTransition(dest, symbol);
      }
      translator.nodeMapQueue.pop();
   }
}

//------------------------------------------------------------------------------
// translateTransition(int& dest, char& symbol)
// Declares and assigns a set of destination nodes in nfae reachable on
// transitionChar suymbol.  If destSet is not empty but the symbol is EPSILON
// the source set is made to be the union of the source set and the destination
// set.  If symbol is not EPSILON, a new dfa transition is made,  Furthermore,
// if the source and destination sets are dissimilar we check for goal node
// status (and update dfa.goalNodes appropriately), add the new dfa destination
// to dfa.nodes and push the new destination set and destination onto
// nodeMapQueue.
// Calls:
//    buildDestinationSetWithTran()
//    setUnion()
//    makeNewDfaTransition()
//    isGoalNode()
//    nodeMapQueue.push()
//------------------------------------------------------------------------------
void CompiledNfaEpsilon::translateTransition(int& dest, char& symbol)
{
   unordered_set<int> destSet;
   buildDestinationSetWithTran(destSet, symbol);
   if(destSet.size() == 0) { return; }

   if(symbol == EPSILON)
   {
      translator.nodeMapQueue.unmappedNodeSets.front() =
         setUnion(translator.nodeMapQueue.unmappedNodeSets.front(), destSet);
   }
   else
   {
      makeNewDfaTransition(symbol, destSet, dest);
      if(destSet != translator.nodeMapQueue.unmappedNodeSets.front())
      {
         if(isGoalNode(destSet, translator.nfae))
         {
            translator.dfa.goalNodes.emplace(dest);
         }
         translator.dfa.nodes.emplace(dest);
         translator.nodeMapQueue.push(destSet, dest);
      }
   }
}

//------------------------------------------------------------------------------
// makeNewDfaTransition(char& symbol, unordered_set<int> destSet, int& dest)
// Creates a Transition from translator.nodeMapQueue.unmappedNodes.front(),
// symbol and dest. Then we add this Transition to translator.dfa.transitions
// NB: if the source node set is identical to the destination node set we do
// not increment the destination node.
//------------------------------------------------------------------------------
void CompiledNfaEpsilon::makeNewDfaTransition(char& symbol,
   unordered_set<int> destSet, int& dest)
{
   Transition tran;
   tran.source = translator.nodeMapQueue.unmappedNodes.front();
   tran.transitionChar = symbol;
   (translator.nodeMapQueue.unmappedNodeSets.front() == destSet) ?
      tran.destination = translator.nodeMapQueue.unmappedNodes.front() :
      tran.destination = ++dest;
   translator.dfa.transitions.push_back(tran);
}

//------------------------------------------------------------------------------
// setUnion(unordered_set<int>& setA, unordered_set<int>& setB)
// Returns the union of setA and setB.
// NB: a copy of setA is returned so that allocation exceptions are avoided.
//------------------------------------------------------------------------------
unordered_set<int>& CompiledNfaEpsilon::setUnion(unordered_set<int>& setA,
   unordered_set<int>& setB)
{
   unordered_set<int> tmpSetA = setA;
   for(auto i : setB)
   {
      tmpSetA.emplace(i);
   }
   return tmpSetA;
}

//------------------------------------------------------------------------------
// isGoalNode(unordered_set<int>& destinationSet, FiniteStateMachine& nfae)
// Returns true if any node destinationSet is also in nfae.goalNodes, returns
// false otherwise.  In other words, returns true if the intersection between
// these two sets is not empty, false otherwise.
//------------------------------------------------------------------------------
bool CompiledNfaEpsilon::isGoalNode(unordered_set<int>& destinationSet,
   FiniteStateMachine& nfae)
{
   bool isGoal = false;
   for(int node : destinationSet)
   {
      auto nodeIterator = nfae.goalNodes.find(node);
      if(nodeIterator != nfae.goalNodes.end())
      {
         isGoal = true;
         break;
      }
   }
   return isGoal;
}

//------------------------------------------------------------------------------
// fillEpsilonClosure(int& curState, unordered_set<int>& closure)
// Conducts a depth first search for nodes reachable with EPSILON and puts them
// into closure.
// Calls:
//    depthFirstSearchEpsilon() - the non-translator version.
//------------------------------------------------------------------------------
void CompiledNfaEpsilon::fillEpsilonClosure(int& curState,
   unordered_set<int>& closure)
{
   for(Transition transition : transitions)
   {
      if(transition.source == curState && transition.transitionChar == EPSILON)
      {
         closure.emplace(transition.source);
         depthFirstSearchEpsilon(transitions, transition.source, closure);
         break;
      }
   }
}

//------------------------------------------------------------------------------
// fillDestSetFromSrcSet(unordered_set<int>& sourceNodeSet, char& inputChar,
// unordered_set<int>& destinationNodeSet)
// If sourceNodeSet is not empty, adds every node reachable from sourceNodeSet
// on inputchar to destinationNodeSet.
//------------------------------------------------------------------------------
void CompiledNfaEpsilon::fillDestSetFromSrcSet(unordered_set<int>& sourceNodeSet,
   char& inputChar,unordered_set<int>& destinationNodeSet)
{
   if(sourceNodeSet.size() == 0)
   {
      return;
   }

   for(Transition transition : transitions)
   {
      auto src = sourceNodeSet.find(transition.source);
      if(src != sourceNodeSet.end())
      {
         if(transition.transitionChar == inputChar)
         {
            destinationNodeSet.emplace(transition.destination);
         }
      }
   }
}

//------------------------------------------------------------------------------
// fillDestSetFromTransitions(int& curState, char& inputChar,
// unordered_set<int>& destinationNodeSet)
// Adds every destination node reachable from curState on inputChar to
// destinationNodeSet.
//------------------------------------------------------------------------------
void CompiledNfaEpsilon::fillDestSetFromTransitions(int& curState,
   char& inputChar,unordered_set<int>& destinationNodeSet)
{
   for(auto it = transitions.begin(); it!= transitions.end(); ++it)
   {
      if(it->source == curState && it->transitionChar == inputChar)
      {
         destinationNodeSet.emplace(it->destination);
      }
   }
}

//------------------------------------------------------------------------------
// checkIfSetContainsGoalNode(unordered_set<int>& nodeSet)
// Returns true of nodeSet contains a node that is also contained in goalNodes,
// false otehrwise.  In other words, returns true if the intersection is not
// empty, false otherwise.
//------------------------------------------------------------------------------
bool CompiledNfaEpsilon::checkIfSetContainsGoalNode(unordered_set<int>& nodeSet)
{
   for(int node : nodeSet)
   {
      auto nodeIterator = goalNodes.find(node);
      return (nodeIterator == goalNodes.end()) ? false : true;
   }
   return false;
}
