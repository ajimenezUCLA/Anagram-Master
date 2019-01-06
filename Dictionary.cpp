// Dictionary.cpp

#include "Dictionary.h"
#include <string>
#include <list>	// is there even a difference?
#include <cctype>
#include <utility>  // for swap
#include <vector>	// plenty fast
#include <algorithm>

// #include <iostream>	// for testing

using namespace std;

//////////////////////////////////////////////
//		FUNCTION PROTOTYPES & CONSTANTS		//
//////////////////////////////////////////////

const size_t HASHBUCKETS = 49999;	// size_t for string size comparisons
//const int prime[26] = { 5, 71, 37, 29,  2, 53, 59, 19, 11, 83, 79, 31, 43, 13, 7, 67, 97, 23, 17, 3, 41, 73, 47, 89, 61, 101 };
void removeNonLetters(string& s);
void generateNextPermutation(string& permutation);

//////////////////////////////////////////
//		CLASS & STRUCT DECLARATION		//
//////////////////////////////////////////

class DictionaryImpl
{
public:
	DictionaryImpl() {}	
	~DictionaryImpl() {}
	void insert(string word);
	void lookup(string letters, void callback(string)) const;
private:
	vector<string> hashTable[HASHBUCKETS];

};

//////////////////////////////
//		HASH FUNCTIONS		//
//////////////////////////////

// has to be unsigned because we don't want overflow
// has to be long because, we still don't want overflow.

unsigned long hashFunction(string word)
{
	// etaoinshrdlcumwfgypbvkjxqz
	int prime[26] = { 5, 71, 37, 29,  2, 53, 59, 19, 11, 83, 79, 31, 43, 13, 7, 67, 97, 23, 17, 3, 41, 73, 47, 89, 61, 101 };
	
	unsigned long keyGen = 25111;	// used to generated a unique key
	for (size_t i = 0; i < word.length(); i++)
	{
		keyGen = keyGen * prime[word[i] - 'a'];
	}
	return keyGen % HASHBUCKETS;
}

//////////////////////////////
//		MAIN FUNCTIONS		//
//////////////////////////////

void DictionaryImpl::insert(string word)
{
    removeNonLetters(word);
	if (!word.empty())
	{
		// m_words.push_back(word);
		unsigned long tempKey = hashFunction(word);
		hashTable[tempKey].push_back(word);
	}
	
}

void DictionaryImpl::lookup(string letters, void callback(string)) const
{
	if (callback == nullptr)
		return;

	removeNonLetters(letters);
	if (letters.empty())
		return;

	unsigned long tempKey = hashFunction(letters);
	vector<string>::const_iterator it = hashTable[tempKey].begin();
	while (it != hashTable[tempKey].end())
	{
		string tempWordUno = letters;
		string tempWordDos = (*it); 
		sort(tempWordUno.begin(), tempWordUno.end());
		sort(tempWordDos.begin(), tempWordDos.end());
		if (tempWordUno == tempWordDos)
		{
			callback(*it);
		}
		it++;
	}

}
    
//////////////////////////////////
//		DON'T TOUCH THESE		//
//////////////////////////////////

void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop everything off from "to" to end.
} 

  // Each successive call to this function generates the next permutation of the
  // characters in the parameter.  For example,
  //    string s = "eel";
  //    generateNextPermutation(s);  // now s == "ele"
  //    generateNextPermutation(s);  // now s == "lee"
  //    generateNextPermutation(s);  // now s == "eel"
  // You don't have to understand this function's implementation.
void generateNextPermutation(string& permutation)
{
    string::iterator last = permutation.end() - 1;
    string::iterator p;

    for (p = last; p != permutation.begin()  &&  *p <= *(p-1); p--)
        ;
    if (p != permutation.begin())
    {
        string::iterator q;
        for (q = p+1; q <= last  &&  *q > *(p-1); q++)
            ;
        swap(*(p-1), *(q-1));
    }
    for ( ; p < last; p++, last--)
        swap(*p, *last);
}

//////////////////////////////////////
//		DICTIONARY MAINTENANCE		//
//////////////////////////////////////

//******************** Dictionary functions ********************************

// These functions simply delegate to DictionaryImpl's functions.
// You probably don't want to change any of this code

Dictionary::Dictionary()
{
    m_impl = new DictionaryImpl;
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters,callback);
}
