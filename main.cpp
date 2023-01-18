//
//  main.cpp
//  hw4
//
//  Created by Kaytlin Simmer on 3/24/20.
//  Copyright © 2020 Kaytlin Simmer. All rights reserved.
//


#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <stdexcept>
#include"ArgumentManager.h"

using namespace std;

class PrecondViolatedExcep
{
public:
    PrecondViolatedExcep(const string& message)
    {
        cout<<message<<endl;
    }
};

struct command
{
    string comm;
    int priority;
};

class Node
{
private:
 command item;
 Node* next;
public:
 Node()
    {
        next = nullptr;
    }
 Node( const command& anItem)
    {
        item.comm = anItem.comm;
        item.priority = anItem.priority;
        next = nullptr;
    }
 Node( const command& anItem, Node* nextNodePtr)
    {
        item.comm = anItem.comm;
        item.priority = anItem.priority;
        next = nextNodePtr;
    }
void setItem( const command& anItem)
    {
        item = anItem;
    }
void setNext(Node* nextNodePtr)
    {
        next = nextNodePtr;
    }
 command getItem() const
    {
        return item;
    }
 Node* getNext() const
    {
        return next;
    }
};


class LinkedSortedList
{
private:
    Node* headPtr;
    int itemCount;
    Node* getNodeBefore( const command& anEntry) const
    {
        Node* curptr = headPtr;
        Node* prevptr = nullptr;
        while ((curptr != nullptr) && (anEntry.priority <= curptr->getItem().priority))
        {
            prevptr = curptr;
            curptr = curptr->getNext();
        }
        return prevptr;
    }
    Node* getNodeAt( int position) const
    {
       if ((position >= 1) && (position <= itemCount))
       {
        Node* curptr = headPtr;
        for (int i = 1; i < position; i++)
        {
            curptr = curptr->getNext();
        }
        return curptr;
       }
        else
            throw(PrecondViolatedExcep("getEntry() called with an empty list or invalid position"));
        
    }
 /*   Node<ItemType>* copyChain( const Node<ItemType>* origChainPtr)
    {
        Node <ItemType>* copiedChainPtr;
        if (origChainPtr == nullptr)
        {
            copiedChainPtr = nullptr;
            itemCount = 0;
        }
        else
        {
            ItemType itemA = origChainPtr -> getItem();
            Node* <ItemType>* copiedChainPtr = new Node <ItemType> (itemA);
            copiedChainPtr -> setNext(copyChain(origChainPtr->getNext()));
        }
        return copiedChainPtr;
    } */
    
public:
    LinkedSortedList()
    {
        headPtr = nullptr;
        itemCount = 0;
    }
  /*  LinkedSortedList(const LinkedSortedList<ItemType>& aList)
    {
        headPtr = copyChain(aList.headPtr);
    } */
    virtual ~LinkedSortedList()
    {
        clear();
    }
    void insertSorted( const command& newEntry)
    {
        Node* newNodePtr = new Node (newEntry);
        Node* prevPtr = getNodeBefore(newEntry);
        if (isEmpty() || prevPtr == nullptr)
        {
            newNodePtr -> setNext(headPtr);
            headPtr = newNodePtr;
        }
        else
        {
            Node* aftptr = prevPtr -> getNext();
            newNodePtr -> setNext (aftptr);
            prevPtr -> setNext(newNodePtr);
        }
        itemCount++;
    }
    bool removeSorted( const command& anEntry)
    {
        bool ableToDelete = false;
        if (!isEmpty())
        {
            Node* nodeToRemovePtr = headPtr;
            Node* prevPtr = getNodeBefore(anEntry);
            if (prevPtr != nullptr)
                nodeToRemovePtr = prevPtr->getNext();
            ableToDelete = (nodeToRemovePtr != nullptr) && (anEntry.comm == nodeToRemovePtr->getItem().comm && anEntry.priority == nodeToRemovePtr->getItem().priority);
            if (ableToDelete)
            {
                Node* aftptr = nodeToRemovePtr->getNext();
                if (nodeToRemovePtr == headPtr)
                {
                    headPtr = aftptr;
                }
                else
                {
                    prevPtr->setNext(aftptr);
                }
                nodeToRemovePtr->setNext(nullptr);
                delete nodeToRemovePtr;
                nodeToRemovePtr = nullptr;
                itemCount--;
            }
            
        }
        return ableToDelete;
    }
    int getPosition( const command& anEntry) const
    {
        int entry = 1;
        Node* tempPtr = headPtr;
        while ((tempPtr != nullptr) && ( anEntry.priority > tempPtr -> getItem().priority))
        {
            tempPtr = tempPtr ->getNext();
            entry++;
        }
        if ((tempPtr == nullptr) || (anEntry.priority != tempPtr->getItem().priority))
            entry =-entry;
        return entry;
    }
    bool isEmpty() const
    {
        return itemCount == 0;
    }
    int getLength() const
    {
        return itemCount;
    }
    bool remove(int position)
    {
        bool ableToDelete = (position >= 1) && (position <= itemCount);
        if (ableToDelete)
        {
            Node* curptr = nullptr;
            if (position == 1)
            {
                curptr = headPtr;
                headPtr = headPtr -> getNext();
            }
            else
            {
                Node* prevptr = getNodeAt(position - 1);
                curptr = prevptr->getNext();
                prevptr -> setNext(curptr -> getNext());
            }
            curptr->setNext(nullptr);
            delete curptr;
            curptr = nullptr;
            itemCount--;
        }
        return ableToDelete;
    }
    void clear()
    {
        while (!isEmpty())
        remove(1);
    }
    command getEntry(int position) const throw(PrecondViolatedExcep)
    {
        bool ableToGet = (position >= 1) && (position <= itemCount);
        if (ableToGet)
        {
            Node* nodePtr = getNodeAt(position);
            return nodePtr -> getItem();
        }
        else
        {
            throw(PrecondViolatedExcep("getEntry() called with an empty list or invalid position"));
        }
    }
};



