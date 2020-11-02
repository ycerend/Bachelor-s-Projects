#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <iterator>
using namespace std;


struct Passenger {
    double arrive_time_lug,arrive_time_sec, flight_time, luggage_time, security_time,departure_time;
    bool is_vip, is_lug_exists;

    Passenger(double arrive_time_lug_, double arrive_time_sec_,double flight_time_, double luggage_time_, double security_time_,double departure_time_, bool is_vip_,
              bool is_lug_exists_) {
        arrive_time_lug = arrive_time_lug_;
        arrive_time_sec = arrive_time_sec_;
        flight_time = flight_time_;
        luggage_time = luggage_time_;
        security_time = security_time_;
        departure_time = departure_time_;
        is_vip = is_vip_;
        is_lug_exists = is_lug_exists_;
    }

    Passenger() {
        arrive_time_lug = 0;
        flight_time = 0;
        luggage_time = 0;
        security_time = 0;
        departure_time = 0;
        is_vip = false;
        is_lug_exists = false;
    }
};
struct Comparator1
{
    bool operator()(const Passenger& lhs, const Passenger& rhs)
    {
        return lhs.arrive_time_lug>rhs.arrive_time_lug;
    }
};

struct Comparator2
{
    bool operator()(const Passenger& lhs, const Passenger& rhs)
    {
        return lhs.arrive_time_sec>rhs.arrive_time_sec;
    }
};

struct Comparator3
{
    bool operator()(const Passenger& lhs, const Passenger& rhs)
    {
        if(lhs.flight_time == rhs.flight_time){
            return lhs.arrive_time_lug>rhs.arrive_time_lug;
        }
        return (lhs.flight_time > rhs.flight_time);
    }
};

struct Comparator4
{
    bool operator()(const int& lhs, const int& rhs)
    {
        return lhs > rhs;
    }
};

struct Comparator5
{
    bool operator()(const Passenger& lhs, const Passenger& rhs)
    {

            if(lhs.flight_time == rhs.flight_time){
                return lhs.arrive_time_sec>rhs.arrive_time_sec;
            }
            return (lhs.flight_time > rhs.flight_time);

    }
};

struct CaseX{
    int time;
    int vector_count;
    double average_time;
    int people_late_count ;
    bool vip_check;
    bool lug_check;
    std::priority_queue<Passenger, std::vector<Passenger>, Comparator1> lug_queue;
    std::priority_queue<Passenger, std::vector<Passenger>, Comparator2> sec_queue;
    CaseX(int time_, int vector_count_, double average_time_, int people_late_count_,bool vip_check_, bool lug_check_, priority_queue<Passenger, std::vector<Passenger>, Comparator1> lug_queue_,priority_queue<Passenger, std::vector<Passenger>, Comparator2> sec_queue_) {
        time = time_;
        vector_count = vector_count_;
        average_time = average_time_;
        people_late_count = people_late_count_;
        vip_check = vip_check_;
        lug_check = lug_check_;
        lug_queue = lug_queue_;
        sec_queue = sec_queue_;

    }

    CaseX() {
        time = 0;
        vector_count = 0;
        average_time = 0;
        people_late_count = 0;
        vip_check = false;
        lug_check = false;
        lug_queue = {};
        sec_queue = {};
    }

};

struct CaseY{
    int time;
    int vector_count;
    double average_time;
    int people_late_count ;
    bool vip_check;
    bool lug_check;
    std::priority_queue<Passenger, std::vector<Passenger>, Comparator3> lug_queue;
    std::priority_queue<Passenger, std::vector<Passenger>, Comparator5> sec_queue;
    CaseY(int time_, int vector_count_, double average_time_, int people_late_count_,bool vip_check_, bool lug_check_, priority_queue<Passenger, std::vector<Passenger>, Comparator3> lug_queue_,priority_queue<Passenger, std::vector<Passenger>, Comparator5> sec_queue_) {
        time = time_;
        vector_count = vector_count_;
        average_time = average_time_;
        people_late_count = people_late_count_;
        vip_check = vip_check_;
        lug_check = lug_check_;
        lug_queue = lug_queue_;
        sec_queue = sec_queue_;

    }

    CaseY() {
        time = 0;
        vector_count = 0;
        average_time = 0;
        people_late_count = 0;
        vip_check = false;
        lug_check = false;
        lug_queue = {};
        sec_queue = {};
    }
};

