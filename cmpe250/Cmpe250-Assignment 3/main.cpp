#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <iterator>
using namespace std;



int main(int argc, char* argv[]) {
    // below reads the input file
    // in your next projects, you will implement that part as well
    if (argc != 3) {
        return 1;
    }

    int num_of_banks = 0;
    int number_of_keys = 0;
    int to = 0;
    ifstream in(argv[1]);
    in >> num_of_banks;

    vector<vector<int>> banks_info(num_of_banks+1);
    vector<int> degrees(num_of_banks+1);
    for(int i=0;i<= num_of_banks;i++)
        degrees[i]=0;
    if(num_of_banks != 0){
        for(int i = 0;i<num_of_banks;i++){
            int from = i+1;
            in >> number_of_keys;
            //cout<< number_of_keys<< endl;
            for(int j = 0 ; j < number_of_keys; j++){
                in>> to;
                banks_info[from].push_back(to);
                if(from != to)
                    degrees[to]++;
            }


        }
    }
    queue<int> notReachable;
    vector<int> to_be_cracked;
    vector<bool> visited_vector(num_of_banks+1);
    int zero_count = 0;
    for(int i=1; i<= num_of_banks;i++){
        if(degrees[i] == 0){
            notReachable.push(i);
            zero_count++;
        }

    }
    if(zero_count == 0){
        notReachable.push(1);
        to_be_cracked.push_back(1);
    }

    for(int i=1; i<= num_of_banks;i++){
        visited_vector[i]= false;
    }
    while(!notReachable.empty()){
        int bank = notReachable.front();
        notReachable.pop();
        visited_vector[bank] = true;
        //cout<< "now : " << bank<< endl;
        if(degrees[bank] == 0){
            to_be_cracked.push_back(bank);
        }
        for(int i=0 ;i < banks_info[bank].size();i++){
            if(!visited_vector[banks_info[bank][i]]){
                notReachable.push(banks_info[bank][i]);
            }

        }
    }
    ofstream myfile;
    myfile.open (argv[2]);
    myfile<< to_be_cracked.size() << " ";
    for(int i=0; i<to_be_cracked.size();i++){

            myfile<< to_be_cracked[i] << " ";

    }
    myfile.close();
    return 0;
}