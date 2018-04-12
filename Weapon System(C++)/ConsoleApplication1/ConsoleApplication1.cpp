/*
	Andre Reano 101075780
	Taha Saleem 100803048

 */

#include "stdafx.h"
#include<iostream>
#include <string>

using namespace std;


// Weapon class
class Weapon {
public:
	string weaponName;
	int range;
	int damage;
	float weight;
	float cost;

	Weapon(string n, int rang, int dam, float w, float c) // Initiate weapon object with its attributes
	{
		weaponName = n;
		damage = dam;
		range = rang;
		weight = w;
		cost = c;
	}
};

// Node class
class Node {
public:
	Weapon* myWeapon;
	Node * left;
	Node * right;

	Node(Weapon* d)
	{
		myWeapon = d;
		left = right = NULL;
	}
};

// Binary tree in which weapons will be stored
class BinaryTree
{
public:
	Node *root;

	BinaryTree() {
		root = NULL;
	}

	// Pushes weapon into binary tree
	void push(Weapon* x)
	{
		Node* newNode = new Node(x); // Creates node for weapon storage

		if (root == NULL) // If there is no root, then first push becomes root of tree
		{
			root = newNode;
			return;
		}
		Node* parent = root;
		Node* current = root;

		while (current != NULL)
		{
			parent = current;
			if (current->myWeapon->weaponName < x->weaponName) // If new weapon being pushed is bigger than parent node, insert right side
				current = current->right;
			else
				current = current->left; // If it's smaller than parent, insert on left side
		}
		if (parent->myWeapon->weaponName < x->weaponName)
			parent->right = newNode;
		else
			parent->left = newNode;
	}

	Node* search(Node* r, string wName)
	{
		if (root == NULL)
		{
			return NULL;	// Returns null if nothing is in tree
		}
		Node * parent = r;
	
		if (parent == NULL)
		{
			return NULL;
		}
		if (parent->myWeapon->weaponName.compare(wName) == 0) 
		{
			return parent; //Weapon name found
		}
		else if (parent->myWeapon->weaponName.compare(wName) < 0) // Searches right side of tree
		{
			parent = search(parent->right, wName);
			return parent;
			
		}
		else if (parent->myWeapon->weaponName.compare(wName) > 0) // Searches left side of tree
		{
			parent = search(parent->left, wName);
			return parent;
		}
		
	}

	Node* searchNode(string key)
	{
		return search(root, key);
	}

	void displayInorder() // Displays weapons 
	{
		cout << "Weapon List: \n";
		inOrder(root); //Calls in order function with root as parameter and begins displaying in order
	}

	void inOrder(Node * n) 
	{
		if (n == NULL)
		{
			return; // Nothing to display
		}
		inOrder(n->left); // Prints from left to right in BST
		cout << "Weapon Name: "<< n->myWeapon->weaponName << " / ";
		cout  << "Weapon Damage: " << n->myWeapon->damage<< " / ";
		cout  << "Weapon Cost: $" << n->myWeapon->cost <<endl;
		inOrder(n->right);
	}

	Node* getInOrderSucc(Node* node)
	{
		Node* curr = node;
		while (curr->left != NULL)
			curr = curr->left;

		return curr;
	}

	Node* deleteNode(Node* curr, string key) 
	{
		if (curr == NULL) return NULL; // Returns null no nodes to delete

		if (key < curr->myWeapon->weaponName) 
			curr->left = deleteNode(curr->left, key);

		// If one of the sides is null but not the other
		else {

			if (key > curr->myWeapon->weaponName)
			{
				curr->right = deleteNode(curr->right, key);
			}

			else 
			{
				if (curr->left == NULL)
				{
					Node* tempVal = curr->right;
					delete(curr);
					return tempVal;
				}
				else if (curr->right == NULL)
				{
					Node* tempVal = curr->left;
					delete(curr);
					return tempVal;
				}
				
				else // More than 1 level of nodes in tree, brings in successor node to replace deleted node
				{
					Node* temp = getInOrderSucc(curr->right);
					curr->myWeapon->weaponName = temp->myWeapon->weaponName;
					curr->right = deleteNode(curr->right, temp->myWeapon->weaponName);
				}
				
			}
		}
		return curr;
	}

