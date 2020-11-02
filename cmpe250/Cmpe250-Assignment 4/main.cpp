#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <iterator>
using namespace std;

struct Edge
{
    int source_x=0,source_y=0;
    long weight=0;
    Edge(int s_x, int s_y, int w){
        source_x = s_x;
        source_y = s_y;
        weight = w;
    }
    Edge() {
        source_x=0;
        source_y=0;
        weight=0;
    }
};
struct Comparator
{
    bool operator()(const Edge& lhs, const Edge& rhs)
    {
            return lhs.weight>rhs.weight;
    }
};


long backtrace(vector<vector<Edge>> path,vector<vector<int>> vec,int source_x,int source_y,int target_x,int target_y){
    Edge e = path[target_x][target_y];
    long weight = 0,temp=0;
    while(!(e.source_x == source_x && e.source_y == source_y)){
        //cout<<"self: "<< e.source_x<< " "<<e.source_y<<" parent_X: "<<path[e.source_x][e.source_y].source_x<<"parent_Y: "<<path[e.source_x][e.source_y].source_y << "weight: "<<e.weight<<" cost: "<<cost[e.source_x][e.source_y] << endl;
        if(e.weight> weight)
            weight = e.weight;
        //temp = abs(vec[e.source_x][e.source_y]-vec[path[e.source_x][e.source_y].source_x][path[e.source_x][e.source_y].source_y]);
        //if(weight< temp)
          //  weight = temp;
        e = path[e.source_x][e.source_y];

    }
    if(e.weight> weight)
        weight = e.weight;
    return weight;

}
int prim_alg(int num_row,int num_col,int source_x,int source_y,int target_x,int target_y,vector<vector<int>> vec,vector<vector<bool>> known,vector<vector<Edge>> path){
    priority_queue<Edge, std::vector<Edge>, Comparator> edge_queue;
    //cout<<target_x<<" "<<target_y<<endl;
    edge_queue.push(Edge(source_x,source_y,0));
    long weight=0;
    while(!edge_queue.empty()){
        Edge e = edge_queue.top();
        edge_queue.pop();
        //cout<<"edge in kendisi budur popladığımız x: "<< e.source_x << " y: "<< e.source_y<< " w: "<< e.weight<<" kn: " << known[e.source_x][e.source_y]<< " p_x: "<<path[e.source_x][e.source_y].source_x <<" p_y: "<<path[e.source_x][e.source_y].source_y<< " cst: "<<cost[e.source_x][e.source_y]<<endl;

        if(e.source_x == target_x && e.source_y == target_y){
            //cout<<"bitirdik ############"<<endl;
            return backtrace(path,vec,source_x,source_y,target_x,target_y);
        }
        if(!known[e.source_x][e.source_y]){
            known[e.source_x][e.source_y] = true;
        }


        if(e.source_x-1>0){
            //cout<<e.source_x-1 << "  "<< e.source_y<<endl;
            //if(abs(vec[e.source_x-1][e.source_y]-vec[e.source_x][e.source_y]) == 4175145){
              //  cout<<"birrrr   "<<e.source_x-1 << "  "<< e.source_y<< vec[e.source_x-1][e.source_y]<<"  "<<vec[e.source_x][e.source_y]<<endl;
            //}
            if(!known[e.source_x-1][e.source_y]){
                weight = abs(vec[e.source_x-1][e.source_y]-vec[e.source_x][e.source_y]);

                if(path[e.source_x-1][e.source_y].weight> weight){
                    //cout<<"girdi 1"<<"  "<< weight<<endl;
                    edge_queue.push(Edge(e.source_x-1,e.source_y,weight));
                    path[e.source_x-1][e.source_y].source_x = e.source_x;
                    path[e.source_x-1][e.source_y].source_y = e.source_y;
                    path[e.source_x-1][e.source_y].weight = weight;
                }
           }
        }
        if(e.source_x+1<=num_row){
            //cout<<e.source_x+1 << "  "<< e.source_y<< "  "<< e.weight<<endl;
  //          if(abs(vec[e.source_x+1][e.source_y]-vec[e.source_x][e.source_y]) == 4175145){
    //            cout<<"ikiiii  "<<e.source_x+1 << "  "<< e.source_y<< "  "<< e.weight<<"  "<< vec[e.source_x+1][e.source_y]<<"  "<<vec[e.source_x][e.source_y]<<endl;
      //      }
            if(!known[e.source_x+1][e.source_y]){
                weight = abs(vec[e.source_x+1][e.source_y]-vec[e.source_x][e.source_y]);

                if(path[e.source_x+1][e.source_y].weight>weight) {
                    //cout<<"girdi 2"<<"  "<< weight<<endl;
                    edge_queue.push(Edge(e.source_x + 1, e.source_y, weight));
                    path[e.source_x + 1][e.source_y].source_x = e.source_x;
                    path[e.source_x + 1][e.source_y].source_y = e.source_y;
                    path[e.source_x + 1][e.source_y].weight = weight;
                }
            }
        }
        if(e.source_y-1>0){
            //cout<<e.source_x << "  "<< e.source_y-1<< "  "<< e.weight<<endl;
            //if(abs(vec[e.source_x][e.source_y-1]-vec[e.source_x][e.source_y] )== 4175145){
              //  cout<<"üççççç  "<<e.source_x << "  "<< e.source_y-1<< "  "<< e.weight<<"  "<< vec[e.source_x][e.source_y-1]<<"  "<<vec[e.source_x][e.source_y]<<endl;
            //}
            if(!known[e.source_x][e.source_y-1]){
                weight = abs(vec[e.source_x][e.source_y-1]-vec[e.source_x][e.source_y]);

                if(path[e.source_x][e.source_y-1].weight>weight){
                    //cout<<"girdi 3"<<"  "<< weight<<endl;
                    edge_queue.push(Edge(e.source_x,e.source_y-1,weight));
                    path[e.source_x][e.source_y-1].source_x = e.source_x;
                    path[e.source_x][e.source_y-1].source_y = e.source_y;
                    path[e.source_x][e.source_y-1].weight = weight;
                }
            }
        }
        if(e.source_y+1<=num_col){
            //cout<<e.source_x << "  "<< e.source_y+1<< "  "<< e.weight<<endl;
     //       if(abs(vec[e.source_x][e.source_y+1]-vec[e.source_x][e.source_y]) == 4175145){
       //         cout<<"dörrttt  " <<e.source_x << "  "<< e.source_y+1<< "  "<< e.weight<<"  " <<vec[e.source_x][e.source_y+1]<<"  "<<vec[e.source_x][e.source_y]<<endl;
         //   }
            if(!known[e.source_x][e.source_y+1]){

                weight = abs(vec[e.source_x][e.source_y+1]-vec[e.source_x][e.source_y]);

                if(path[e.source_x][e.source_y+1].weight> weight){
                    //cout<<"girdi 4"<<"  "<< weight<<endl;
                    edge_queue.push(Edge(e.source_x,e.source_y+1,weight));
                    path[e.source_x][e.source_y+1].source_x = e.source_x;
                    path[e.source_x][e.source_y+1].source_y = e.source_y;
                    path[e.source_x][e.source_y+1].weight = weight;
                }
            }
        }

    }



    cout<< source_x<< "  "<<source_y<<endl;
    return 0;
}
int main(int argc, char* argv[]) {


    if (argc != 3) {
        return 1;
    }

    int num_col =0, num_row = 0;
    ifstream in(argv[1]);
    in >> num_row >> num_col ;

    vector<vector<int>> vec(num_row+1, vector<int> (num_col+1, 0));
    vector<vector<bool>> known(num_row+1, vector<bool> (num_col+1, 0));
    vector<vector<Edge>> path = vector<vector<Edge>> (num_row+1, vector<Edge>(num_col+1));
    for(int i=1 ; i<=num_row ; i++){
        for(int j=1 ; j <=num_col ; j++){
            in>> vec[i][j];
            known[i][j]= false;
            path[i][j] = Edge(0,0,INT_MAX);
        }
    }
    int source_x =0 ,source_y = 0;
    int target_x = 0 , target_y = 0;
    int query_count = 0;
    in>> query_count ;
    ofstream myfile;
    myfile.open (argv[2]);
    for(int i = 0 ; i< query_count ; i++){
        in>>source_x >> source_y >> target_x >> target_y ;
        myfile <<prim_alg(num_row,num_col,source_x,source_y,target_x,target_y,vec,known,path)<<endl;
    }
    myfile.close();
    return 0;
}

