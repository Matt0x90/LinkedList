//============================================================================
// Name        : LinkedList.cpp
// Author      : Matt
// Version     : 1.0
// Description : Linked lists implementation that reads/writes a CSV file for bid data.
// Utilizes a dummy head node to simplify logic. Modified the original version to load any data path.
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include <fstream>
#include <iomanip>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    //Internal structure for list entries, housekeeping variables
    struct Node {
        Bid bid; // Bid data
		Node* next; // Pointer to next node

        // default constructor
		//useful for creating a dummy node
        Node() {
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) {
            bid = aBid;
            next = nullptr;
        }
    };

    Node* head; //dummy head node
    Node* tail; // points to tail end for append
	int size = 0; // number of bids in the list

public:
	LinkedList(); // default constructor
	virtual ~LinkedList(); //virtual destructor
    void Append(Bid bid); // append bid data to end of list
	void Prepend(Bid bid); // prepend bid data to start of list
	void PrintList(); // print all bids in the list
	void Remove(string bidId); // remove requested bid using bid ID
	Bid Search(string bidId); // search for bid using bid ID, return Bid structure if found, or empty Bid if not
    int Size(); // return number of bids in list
	void SaveCSV(const string& path) const; // save list to CSV file
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
	//I'm using a dummy node instead for head/tail
    head = new Node(); // dummy node
	tail = head; // at start, tail is same as head
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // Sort of confusing the way it was done, so I rewrote it.
	Node* current = head; // start at dummy head
	while (current != nullptr) { // while not at end of list or empty
        Node* next = current->next; // hold on to next node
        delete current; // delete current node
        current = next; // move to next node
	}
	head = tail = nullptr; // ensure head and tail are null
	size = 0; // reset size
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
    
    //Create new node
    Node* newNode = new Node(bid);
    // Since I'm using dummy nodes, I can skip conditional checks
	tail->next = newNode; // make current tail node point to the new node
	tail = newNode; // update tail to the new node
	size++; // increase size count
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
    // Create new node
    Node* newNode = new Node(bid);

	newNode->next = head->next; // new node points to current first node
	head->next = newNode; // head now points to new node
    if (tail == head) { // if list was empty, update tail
        tail = newNode;
	}
	size++; // increase size count
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    // start at the head
    Node* current = head->next; // skip dummy head
	// while it isn't empty & isn't at the end of the list
    while (current != nullptr) {
        cout << current->bid.bidId << ": " << current->bid.title << " | "
             << current->bid.amount << " | " << current->bid.fund << endl;
        current = current->next; // move to next node
	}
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
	Node* prev = head; // prev = dummy head
	Node* current = head->next; // current = first real node
	while (current != nullptr) { // while list isn't empty & not at end
		if (current->bid.bidId == bidId) { // if match found
			prev->next = current->next; // first real node now points beyond current
            if (current == tail) { // if removing tail, update tail
				tail = prev;
			}
			delete current; // free memory
			size--; // decrease size count
			return; // return
		}
		prev = current; // prev moves to current node
		current = current->next; // current moves to next node
	}
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {
	Node* current = head->next; // start at first real node

    while (current != nullptr) { // while list isn't empty & not at end
        if (current->bid.bidId == bidId) { // if match found)
            return current->bid; // return the bid
        }
        current = current->next; // else move to next node
    }

    //(the next two statements will only execute if search item is not found)
	Bid bid; // create new empty bid
	return bid; //return empty bid 
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

void LinkedList::SaveCSV(const string& path) const
{
    std::ofstream file(path);
    if (!file) return;
	file << "Bid Id,Title,Fund,Amount\n"; // CSV header
	file << std::fixed << std::setprecision(2); // format for amount

	Node* current = head->next; // start at first real node
    while (current)
    {
	    file << current->bid.bidId << "," 
             << "\"" << current->bid.title << "\"" << "," // handle titles with commas
             << current->bid.fund << "," 
             << current->bid.amount << "\n";
		current = current->next; // move to next node
    }

}


// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount
         << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add this bid to the end
            list->Append(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98109";
    }

    clock_t ticks;

    LinkedList bidList;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  6. Prepend Bid" << endl;
        cout << "  7. Get Size" << endl;
		cout << "  8. Save Bids to CSV" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bid = getBid();
            bidList.Append(bid);
            displayBid(bid);
            break;

        case 2:
			cout << "Enter csv file path: file.csv for instance\n";
            cin >> csvPath;
            ticks = clock();
            loadBids(csvPath, &bidList);
            cout << bidList.Size() << " bids read" << endl;
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 3:
            bidList.PrintList();
            break;

        case 4:
            //added for user to enter bid id
            cout << "Enter bid id to search for: ";
            cin >> bidKey;
            ticks = clock();
            bid = bidList.Search(bidKey);
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            if (!bid.bidId.empty()) {
                displayBid(bid);
            }
            else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 5:
            cout << "Enter bid id to remove: ";
            cin >> bidKey;
            bidList.Remove(bidKey);
            break;

        case 6: // append bid
            bid = getBid();
            bidList.Prepend(bid);
            displayBid(bid);
            break;

        case 7: // get size
            cout << "Size: " << bidList.Size() << " bids" << endl;
            break;
        case 8: // save to CSV
            cout << "Saving bids to CSV file bids_saved.csv\n";
            bidList.SaveCSV("bids_saved.csv");
        }
    }
    cout << "Good bye." << endl;

    return 0;
}
