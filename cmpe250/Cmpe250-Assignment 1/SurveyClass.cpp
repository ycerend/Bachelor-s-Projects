#include "SurveyClass.h"

//copy constuctor of SurveyClass
SurveyClass::SurveyClass(){
    //initalizing a new linkedlist for the class
    members = new LinkedList;
}
//copy constructor of SurveyClass
SurveyClass::SurveyClass(const SurveyClass& other){
    //initializing a new LinkedList
    members = new LinkedList;
    //copying head of the other
    Node *temp = other.members->head;
    //copying nodes from the other
    while(temp != NULL ){
        members->head = new Node(*(temp));
        temp = temp->next;
        members->head = members->head->next;
    }

}

//copy assignment operator of SurveyClass
SurveyClass& SurveyClass::operator=(const SurveyClass& list){
    //taking head of the list to operate on the list
    Node *temp = list.members->head;
    members->head = temp;
    members->tail = list.members->tail;
    //copying nodes from the list
    while(temp != NULL) {
        delete members->head;
        members->head = new Node(*(temp));
        temp = temp->next;
        members->head = members->head->next;
    }
    return *this;

}
//move constructor operator of SurveyClass
SurveyClass::SurveyClass(SurveyClass&& other){
    //taking head of the list to operate on the list
    Node *temp = other.members->head;
    //moving variables from the other list
    members->length = move(other.members->length);
    members->head = move(other.members->head);
    members->tail = move(other.members->tail);
    //moving noes from the list
    while(temp != NULL){
        members->head = new Node(*(temp));
        temp = temp->next;
        members->head = members->head->next;
    }
    //deleting the other LinkedList
    delete other.members;

}
//move assignment operator of SurveyClass
SurveyClass& SurveyClass::operator=(SurveyClass&& list){
    //taking head of the list to operate on the list

    Node *temp = list.members->head;
    //moving variables from the other list
    members->length = move(list.members->length);
    members->head = move(list.members->head);
    members->tail = move(list.members->tail);
    //moving noes from the list
    while(temp != NULL){
        members->head = new Node(*(temp));
        temp = temp->next;
        members->head = members->head->next;
    }
    //deleting the other LinkedList
    delete list.members;

    return *this;
}
//destructor of SurveyClass
SurveyClass::~SurveyClass(){

    if(members) delete members;

}
//calculates all of the possible calculations like maximum,minimum and avreage and returns the operation that is asked to
float AllCalculation(int whichOne, LinkedList* members){
    //taking head of the list to operate on list
    Node *temp = members->head;
    //initializing max, min and average
    float max= temp->amount;
    float min= temp->amount;
    float average = 0;
    //takes the amount of the nodes
    int count=0;
    //traces all of the nodes on the list to calculate min, max or average
    while(temp != NULL){
        //increasing count by 1
        count++;
        //determining if the node's amount is bigger than max value before that node
        if(max < temp->amount){
            max = temp->amount;
        }
        if(min > temp->amount){ //determining if the node's amount is bigger than max value before that node
            min = temp->amount;
        }
        //adding the amoun of the node
        average += (temp->amount);


        temp = temp->next;
    }
    //if the asked operation is min, return min
    if(whichOne ==0){
        return min;
    }
        //if the asked operation is max, return max
    else if(whichOne ==1){
        return max;
    }
        //if the asked operation is average, arrange 2 decimals, return average
    else {

        return ((int(average*100))/count)/100.0;
    }

}
// Adds a new Node to the linked list or updates the corresponding Node in the linked list
void SurveyClass::handleNewRecord(string _name, float _amount){
    //taking head of the list to operate on list
    Node *ptr = members->head;
    //to check node is existing before with same name
    bool isExist = false;
    //check existence of the node
    while(ptr != NULL){
        //if exists,update the node
        if(ptr->name == _name){
            members->updateNode(_name,_amount);
            isExist = true;
        }
        ptr = ptr->next;
    }
    //if not exists, add new node
    if(!isExist){
        members->pushTail(_name,_amount);

    }
}
// Calculates and returns the minimum amount of expense.
// The minimum amount can have up to two decimal points.
float SurveyClass::calculateMinimumExpense(){
    //calculates min
    return AllCalculation(0,members);
}
// Calculates and returns the maximum amount of expense.
// The maximum amount can have up to two decimal points.
float SurveyClass::calculateMaximumExpense(){
    //calculates max
    return AllCalculation(1,members);
}
// Calculates and returns the average amount of expense.
// The average amount can have up to two decimal points.
float SurveyClass::calculateAverageExpense(){
    //calculates average
    return AllCalculation(2,members);
}