	// Calls delete node function with the name of weapon we want to delete as parameter
	void deleteNodeCall(string key) 
	{
		root = deleteNode(root, key);
	}

};

// Player class
class Player {
public:
	string name;
	Weapon ** backpack;
	int numItems;
	float money;

	Player(string n, float m) // Player object with data initiations
	{
		name = n;
		money = m;
		numItems = 0;
		backpack = new Weapon*[10];
	}

	void buy(Weapon * w) // When players buys a weapon from shop
	{
		cout << w->weaponName << " bought..." << endl;
		backpack[numItems] = w;
		numItems++;
		cout <<"Number of Weapons owned:  " << numItems <<endl;
	}

	void withdraw(float amt) 
	{
		money = money - amt;
	}

	bool inventoryFull() // If inventory full returns true
	{
		bool full = false;
		if (numItems == 10)full = true;
		return full;
	}


	void printCharacter() {
		cout << " Name:" << name << "\n Money:" << money << endl;
		printBackpack();
	}

	void printBackpack() {
		cout << " " << name << ", you own " << numItems << " Weapons:" << endl;
		for (int x = 0; x<numItems; x++) {
			cout << " " << backpack[x]->weaponName << endl;
		}
		cout << endl;
	}

};

void addWeapons(BinaryTree *h) // Player inputs name and weapon specifications, program proceeds to push weapon objects with its data into tree
{
	cout << "***********WELCOME TO THE WEAPON ADDING MENU*********" << endl;
	string weaponName; int weaponRange; int weaponDamage; float weaponWeight; float weaponCost;
	cout << "Please enter the NAME of the Weapon ('end' to quit):"; cin >> weaponName;
	while (weaponName.compare("end") != 0) {
		cout << "Please enter the Range of the Weapon (0-10):"; cin >> weaponRange;
		cout << "Please enter the Damage of the Weapon:"; cin >> weaponDamage;
		cout << "Please enter the Weight of the Weapon (in pounds):"; cin >> weaponWeight;
		cout << "Please enter the Cost of the Weapon:"; cin >> weaponCost;
		Weapon *w = new Weapon(weaponName, weaponRange, weaponDamage, weaponWeight, weaponCost);
		h->push(w);
		cout << "Please enter the NAME of another Weapon ('end' to quit):"; cin >> weaponName; // Enter another weapon or stops if player inputs 'end'
	}
}

void showRoom(BinaryTree *ht, Player *p) // Function that shows functionality of weapon purchase
{
	string choice;
	cout << "WELCOME TO THE SHOWROOM!!!!" << endl;
	ht->displayInorder();
	cout << " You have $ " << p->money << endl;
	cout << "Please select a weapon to buy('end' to quit):"; cin >> choice;

	while (choice.compare("end") != 0 && !p->inventoryFull()) 
	{
		Weapon *w;
		Node* room = ht->searchNode(choice); // Searches tree for weapon to buy

		if (room == NULL)
		{
			w = NULL;
		}
		else
		{
			w = room->myWeapon;
		}

		if (w != NULL) 
		{
			if (w->cost > p->money) 
			{
				cout << "Insufficient funds to buy " << w->weaponName << endl;
			}
			else 
			{
				p->buy(w);
				p->withdraw(w->cost); // Decreases money
				ht->deleteNodeCall(w->weaponName); // Deletes bought weapon from tree
				ht->displayInorder(); // Displays weapons left to be bought
			}
		}
		else 
		{
			cout << " ** " << choice << " not found!! **" << endl; // If weapon name entered is not in tree, message is displayed not "found"
		}
		cout << "Please select another weapon to buy('end' to quit):"; cin >> choice;
	}
	cout << endl;
}


int main() {
	string pname;
	BinaryTree *ht = new BinaryTree();
	cout << "Please enter Player name: " << endl;
	cin >> pname;
	Player pl(pname, 100);
	addWeapons(ht);
	showRoom(ht, &pl);
	pl.printCharacter();

	return 0;
}