void Case1_3_5_7(int num_of_passenger, CaseX& case1,std::priority_queue<Passenger, std::vector<Passenger>, Comparator1> passenger_infos,std::priority_queue<int, std::vector<int>, Comparator4> lug_check_queue, std::priority_queue<int, std::vector<int>, Comparator4> sec_check_queue ){
    while(!case1.lug_queue.empty()) {
        Passenger p = case1.lug_queue.top();
        case1.lug_queue.pop();
        int temp_time = 0;
        if(!case1.lug_check){
            int time_available = lug_check_queue.top();
            lug_check_queue.pop();
            if(time_available <= p.arrive_time_lug )
                temp_time = p.arrive_time_lug + p.luggage_time ;
            else
                temp_time = p.luggage_time +p.arrive_time_lug +(time_available - p.arrive_time_lug );
            p.arrive_time_sec= temp_time;
            case1.time = temp_time;

            lug_check_queue.push(temp_time);
        }
        else{
            if(!p.is_lug_exists){
                p.arrive_time_sec = p.arrive_time_lug;
                temp_time = p.arrive_time_sec;
            }
            else{
                int time_available = lug_check_queue.top();
                lug_check_queue.pop();
                if(time_available <= p.arrive_time_lug )
                    temp_time = p.arrive_time_lug + p.luggage_time ;
                else
                    temp_time = p.luggage_time +p.arrive_time_lug +(time_available - p.arrive_time_lug );
                p.arrive_time_sec= temp_time;
                case1.time = temp_time;

                lug_check_queue.push(temp_time);
            }
        }


        if(!case1.vip_check)
            case1.sec_queue.push(p);
        else{
            if(p.is_vip){

                p.departure_time = temp_time;
                if(temp_time> p.flight_time){
                    case1.people_late_count++;

                }
                case1.average_time += p.departure_time - p.arrive_time_lug ;
            }
            else{
                case1.sec_queue.push(p);
            }
        }
        while(((passenger_infos.top().arrive_time_lug <= lug_check_queue.top()) || case1.lug_queue.empty()) && !passenger_infos.empty()){
            case1.lug_queue.push(passenger_infos.top());
            passenger_infos.pop();
        }


    }
    while(!case1.sec_queue.empty()){
        Passenger p = case1.sec_queue.top();

        int time_available2 = sec_check_queue.top();
        sec_check_queue.pop();
        case1.sec_queue.pop();
        int temp_time2 = 0;
        if(time_available2 <= p.arrive_time_sec )
            temp_time2 = p.arrive_time_sec + p.security_time ;
        else
            temp_time2 = p.security_time + p.arrive_time_sec + (time_available2- p.arrive_time_sec );
        p.departure_time= temp_time2;
        sec_check_queue.push(temp_time2);
        if(temp_time2> p.flight_time){
            case1.people_late_count++;

        }
        case1.average_time += p.departure_time - p.arrive_time_lug ;
    }

}

void Case2_4_6_8(int num_of_passenger, CaseY& case2, std::priority_queue<Passenger, std::vector<Passenger>, Comparator1> passenger_infos,std::priority_queue<int, std::vector<int>, Comparator4> lug_check_queue, std::priority_queue<int, std::vector<int>, Comparator4> sec_check_queue ){
    std::priority_queue<Passenger, std::vector<Passenger>, Comparator2> sec_queue_2;

    while(!case2.lug_queue.empty()) {
        Passenger p = case2.lug_queue.top();
        case2.lug_queue.pop();
        int temp_time = 0;
        if(!case2.lug_check){
            int time_available = lug_check_queue.top();
            lug_check_queue.pop();
            if(time_available < p.arrive_time_lug ){
                temp_time = p.arrive_time_lug + p.luggage_time ;

            }

            else
                temp_time = p.luggage_time +p.arrive_time_lug +(time_available - p.arrive_time_lug );
            p.arrive_time_sec= temp_time;
            case2.time = temp_time;

            lug_check_queue.push(temp_time);
        }
        else{
            if(!p.is_lug_exists){
                p.arrive_time_sec = p.arrive_time_lug;
                temp_time = p.arrive_time_sec;
            }
            else{
                int time_available = lug_check_queue.top();
                lug_check_queue.pop();
                if(time_available <= p.arrive_time_lug )
                    temp_time = p.arrive_time_lug + p.luggage_time ;
                else
                    temp_time = p.luggage_time +p.arrive_time_lug +(time_available - p.arrive_time_lug ) ;
                p.arrive_time_sec= temp_time;
                case2.time = temp_time;

                lug_check_queue.push(temp_time);
            }
        }

        if(!case2.vip_check)
            sec_queue_2.push(p);
        else{
            if(p.is_vip){

                p.departure_time = temp_time;
                if(p.departure_time> p.flight_time){
                    case2.people_late_count++;

                }
                case2.average_time += p.departure_time - p.arrive_time_lug ;
            }
            else{
                sec_queue_2.push(p);
            }
        }
        while(((passenger_infos.top().arrive_time_lug <= lug_check_queue.top()) || case2.lug_queue.empty())&& !passenger_infos.empty()){
            case2.lug_queue.push(passenger_infos.top());
            passenger_infos.pop();
        }



    }
    case2.sec_queue.push(sec_queue_2.top());
    sec_queue_2.pop();
    while(!case2.sec_queue.empty()){
        Passenger p = case2.sec_queue.top();

        int time_available2 = sec_check_queue.top();
        sec_check_queue.pop();
        case2.sec_queue.pop();
        int temp_time2 = 0;
        if(time_available2 < p.arrive_time_sec )
            temp_time2 = p.arrive_time_sec + p.security_time ;
        else
            temp_time2 =  p.security_time + p.arrive_time_sec + (time_available2- p.arrive_time_sec );
        p.departure_time= temp_time2;
        sec_check_queue.push(temp_time2);
        if(p.departure_time> p.flight_time){

            case2.people_late_count++;

        }
        case2.average_time += p.departure_time - p.arrive_time_lug ;



        while(((sec_queue_2.top().arrive_time_sec <= sec_check_queue.top()) || case2.sec_queue.empty())&& !sec_queue_2.empty()){
            case2.sec_queue.push(sec_queue_2.top());
            sec_queue_2.pop();

        }


    }





}