class SL_PriorityQueue
{
private:
 LinkedSortedList* slistPtr;
public:
    SL_PriorityQueue()
    {
        slistPtr = new LinkedSortedList();
    }
 SL_PriorityQueue( const SL_PriorityQueue& pq)
    {
        slistPtr = new LinkedSortedList();
        for (int i = 1; i <= pq.slistPtr->getLength(); i++)
        {
            slistPtr->insertSorted(pq.slistPtr->getEntry(i));
        }
    }
 ~SL_PriorityQueue()
    {
        
    }
bool isEmpty() const
    {
        return slistPtr->isEmpty();
    }
bool add(command newEntry)
    {
        slistPtr->insertSorted(newEntry);
        return true;
    }
bool remove()
    {
        return slistPtr ->remove(slistPtr->getLength());
    }
 string peek() const throw(PrecondViolatedExcep)
    {
        if (isEmpty())
            throw PrecondViolatedExcep("peek() called with empty queue");
        return slistPtr->getEntry(slistPtr->getLength()).comm;
    }
};

void print(queue <string> myQueue, ofstream &out)
{
     while (!myQueue.empty())
     {
        out<<myQueue.front()<<endl;
        cout<<myQueue.front()<<endl;
        myQueue.pop();
    }
}

int main(int argc, char* argv[])
{
    ArgumentManager am(argc, argv);
   // const string input = am.get("input");
    //const string output = am.get("output");
    
   string input = "/Users/kaytlinsimmer/Desktop/hw4/input42.txt";
    string output = "/Users/kaytlinsimmer/Desktop/hw4/ans42.txt";
    string str;
    string sInt;
    string decodeMessage;
    bool decodeMessageExists = false;
    string cmd;
    int pos;
    int counter=0;
    SL_PriorityQueue commandQue;
    int count = 0;
    queue <string> decodeQue;
    
    ifstream inFile(input);
    if (!inFile.is_open())
    {
    cout << "Error: Could not open file!" << endl;
    return -1;
    }
    if (inFile.peek() == std::ifstream::traits_type::eof())
    {
    cout << "Error: File is empty!" << endl;
    return -1;
    }
    
    
    while(getline(inFile,str))
    {
        if (str.length() < 0)
        {
            continue;
        }
        int pri = 0;
      //  string temp = str;
        pos = str.find("(");
        sInt = str.substr(pos+1, 1);
        pri = stoi(sInt); //the priority - isolated
        
        cmd = str.substr(0, str.find('('));
        
        if (cmd.find("DECODE") != string::npos)
        {
            decodeMessageExists = true;
            string commU = cmd;
            int posCU = commU.find(':');
            string commUSub = commU.substr(0,posCU);
             string decodeMessage = commU.substr(posCU+1);
            decodeMessage = decodeMessage.substr(1, decodeMessage.size()-2);
             if (decodeMessage.length() != 0)
             {
                     decodeQue.push(decodeMessage);
                     counter++;
             }
        }
        
     /*  int pos3 = temp.find("[");
        int pos4 = temp.find("]");
        string str1 = temp.substr(pos3+1, pos3+pos4); */
                            
      cout<<"command: "<<cmd<<endl;
        cout<<"priority: "<<pri<<endl;
        //cout<<"string: "<<str1<<endl;
        
        command newCommand;
        newCommand.comm = cmd;
        newCommand.priority = pri;
        commandQue.add(newCommand);
        count++;
    }
    
    
    
    cout<<endl;
   cout<<counter<<endl;
   cout<<endl;
  
    ofstream outFile;
    outFile.open(output);
    
    for (int i = 0; i < count; i++)
    {
      if (decodeMessageExists)
      {
       string commU = commandQue.peek();
        cout<<commU<<endl;
       int posCU = commU.find(':');
       string commUSub = commU.substr(0,posCU);
       cout<<commUSub<<endl;
       cout<<endl;
          
          if (commUSub == "DECODE")
          {
              commandQue.remove();
          }
         else if (commUSub == "REPLACE") //works
          {
              string replace = commU.substr(posCU+1);
              replace = replace.substr(1, replace.size()-2);
              string firstOne = replace.substr(0,1);
              string secondOne = replace.substr(2,3);
              char arr1[1];
              arr1[0]=firstOne[0];
              char arr2[1];
              arr2[0]=secondOne[0];
              for (int i = 0; i<counter;i++)
              {
              if (!decodeQue.empty())
              {
              int sizeM = (decodeQue.front()).length();
              for (int i = 0; i<sizeM; i++)
              {
                  if (decodeQue.front()[i]==arr1[0])
                  {
                      decodeQue.front()[i]=arr2[0];
                  }
              }
                  decodeQue.push(decodeQue.front());
                  decodeQue.pop();
              }
              }
             cout<<endl;
              print(decodeQue, outFile);
              commandQue.remove();
          }
          else if (commUSub == "ADD") //works
          {
              string replace = commU.substr(posCU+1);
              replace = replace.substr(1, replace.size()-2);
              string firstOne = replace.substr(0,1);
              string secondOne = replace.substr(2,3);
              char arr1[1];
              arr1[0]=firstOne[0];
              char arr2[1];
              arr2[0]=secondOne[0];
              for (int i = 0; i<counter;i++)
              {
              if (!decodeQue.empty())
              {
              int sizeM = (decodeQue.front()).length();
              for (int i = 0; i<sizeM; i++)
              {
                  if (decodeQue.front()[i]==arr1[0])
                  {
                      decodeQue.front().insert(i+1,1,arr2[0]);
                  }
              }
                  decodeQue.push(decodeQue.front());
                  decodeQue.pop();
              }
              }
             cout<<endl;
             print(decodeQue, outFile);
              commandQue.remove();
          }
          else if (commUSub == "REMOVE") //works
          {
              string replace = commU.substr(posCU+1);
              replace = replace.substr(1, replace.size()-1);
              string firstOne = replace.substr(0,1);
              char arr1[1];
              arr1[0]=firstOne[0];
              for (int i = 0; i<counter;i++)
              {
              if (!decodeQue.empty())
              {
                int sizeM = (decodeQue.front()).length();
                for (int i = 0; i<sizeM; i++)
                {
                    if (decodeQue.front()[i]==arr1[0])
                    {
                        decodeQue.front().erase(decodeQue.front().begin()+i); //missing the second 2?
                    }
                }
                decodeQue.push(decodeQue.front());
                decodeQue.pop();
              }
              }
            cout<<endl;
           print(decodeQue, outFile);
              commandQue.remove();
            }
          else if (commUSub == "SWAP") //FIXME FMLFMJDKFNAW
          {
              string replace = commU.substr(posCU+1);
              replace = replace.substr(1, replace.size()-2);
              string firstOne = replace.substr(0,1);
              string secondOne = replace.substr(2,3);
              char arr1[1];
              arr1[0]=firstOne[0];
              char arr2[1];
              arr2[0]=secondOne[0];
              if (!decodeQue.empty())
              {
                  string temps = decodeQue.front();
              int sizeM = temps.size();
                  decodeQue.pop();
              for (int i = 0; i<sizeM; i++)
              {
                  if (temps[i]==arr1[0])
                  {
                      temps[i]=arr2[0];
                      continue;
                  }
                 if (temps[i] == arr2[0])
                  {
                      temps[i]=arr1[0];
                      continue;
                  }

                  
              }
               decodeQue.push(temps);
              }
              cout<<endl;
              print(decodeQue, outFile);
              commandQue.remove();
          }
          else if (commUSub == "PRINT")
          {
              print(decodeQue, outFile);
              commandQue.remove();
          }
          else
          {
              cout<<"UNRECOGNIZED COMMAND"<<endl;
              commandQue.remove();
          }
      }
        else
        {
            outFile<<""<<endl;
        }
    }
    
}

