
/*bsh.h*/
//
// <<JUDY PARK>>
// U. of Illinois, Chicago
// CS 251: Fall 2019
//
// Project #02: Binary search tree class
//

#pragma once

#include <iostream>
#include <algorithm>  // std::max

using namespace std;

template<typename TKey, typename TValue>
class binarysearchtree
{
private:
	// each NODE struct will have a Key which will aid in searching,
	// a Value which will store the information, and its respective
	// Left and Right pointers
	struct NODE
	{
		TKey  Key;
		TValue Value;
		NODE* Left;
		NODE* Right;
	};

	NODE* Root;  // pointer to root node of tree (nullptr if empty)
	int   Size;  // # of nodes in the tree (0 if empty)

	// _height does the actual work of computing height, recursively.
	int _height(NODE* cur)
	{
		// tree is empty
		if (cur == nullptr)
			return -1;
			
		// tree is not empty
		else
		{
			int heightL = _height(cur->Left); // traverse the left side of the tree and record the height
			int heightR = _height(cur->Right); // traverse the right side of the tree and record the height

			return 1 + std::max(heightL, heightR); // return the larger height (whether it is the right or left side) + 1
		}
	}

	// _inorder does the actual inorder traversal and output 
	// to console.  Each key is output to the console followed
	// by " ", including the last key.
	void _inorder(NODE* cur)
	{
		// tree is empty, nothing is printed
		if (cur == nullptr) {
			return;
		}
		
		// tree is not empty
		else {
			_inorder(cur->Left); // traverse the left side
			cout << cur->Key << " "; // print out the key of each node
			_inorder(cur->Right); // traverse the right side
		}
	}
  
	// _destroyNode helper function to be used in the destructor to delete/clear a tree recursively
	void _destroyNode( NODE* node)
	{
		// tree is not empty
		if ( node != nullptr) {
		
			_destroyNode( node->Left); // traverse the left side
			_destroyNode( node->Right); // traverse the right side
			
			delete node; // delete the node
		}
	}
	
	// _copyTree helper function to help create a copy of a tree recursively
	void _copyTree( NODE*& thisRoot, NODE*& sourceRoot)
	{
		// the source tree is empty
		if (sourceRoot == nullptr) {
			thisRoot = nullptr; // make a copy of an empty tree
		}
		
		// the source tree is not empty
		else {
		
			thisRoot = new NODE; // create a new node for the copy
			
			// copy variables from a node of the source tree to the new node in the copy tree
			thisRoot->Key = sourceRoot->Key; // copy the key
			thisRoot->Value = sourceRoot->Value; // copy the value
			
			_copyTree(thisRoot->Left, sourceRoot->Left); // traverse the left side of the tree
			_copyTree(thisRoot->Right, sourceRoot->Right); // traverse the right side of the tree
		}
	}

public:

	// default constructor:
	// Creates an empty tree.
	binarysearchtree()
	{
		Root = nullptr;
		Size = 0;
	}

	// copy constructor:
	// Creates a copy of another tree
	binarysearchtree(binarysearchtree& other)
	{
		this->Root = nullptr;
		this->Size = other.Size;
		_copyTree(this->Root, other->Root);
	}
  
	// destructor:
	// Called automatically by system when tree is about to be destroyed;
	// this is our last chance to free any resources / memory used by
	// this tree.
	virtual ~binarysearchtree()
	{
		_destroyNode(Root);
	}
  
	// Returns the # of nodes in the tree, 0 if empty.
	int size()
	{
		return Size;
	}

	// Computes and returns height of tree; height of an empty tree is
	// defined as -1.
	int height()
	{
		return _height(Root);
	}

	// Searches the tree for the given key, returning a pointer to the node's value if found.
	// if it has not been found, return a nullptr
	TValue* search(TKey key)
	{
		NODE* cur = Root; // start at the root of the tree

		// loop through until the key is found
		while (cur != nullptr)
		{
			if (key == cur->Key)  // already in tree
				return (&cur->Value);

			if (key < cur->Key)  // search left:
			{
				cur = cur->Left;
			}
			else // search right
			{
				cur = cur->Right;
			}
		}//while  

		// if get here, not found
		return nullptr;
	}

	// Inserts the given key into the tree; if the key has already been insert then
	// the function returns without changing the tree.
	void insert(TKey key, TValue value)
	{
		NODE* prev = nullptr;
		NODE* cur = Root;

		// search to see if tree already contains key:
		while (cur != nullptr)
		{
			if (key == cur->Key)  // already in tree
				return;

			if (key < cur->Key)  // search left:
			{
				prev = cur;
				cur = cur->Left;
			}
			else // search right
			{
				prev = cur;
				cur = cur->Right;
			}
		}//while

		// if we get here, key is not in tree, so allocate
		// a new node to insert:
		NODE* newNode;
		newNode = new NODE();
		newNode->Key = key;
		newNode->Value = value;
		newNode->Left = nullptr;
		newNode->Right = nullptr;

		// link in the new node:
		if (prev == nullptr) // tree is empty, insert first node
			Root = newNode;
		else if (key < prev->Key) // key is less than the previous node, insert to left
			prev->Left = newNode;
		else
			prev->Right = newNode; // key is greater than the previous node, insert to right

		// update size
		Size++;
	}
	
	// Prints the memory address of the root node of a tree
	void printRootAddress(){
        cout << "Root Key Address : "<< &Root->Key << endl;
    }
	
	// Performs an inorder traversal of the tree, outputting
	// the keys to the console.
	void inorder()
	{
		cout << "Inorder: ";

		_inorder(Root);

		cout << endl;
	}


	// Clears the contents of the tree, resetting the tree to empty.
	void clear()
	{
		// Delete all the old nodes
		_destroyNode (Root);

		// Re-initialize root pointer and size to "empty":
		Root = nullptr;
		Size = 0;
	}

};