int main(int argc, char* argv[]) {
    // below reads the input file
    // in your next projects, you will implement that part as well
    if (argc != 3) {
        return 1;
    }

    int num_of_passenger = 0,num_luggage = 0, num_security = 0 ;
    double arrive_time =0 , flight_time = 0 , luggage_time = 0 , security_time = 0 ;
    char vip_info = 'N';
    char lug_info = 'N';
    bool is_vip = false, is_lug_exists = false ;
    ifstream in(argv[1]);
    in >> num_of_passenger >> num_luggage >> num_security;


    std::priority_queue<Passenger, std::vector<Passenger>, Comparator1> passenger_infos;
    if(num_of_passenger != 0){
        for(int i = 0;i<num_of_passenger;i++){
            in >> arrive_time >> flight_time>> luggage_time >> security_time;
            in >> vip_info >> lug_info ;
            if(vip_info == 'V'){
                is_vip = true;
            }
            else{
                is_vip = false;
            }
            if(lug_info == 'L'){
                is_lug_exists = true ;
            }
            else{
                is_lug_exists = false ;
            }
            passenger_infos.push(Passenger(arrive_time,0,flight_time,luggage_time,security_time,0,is_vip,is_lug_exists));


        }
    }

    std::priority_queue<int, std::vector<int>, Comparator4> lug_check_queue;
    std::priority_queue<int, std::vector<int>, Comparator4> sec_check_queue;




    for(int i = 0 ; i< num_luggage; i++){
        lug_check_queue.push(0);
    }
    for(int i = 0 ; i< num_security; i++){
        sec_check_queue.push(0);
    }

    CaseX case1 = CaseX(0,0,0,0,false,false, {},{});
    case1.lug_queue.push(passenger_infos.top());


    CaseY case2 = CaseY(0,0,0,0,false,false,{},{});
    case2.lug_queue.push(passenger_infos.top());


    CaseX case3 = CaseX(0,0,0,0,true,false, {},{});
    case3.lug_queue.push(passenger_infos.top());

    CaseY case4 = CaseY(0,0,0,0,true,false,{},{});
    case4.lug_queue.push(passenger_infos.top());

    CaseX case5 = CaseX(0,0,0,0,false, true, {},{});
    case5.lug_queue.push(passenger_infos.top());


    CaseY case6 = CaseY(0,0,0,0,false,true,{},{});
    case6.lug_queue.push(passenger_infos.top());


    CaseX case7 = CaseX(0,0,0,0,true, true, {},{});
    case7.lug_queue.push(passenger_infos.top());


    CaseY case8 = CaseY(0,0,0,0,true,true,{},{});
    case8.lug_queue.push(passenger_infos.top());
    passenger_infos.pop();

    Case1_3_5_7(num_of_passenger,case1,passenger_infos,lug_check_queue,sec_check_queue);
    Case2_4_6_8(num_of_passenger,case2,passenger_infos,lug_check_queue,sec_check_queue);
    Case1_3_5_7(num_of_passenger,case3,passenger_infos,lug_check_queue,sec_check_queue);
    Case2_4_6_8(num_of_passenger,case4,passenger_infos,lug_check_queue,sec_check_queue);
    Case1_3_5_7(num_of_passenger,case5,passenger_infos,lug_check_queue,sec_check_queue);
    Case2_4_6_8(num_of_passenger,case6,passenger_infos,lug_check_queue,sec_check_queue);
    Case1_3_5_7(num_of_passenger,case7,passenger_infos,lug_check_queue,sec_check_queue);
    Case2_4_6_8(num_of_passenger,case8,passenger_infos,lug_check_queue,sec_check_queue);


    ofstream myfile;
    myfile.open (argv[2]);
    myfile<< (case1.average_time/num_of_passenger) << "  " << case1.people_late_count<<endl;
    myfile<< (case2.average_time/num_of_passenger) << "  " << case2.people_late_count<<endl;
    myfile<< (case3.average_time/num_of_passenger) << "  " << case3.people_late_count<<endl;
    myfile<< (case4.average_time/num_of_passenger) << "  " << case4.people_late_count<<endl;
    myfile<< (case5.average_time/num_of_passenger) << "  " << case5.people_late_count<<endl;
    myfile<< (case6.average_time/num_of_passenger) << "  " << case6.people_late_count<<endl;
    myfile<< (case7.average_time/num_of_passenger) << "  " << case7.people_late_count<<endl;
    myfile<< (case8.average_time/(num_of_passenger)) << "  " << case8.people_late_count<<endl;
    myfile.close();
    return 0;
}