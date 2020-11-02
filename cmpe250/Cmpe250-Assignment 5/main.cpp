#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <map>
using namespace std;

int word_break(int start_index,int input_end,vector<unsigned long long> dict,char *input_char_array,vector<unsigned long long> &myBoolArray,map<char, int> map){

    if(myBoolArray[start_index]==-1) {
        unsigned long long value = 0;
        unsigned long long possibility = 0;
        int count=0;
        for (int i = start_index; i < input_end; i++) {
            //cout<<"#################################################################   "<< start_index<<endl;
            //cout<<"value:  "<<value<<"    " <<"   "<< input_char_array[i]<<  "   "<<s<< endl;
            unsigned long long base= pow(29,(i-start_index-count))*map[input_char_array[i]];
            base = base % 1000000007;
            value += base;
            value = value % 1000000007 ;
            cout<<"     value:  "<<value<<"   "<<input_char_array[i]<<"   " <<(int)input_char_array[i] << "--"<<(input_char_array[i]-96)<<"   "<<base<<   "    "<<(i-start_index+1)<<endl;
            for (int j = 0; j < dict.size(); j++) {
                unsigned long long dict_value = dict[j];
                //cout<<"gidiyoz"<<dict[j]<<"   "<<j<<"   "<< start_index <<"  "<<i + 1<<endl;
                if (value == dict_value) {
                    //cout<<"gidiyoz"<<dict[j]<<"   "<<j<<"   "<< start_index <<"  "<<i + 1<<endl;
                    possibility += word_break(i + 1, input_end, dict, input_char_array, myBoolArray, map);

                    //cout<<"Biiiiiiirrrr    " <<"dict value:   " <<dict[j]<<"   "<<j<<"   "<< start_index <<"  "<<i + 1<< "   possibility"<<possibility<<endl;

                }

            }

            value *= 29;
            value = value % 1000000007 ;
            count++;

        }


        myBoolArray[start_index]=(possibility% 1000000007);
        //cout<<start_index+1 << "   "<< possibility<<endl;
        return myBoolArray[start_index];
        }else{
        //cout<<"    sooooon   "<<start_index<< "   "<<myBoolArray[start_index];
        return myBoolArray[start_index];

        }


}
// Word Break Problem
int main(int argc, char* argv[])
{//abcdefghijklmnopqrstuvwxyz
    map<char, int> m = {{'a', 101},{'b', 3},{'c', 5},{'d', 7},{'e', 11},{'f', 13},{'g', 17},{'h', 19},{'i', 23},{'j', 29},{'k', 31},{'l', 37},{'m', 41},{'o', 43},{'p', 47},{'r', 53},{'s', 59},{'t', 61},{'u', 67},{'v', 71},{'w', 73},{'x', 79},{'y', 83},{'z', 89}};
    if (argc != 3) {
        return 1;
    }
    ifstream in(argv[1]);
    string input= "";
    in>>input;
    int input_size = input.length();
    char *input_char_array = new char [input_size+1];
    strcpy (input_char_array, input.c_str()); //make input string to be char array
    //cout<<input_char_array[0]<<input_char_array[1]<<endl;
    int size_of_Array = 0;
    in>> size_of_Array;
    vector<unsigned long long> dict(size_of_Array);
    cout<<size_of_Array<<"   "<<input_size<<endl;
    string s="";
    vector<unsigned long long int> input_vec(input_size);
    unsigned long long value1 = 0;
    //unsigned long long base1 = 0;
    string s1="";
    int start_index=0;
    for(int i=0 ; i<input_size; i++){
        unsigned long long base1= (m[input.c_str()[i]]);
        base1 = base1 % 1000000007;
        if(base1<0)
            base1 = base1 + 1000000007;
        value1 += base1;


        value1 = value1 % 1000000007 ;
        if(value1<0)
            value1 = value1 + 1000000007;
        s1 += input_char_array[i];
        input_vec[i]= value1;
        //cout<<i<< "    "<<s << "  "<<input_vec[i]<<"  "<<endl;
        value1 *= 29;
        value1 = value1 % 1000000007 ;

    }
    cout<<s1 << "    last name    "<<input_vec[input_size-1]<<endl;
    for (int i = 0; i < size_of_Array; i++)
    {
        string dictionary = "";
        in>> dictionary;
        char *dict_array = new char [dictionary.length()+1];
        strcpy (dict_array, dictionary.c_str());
        unsigned long long value=0;

        //mcout<<"number:   "<<i<<"    "<< strlen(dict_array)<<"   ";
        for( int j = 0; j< strlen(dict_array); j++){
            unsigned long long base= pow(29,(strlen(dict_array)-j-1))*m[dictionary.c_str()[j]];
            s +=dictionary.c_str()[j];
            value += base;
            if(value<0){
                value = value +1000000007;
            }
            //cout<< (int)dictionary.c_str()[j]<<"  "<<m[dictionary.c_str()[j]]<<"    "<< value<< "  "<<base<< "     ";
        }
        dict[i]= value  % 1000000007 ;
        cout<<"s "<<s<<endl;
        //cout<< "bitti "<<dictionary<< "   "<< dict[i]<<endl;
    }
    vector<unsigned long long> myBoolArray(input_size+1, -1);
    myBoolArray[input_size] =1;
    int possibility =word_break(0,input_size,dict,input_char_array,myBoolArray,m);

    unsigned long long result=myBoolArray[0]% 1000000007;
    ofstream myfile;
    myfile.open (argv[2]);
    myfile <<result<<endl;

    myfile.close();


    return 0;
}