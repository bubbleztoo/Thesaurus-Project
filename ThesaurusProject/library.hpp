#ifndef HASH_LIBRARY_HPP
#define HASH_LIBRARY_HPP

#include <vector>
#include <string>
#include <cctype>

using namespace std;

struct heapNode // heap for data storage of "best" synonyms, MAXHEAP
{
  string word;
  int closeness; // how "good" of a synonym it is
  int natural; // how natural the word is in this sentence case, called in returning top words
};

struct hashNode
{
  hashNode()
  {
    this->LLnext = NULL;
    this->word = "";
  }
  hashNode* LLnext; // next in chaining hash linked list
  string word;
  vector<heapNode> synonyms; // heap of synonyms in vector (undetermined size) for this word
  // synonyms is declared public to allow for get top3 answers easier
  void addSyn(string wor, int closeness, int natural); // add to heap
  // to adjust heap for this node in hash
  void repairUpward(int nodeIndex);
  void repairDownward(int nodeIndex);
  void increClose(int nodeIndex);
private:
  // void repairDownward(int nodeIndex); // I actually don't think we need this. Not deleting any nodes
  void swap(heapNode* swapper, heapNode* swappee); // NOTE: swap takes pointers, but synonyms is not a vector of pointers
};
void hashNode::increClose(int nodeIndex)
{
  if (nodeIndex < 1)
  {
    cout << "Error, invalid increment. User inpt. not stored." << endl;
    return;
  }
  else
  {
    synonyms[nodeIndex].closeness++;
    return;
  }
}
void hashNode::addSyn(string wor, int clo, int nat)
{
  if (synonyms.size() == 0)
  {
    heapNode none;
    none.word = "Placehodler";
    none.closeness = 0;
    none.natural = 0;
    synonyms.push_back(none);
  }
  for (int l = 0; l < wor.length(); l++)
  {
    wor[l] = tolower(wor[l]);
  }
  for (int i = 1; i < synonyms.size(); i++) // increment checking for if word already exists
  {
    if (synonyms[i].word == wor)
    {
      synonyms[i].closeness++;
      repairUpward(i); // newly added code
      return;
    }
  }
  heapNode* temp = new heapNode;
  temp->word = wor;
  temp->closeness = clo;
  temp->natural = nat;
  synonyms.push_back(*temp); // dereference temp, because it is not a vector of pointers
  repairUpward(synonyms.size() - 1); // fix the heap
  return;
}
void hashNode::swap(heapNode* swapper, heapNode* swappee)
{
  heapNode* temp = new heapNode;
  temp->word = swappee->word;
  temp->closeness = swappee->closeness;
  temp->natural = swappee->natural;
  swappee->word = swapper->word;
  swappee->closeness = swapper->closeness;
  swappee->natural = swapper->natural;
  swapper->word = temp->word;
  swapper->closeness = temp->closeness;
  swapper->natural = temp->natural;
  delete temp;
  return;
}
void hashNode::repairDownward(int nodeIndex)
{
  if (nodeIndex >= synonyms.size() || nodeIndex <= 0)
  {
    return; // nothing to change
  }
  // heapNode* temp1 = &synonyms[nodeIndex];
  // nodeIndex = nodeIndex * 2;
  // heapNode* temp2 = &synonyms[nodeIndex];
  // nodeIndex++;
  // heapNode* temp3 = &synonyms[nodeIndex];
  // if (temp1->closeness > temp2->closeness) // max heap
  // {
  //   swap(temp1, temp2);
  //   repairUpward(nodeIndex); // since nodeIndex is now set as the node we (possibly) swapped to, should run again at correct location
  // }
  // return;
  int l = nodeIndex*2;
  int r = nodeIndex*2 + 1;
  if (l >= synonyms.size())
  {
    return; // already at bottom
  }
  if (r >= synonyms.size())
  { // we KNOW that l < synonyms.size()
    if (synonyms[nodeIndex].closeness < synonyms[l].closeness)
    {
      swap(&synonyms[nodeIndex], &synonyms[l]);
    }
    return;
  }
  // Finds the smallest of node, left Child, and right child
  // The parent is swapped with the smallest of all 3
  int largest = nodeIndex;
  if (synonyms[l].closeness > synonyms[nodeIndex].closeness)
  {
    largest = l;
    if (synonyms[r].closeness > synonyms[l].closeness)
    {
      largest = r;
    }
  }
  else if (synonyms[r].closeness > synonyms[nodeIndex].closeness)
      largest = r;

  if (largest == nodeIndex)
  {
    return;
  }
  else
  {
    swap(&synonyms[nodeIndex], &synonyms[largest]);
    nodeIndex = largest;
    repairDownward(largest);
    return;
  }
}

