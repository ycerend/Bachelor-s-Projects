/**
        C++ Producer Consumer using C++11 thread facilities
        To compile: g++ -std=c++11 <program name> -pthread -lpthread -o pc
*/
#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <map>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <time.h>
#include<string>  
#include<queue>  


using namespace std;
 
//custumerInfoArray keeps the information about the customer written in input file
struct customerInfoArray {
    string customer ;
    int waitTime;
    int atm ;
    string billType;
    int billAmount;
};

//billAccount keeps the information about the bills to write to the output file
struct billAccount {
    int billTypeId;
    int currentAmount;
};
//keeps the final information about accounts
map<string, billAccount> bills;
//number of consumers
const int num_consumers = 10;
//number of customers described in input file
int numberOfCustomers = 0;
//number of customers processed
int processedCustomers  = 0;

atomic<int> num_producers_working(0);       // When there's no producer working the consumers will stop, and the program will stop.
mutex xmutex;                               // Our mutex
//queue that keeps the next customers information for atm i 
vector<queue<customerInfoArray> > billsToBePaid(10);
//toWrtie keeps the information where to write output file
static queue<string> toWrite;

//splits the line from the input file
customerInfoArray shell_split_line(char line[],int customerId){
    
    customerInfoArray args;
    int bufsize = 100, position = 0;
    char **tokens = (char**)malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
    }

    token = strtok(line, ",");
    while (token != NULL) {
        tokens[position] = token;
        position++;
        //cout<< token << endl;

        if (position >= bufsize) {
            bufsize += 100;
            tokens = (char**)realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
            fprintf(stderr, "lsh: allocation error\n");
            exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, ",");
    }
    
    tokens[position] = NULL;
    args.customer = "Customer" +  to_string(customerId) ;
    args.waitTime = atoi(tokens[0]);
    args.atm = atoi(tokens[1]);
    args.billType = tokens[2];
    args.billAmount =  atoi(tokens[3]);
    return args;

}

//produces the customer arrays
void produce(customerInfoArray producer_id)
{
    if(producer_id.customer != ""){
        unique_lock<mutex> lock(xmutex);
        billsToBePaid[producer_id.atm-1].push(producer_id);

    }

}
 
//      Consume function, consumer_id will consume a product
void consume(int consumer_id)
{
    if(billsToBePaid[consumer_id].size()>0){
        unique_lock<mutex> lock(xmutex);
        customerInfoArray product;
            //cout<<consumer_id<<"  "<<billsToBePaid[consumer_id].size()<<endl;
            product = billsToBePaid[consumer_id].front();
            //cout<<"consumer " << product.billAmount<< endl; 
            string writeLine = ""+product.customer+ ","+ to_string(product.billAmount)+"TL,"+product.billType;
            bills[product.billType].currentAmount += product.billAmount;
            toWrite.push(writeLine);
            billsToBePaid[consumer_id].pop();
                processedCustomers++;
            }

    
}
 
//      Producer function, this is the body of a producer thread
void producer(customerInfoArray id)
{
        ++num_producers_working;
            struct timespec tp = {0, (long)id.waitTime*1000000};
            nanosleep(&tp, NULL);
            produce(id);

        --num_producers_working;
}
 
//      Consumer function, this is the body of a consumer thread
void consumer(int id)
{
        // Wait until there is any producer working
        while(num_producers_working == 0) this_thread::yield();
        while(num_producers_working != 0 || processedCustomers != numberOfCustomers)
        {
                consume(id);
        }
 
}
 
//
//      Main
//
int main(int argc, char *argv[])
{
    //check whether the input file is specified
    if (argc != 2) {
        cout << "Run the code with the following command: ./simulation [input_file] " << endl;
        return 1;
    }
    //initialize bill accounts
    billAccount account;
    account.billTypeId = 0;
    account.currentAmount = 0;
    bills["cableTV"] = account ;
    account.billTypeId = 1;
    account.currentAmount = 0;
    bills["electricity"] = account ;
    account.billTypeId = 2;
    account.currentAmount = 0;
    bills["gas"] =  account ;
    account.billTypeId = 3;
    account.currentAmount = 0;

    bills["telecommunication"] =account;
    account.billTypeId = 4;
    account.currentAmount = 0;
    bills["water"] = account ;
    ifstream infile(argv[1]);
    string line;
    vector<customerInfoArray> input;

    //get number of customers separately
    numberOfCustomers = 0;

    if(getline(infile, line)){
        istringstream iss(line);
        iss >> numberOfCustomers;
        //cout << "numberOfCustomers: " << numberOfCustomers << endl;  
    }
    int customerCount = 1;
    while (getline(infile, line)) {
        istringstream iss(line);
        string str;
        iss >> str;
        int n = str.length(); 
    
        // declaring character array 
        char input_array[n + 1]; 
    
        // copying the contents of the 
        // string to char array 
        strcpy(input_array, str.c_str()); 
        customerInfoArray infoToken  = shell_split_line(input_array,customerCount);
        customerCount ++ ;

        input.push_back(infoToken);
        //cout << "token: " << str << endl;  


    }

        vector<thread> producers_and_consumers;
  
        // Create consumers
        for(int i = 0; i < 10; ++i){
                producers_and_consumers.push_back(thread(consumer, i));

        }
        // Create producers
        for(int i = 0; i < numberOfCustomers; ++i){
                producers_and_consumers.push_back(thread(producer, input[i]));

        }

        // Wait for consumers and producers to finish
        for(auto& t : producers_and_consumers)
                t.join();
        string inputName = argv[1];
        string outputName = inputName.substr(0, inputName.find("."))+ "_log.txt";
        //cout<<outputName<<endl;
        ofstream outfile;
        outfile.open (outputName);
        //outfile << "Writing this to a file.\n";

        while(toWrite.size()>0){
            outfile<<toWrite.front()<<endl;
            toWrite.pop();
        }
        outfile<<"All payments are completed"<<endl;
        outfile<<"CableTV: "<<bills["cableTV"].currentAmount<<"TL"<<endl;
        outfile<<"Electricity: "<<bills["electricity"].currentAmount<<"TL"<<endl;
        outfile<<"Gas: "<<bills["gas"].currentAmount<<"TL"<<endl;
        outfile<<"Telecommunication: "<<bills["telecommunication"].currentAmount<<"TL"<<endl;
        outfile<<"Water: " << bills["water"].currentAmount<<"TL"<<endl;
        outfile.close();
 
}