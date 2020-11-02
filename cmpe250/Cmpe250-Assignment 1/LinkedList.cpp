#include "LinkedList.h"

LinkedList::LinkedList(){

    this->length = 0 ;
}
//copy constructor of Linkedlist class
LinkedList::LinkedList(const LinkedList& list){
    //storing head of the linked list in a node
    Node *temp = list.head;
    //initializing head of the copy
    this->head = temp;
    //copying LinkedList
    while(temp != NULL ){
        this->head = new Node(*(temp));
        temp = temp->next;
        this->head = this->head->next;
    }
}
//implementation of copy assignment operator
LinkedList& LinkedList::operator=(const LinkedList& list){

    //storing head of the linked list in a node
    Node *temp = list.head;
    //initializing head of the copy

    this->head = temp;

    //copying LinkedList and deleting the nodes that exist before
    while(temp != NULL) {
        delete this->head;
        this->head = new Node(*(temp));
        temp = temp->next;
        this->head = this->head->next;
    }
    return *this;

}
//implementation of move constructor
LinkedList::LinkedList(LinkedList&& list){
    //storing the variables in the list
    this->length = move(list.length);
    this->head = move(list.head);
    this->tail = move(list.tail);
    //moving the nodes in list
    while(list.head != NULL){
        this->head = new Node(*(list.head));
        list.head = list.head->next;
        this->head = this->head->next;
    }
    //delete the head of the list
    delete list.head;
}
//move assignment operator
LinkedList& LinkedList::operator=(LinkedList&& list){
    //storing the variables in the list
    this->length = move(list.length);
    this->head = move(list.head);
    this->tail = move(list.tail);

    //moving the nodes in list
    while(list.head != NULL){
        this->head = new Node(*(list.head));
        list.head = list.head->next;
        this->head = this->head->next;
    }
    //delete the head of the list
    delete list.head;

    return *this;
}

// add a new element to the back of LinkedList
void LinkedList::pushTail(string _name, float _amount){
    //increasing the length of LinkedList
    this->length++ ;
    //if LinkedList is empty initializing head and tail
    if(!head) {
        head = new Node(_name, _amount);
        tail = head ;
        return;
    }
    //else add new node to the tail
    this->tail->next = new Node(_name, _amount);
    this->tail = tail->next ;



}

// update an existing element
void LinkedList::updateNode(string _name, float _amount){
    //take head of the list to operate on LinkedList
    Node *ptr = head;
    //trace all nodes if a node is already exist then update it
    while(ptr != NULL){

        if(ptr->name == _name){
            ptr->amount= _amount;
        }
        ptr = ptr->next;
    }
}


//Destructor of LinkedList class
LinkedList::~LinkedList(){
    if(head)
        delete head;
}