void hashNode::repairUpward(int nodeIndex)
{
  if (nodeIndex < 1) // just to make sure I didn't screw anything up
  {
    cout << "How did this happen?" << endl;
    return;
  }

  if (nodeIndex == 1) // i.e. at start of heap
  {
    // cout << "At start of heap." << endl;
    return;
  }
  /**
  Alternatively:
  if (nodeIndex % 2 == 0) // left child
  {
    if (synonyms[nodeIndex].closeness > synonyms[nodeIndex / 2].closeness)
    {
      heapNode* temp1 = synonyms[nodeIndex];
      heapNode* temp2 = synonyms[nodeIndex / 2];
      swap(temp1, temp2);
    }
  }
  else if (nodeIndex % 2 == 1) // right child (can just be else)
  {
    if (synonyms[nodeIndex].closeness > synonyms[(nodeIndex - 1) / 2].closeness) // should swap
    {
      heapNode* temp1 = synonyms[nodeIndex];
      heapNode* temp2 = synonyms[(nodeIndex - 1) / 2];
      swap (temp1, temp2);
    }
  }
  **/
  if (nodeIndex > synonyms.size())
  {
    cout << "Error, no value to swap." << endl;
    return;
  }
  heapNode* temp1 = &synonyms[nodeIndex]; // child
  if (nodeIndex % 2 != 0)
  {
    nodeIndex = (nodeIndex - 1) / 2; // set nodeIndex to be parent nodeIndex
  }
  else
  {
    nodeIndex = nodeIndex / 2;
  }
  heapNode* temp2 = &synonyms[nodeIndex]; // parent
  if (temp1->closeness > temp2->closeness) // max heap
  {
    swap(temp1, temp2);
    repairUpward(nodeIndex); // since nodeIndex is now set as the node we (possibly) swapped to, should run again at correct location
  }
  return;
}

class hashLib
{
public:
  hashLib(int size);
  ~hashLib();
  void addWord(string wor); // do some file i/o using this to create a hashtable from text file
  // remove node functionality?
  hashNode* getHashNode(string wor); // return pointer to location of word/hashNode in the hash table
  // possibly a file i/o statement to write to file?
  void printHashTable()
  {
    for (int i = 0; i < hashSize; i++)
    {
      cout << i << ":\t" << flush;
      if (list[i].word != "")
      {
        cout << list[i].word << flush;
        hashNode* temp = list[i].LLnext;
        while(temp != NULL)
        {
          cout << " -> " << temp->word << flush;
          temp = temp->LLnext;
        }
      }
      cout << endl;
    }
  }
private:
  int hash(string wor);
  hashNode* list; // pointer to first element in hash table list
  int hashSize;
};
hashLib::hashLib(int size)
{
  this->list = new hashNode[size];
  for (int i = 0; i < size; i++)
  {
    this->list[i].LLnext = NULL;
    this->list[i].word = "";
  }
  this->hashSize = size;
}
hashLib::~hashLib()
{
  delete[] this->list;
  this->hashSize = 0;
}
void hashLib::addWord(string wor)
{
  if (getHashNode(wor) != NULL)
  {
    // cout << "Word already in library" << endl;
    return;
  }
  for (int l = 0; l < wor.length(); l++)
  {
    wor[l] = tolower(wor[l]);
  }
  int loc = hash(wor);
  if (list[loc].word != "") // if node exists at this location in hashtable
  {
    hashNode* temp = list[loc].LLnext;
    hashNode* prev = &list[loc];
    while(temp != NULL) // go until end of chain
    {
      prev = temp;
      temp = temp->LLnext;
    }
    temp = new hashNode;
    temp->word = wor;
    temp->LLnext = NULL;
    prev->LLnext = temp;
    return;
  }
  else
  {
    list[loc].word = wor;
    // list[loc].LLnext = NULL; Don't need b/c constructor already sets LLnext = NULL
    return;
  }
}
hashNode* hashLib::getHashNode(string wor)
{
  string wo;
  string wah;
  if (wor[wor.length() - 1] == '.' || wor[wor.length() - 1] == ',' || wor[wor.length() - 1] == '?' || wor[wor.length() - 1] == '!')
  {
    wah = wor.substr(0, wor.length() - 1);
    // cout << wo << endl;
  }
  else
  {
    wah = wor;
  }
  if (wah[0] == ' ' || wah[0] == ',' || wah[0] == '.' || wah[0] == '?' || wah[0] == '!')
  {
    wo = wah.substr(1, wah.length());
  }
  else
  {
    wo = wah;
  }
  for (int l = 0; l < wo.length(); l++)
  {
    wo[l] = tolower(wo[l]);
  }
  // cout << wo << endl;
  // here too
  int loc = hash(wo);
  hashNode* temp = NULL;
  if (list[loc].word == wo)
  {
    temp = &list[loc];
    return temp;
  }
  else
  {
    temp = list[loc].LLnext;
    while (temp != NULL && temp->word != wo)
    {
      temp = temp->LLnext;
    }
    if (temp == NULL)
    {
      // cout << "Word not in library." << endl;
      return NULL;
    }
    return temp;
  }
}
int hashLib::hash(string wor)
{
  string wo;
  if (wor[0] == ' ')
  {
    wo = wor.substr(1, wor.length());
  }
  else
  {
    wo = wor;
  }
  for (int l = 0; l < wo.length(); l++)
  {
    wo[l] = tolower(wo[l]);
  }
  // Ok, it breaks here...
  // cout << wo << endl;
  int ret = 3784;
  for (int i = 0; i < wo.length(); i++)
  {
    ret = ret + wo[i];
  }
  // if (ret < 0)
  // {
  //   ret = 0 - ret; // right? this should return same if it overflows
  // }
  ret = ret % this->hashSize; // I don't need to use this-> do I?
  return ret;
}

// When you add word/add syn you add it to the other place too
// The advantage of using a graph is I wouldn't need to add it for every heap, the connections would already be made
// I mean have a synonym of a synonym already be connected by higher weight, so practiacally, it dosen't matter

// ignore top 100 most common words, this will happen to be a, an, i, and so on

// MAKE A LEARNING FUNCTION WHERE YOU RETURN THE BEST ONE back to it AND IT'LL do ++ to that closeness var
// THEN MAKE A THESAURIZE FULLY FUNCTION WHERERIN IT JUST REPLACES IT WITH TOP FROM HEAP

#endif
