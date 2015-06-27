//------------------------------------------------------------------------------
// main.cpp
// John Wehrle
// 15 March 2015
// Driver for FiniteAutomata project.
// Functions as a basic test bed for the four types of regular expression
// processors in this project:
// NFA-EPSILON
// DFA translation from NFA-EPSILON passed in translateToDfa() function
// DFA translation from NFA-EPSILON passed in constructor
// DFA
// The test regular expression is the regular expression assigned for a
// previous homework: ab*|b*c|a*c*
//------------------------------------------------------------------------------
#include <iostream>
#include "CompiledDfa.h"
#include "CompiledNfaEpsilon.h"

using namespace std;

int main()
{

   //nfa-epsilon for ab*|b*c|a*c*
   FiniteStateMachine nfaeRegEx;
   nfaeRegEx.nodes = {0, 1, 2, 3, 4, 5, 6};
   nfaeRegEx.startNode = 0;
   nfaeRegEx.goalNodes = {1, 2, 4, 5, 6};
   nfaeRegEx.transitions.emplace_back(0, '\x0', 1);
   nfaeRegEx.transitions.emplace_back(0, '\x0', 3);
   nfaeRegEx.transitions.emplace_back(0, '\x0', 5);
   nfaeRegEx.transitions.emplace_back(1, 'a', 2);
   nfaeRegEx.transitions.emplace_back(2, 'b', 2);
   nfaeRegEx.transitions.emplace_back(3, 'b', 3);
   nfaeRegEx.transitions.emplace_back(3, 'c', 4);
   nfaeRegEx.transitions.emplace_back(5, 'a', 5);
   nfaeRegEx.transitions.emplace_back(5, 'c', 6);
   nfaeRegEx.transitions.emplace_back(6, 'c', 6);

   CompiledNfaEpsilon nfae(nfaeRegEx);


   FiniteStateMachine dfaRegEx;
   dfaRegEx.nodes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
   dfaRegEx.startNode = 0;
   dfaRegEx.goalNodes = {0, 1, 3, 4, 5, 6, 7, 8, 9};
   dfaRegEx.transitions.emplace_back(0, 'a', 1);
   dfaRegEx.transitions.emplace_back(0, 'b', 2);
   dfaRegEx.transitions.emplace_back(0, 'c', 3);
   dfaRegEx.transitions.emplace_back(1, 'a', 4);
   dfaRegEx.transitions.emplace_back(1, 'b', 5);
   dfaRegEx.transitions.emplace_back(1, 'c', 6);
   dfaRegEx.transitions.emplace_back(2, 'b', 2);
   dfaRegEx.transitions.emplace_back(2, 'c', 7);
   dfaRegEx.transitions.emplace_back(3, 'c', 8);
   dfaRegEx.transitions.emplace_back(4, 'a', 4);
   dfaRegEx.transitions.emplace_back(4, 'c', 9);
   dfaRegEx.transitions.emplace_back(5, 'b', 5);
   dfaRegEx.transitions.emplace_back(6, 'c', 6);
   dfaRegEx.transitions.emplace_back(8, 'c', 8);
   dfaRegEx.transitions.emplace_back(9, 'c', 9);

   CompiledDfa dfa(dfaRegEx);

   FiniteStateMachine translation = nfae.translateToDFA(nfaeRegEx);
   CompiledDfa translatedFromNFAE(translation);
   CompiledDfa defaultTrans(nfae.translateToDFA());

   string input;
   cout << "Enter a string to check (press x to stop): " << endl;
   while(cin >> input && input != "x")
   {
      cout << "NFA with Epsilon Check: " << input;
      nfae.checkString(input) ? cout << " true" : cout << " false";
      cout << endl;
      cout << "DFA Check: " << input;
      dfa.checkString(input) ? cout << " true" : cout << " false";
      cout << endl;
      cout << "Translation to DFA Check: " << input;
      translatedFromNFAE.checkString(input) ? cout << " true" : cout << " false";
      cout << endl;
      cout << "Default Translation to DFA Check: " << input;
      defaultTrans.checkString(input) ? cout << " true" : cout << " false";
      cout << endl;
   }

   return 0;
}
