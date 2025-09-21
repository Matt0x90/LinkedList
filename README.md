## DSA Class Assignment:

This assignments purpose is to create a singly linked list to handle bid data loaded from a file. I use a dummy head node and adjusted the design as I implemented it. The Node structure holds the bid data and a pointer to the next node. There is a default constructor for creating the dummy node and a constructor for initializing with bid data.

Pseudocode for singly linked list,
```
Class SinglyLinkedList
   private:
      node head // dummy head node
      node tail // last real node or haed if empty
      integer size // for tracking size of list
   public:
      constructor() // house keeping variables
      destructor() // clean up memory
      append(bid) // append bid data to end of list
      prepend(bid) // prepend bid to front of list
      printList() // print all bid entries to console
      remove(bidId) // remove requested bid using bid ID
      search(bidId) // Search for bid ID, return bid structure if found
      size() // return number of elements/size in list.
```
The constructor needs to initialize the head and tail nodes to the dummy node,
```
Constructor()
   head = new Node(emptyData)
   tail = head
```

while the destructor would look like this.
```
Destructor()
   current = head node
   while current isn't empty or equal to nullptr (after the tail)
      next = current.next // this holds on the next node
      delete current node
      current = next node
```

We next had to add the core operations: append, prepend, printlist, remove, and search.

Starting with append, this operation is O(1) since a tail pointer is maintained. I also can skip the conditional checks since I'm using a dummy pointer for head/tail.

```
Append(bid)
   newNode = allocate new Node(bid) // creating new data node
   tail.next = newNode // link last node to new one
   tail = newNode // update tail pointer
   size = size + 1
```

With prepend, we're adding a new node at the start of the list directly after the dummy node, this too is an O(1) operation.

```
Prepend(bid)
   newNode = allocate new Node(bid) // create new data node
   newNode.next = head.next // new node points to current first node
   head.next = newNode // dummy node now points to new node
   if tail = head then // if list is empty
      tail = newNode // new node is also updated for tail
   size = size + 1
```

After prepend, we have print list for printing the list's content which is just a simple traversal.

```
printList()
   current = head.next // head node after dummy
   while current is not null // while it isn't empty and isn't at the end
      print current.bid
      current = current.next // iterate to next node
```

Then the final methods, removing and searching for a node with a specified key or bid ID. This requires traversing the list which is O(n). With an additional 'previous' pointer you can relink nodes.

```
remove(bidId)
   prev = head
   current = head.next
   while current isn't empty/null
      if current.bid.id == bidId then
         prev.next = current.next
         if current = tail then
            tail = prev
        delete current
        size = size -1
        return true
       prev = current
       current = current.next
   return false otherwise (no key found)
```

```
search(bidId)
   current = head.next // starting after dummy node
   while current isn't null (empty)
      if current.bid.id = bidId then
         return current.bid // bid id found
      current = current.next // go next node until the end of the list
   return null // not found 
```

After I finished adding the required functionality, I noticed the menu was missing options for prepend, so I added that in to the menu as a switch case. While I was at it I also added in get size since it took only a few seconds. 

Then I started testing it and noticed it's not requesting input for a bid id on search or remove, which is a problem in how the switch case is handled inside the menu from the code provided to us. It doesn't request user input for the bid to remove or find. 

I also noticed that it isn't saving the changes we make, for example removing, appending, or prepending bid data doesn't last across sessions. I added a new method for saving the CSV file data, and an additional menu option for saving the file. I also modified the load data to allow the user to input a specific csv file, e.g. data.csv. 
