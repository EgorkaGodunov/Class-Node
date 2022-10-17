#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Node{
    public:
        int name;
        int parent;
        vector<int> connects;
        int visited = 0;
        Node(){}
        Node(int n){
            name = n;
        }
        void add(int connection){
            connects.push_back(connection);
            
        }
        int size(){
            return connects.size();
        }
        void self_print(){
            cout<<name<<": ";

        }
        vector<int> get_connections(){
            return connects;
        }      
};


template<class C, typename T>
bool contains(C&& c, T t) {
    return find(begin(c), end(c), t) != end(c);
};
 
vector<Node*> add_nodes(vector<Node*> nodes,vector<vector<int>> connects){
    vector<int> unique_nods;
    for(auto &line : connects)
    {   
        int i=0;
        do{
            bool exists = contains(unique_nods,line[i]);
            if (!exists && i!=2){
                unique_nods.push_back(line[i]);
                nodes.push_back(new Node(line[i]));
            }
            i+=1;
            exists=false;
        }while(i<line.size());
    }
    return nodes;
}

vector<Node*> nodes;
vector<vector<int>> connects;

Node* get_node_by_number(vector<Node*> nodes,int num){
    for(int i=0;i< nodes.size();i++){
        if( nodes[i]->name==num){
            return  nodes[i];
        } 
           
    }
    return 0;
}
vector<Node*> add_connections(vector<vector<int>> connections,vector<Node*> ac=add_nodes(nodes,connects)){
    // correct connections form {from,to,weight}
    for(int i=0;i<ac.size();i++){
        for(auto &line : connections)
        {   
            if(ac[i]->name==line[0]){
                ac[i]->add(line[1]);
            }
        }
    }
    for(int i=0;i<ac.size();i++){
        for(auto &line : ac[i]->connects)
        {   
            get_node_by_number(ac,line)->parent=ac[i]->name;
        }
    }

    return ac;
}
void print_attached_connections(vector<Node*> nodes){
    for(int i=0;i<nodes.size();i++){
        nodes[i]->self_print();
        for(auto &line : nodes[i]->get_connections()){
            for(int l=0;l<nodes.size();l++){
                if(nodes[l]->name==line){
                    cout<<nodes[l]->name<<" - "<<nodes[l]<<" ";
                }
            }
        }
        cout<<endl;
    }
}

int dfs(vector<Node*> nodes, int next, int search){
    static bool checker = false;
    Node* this_node = get_node_by_number(nodes,next);
    if(this_node->name==search){
        cout<<"Successfully! Your element: "<<this_node->name<<endl;
        checker=true;
    }else{
        if(checker==false){
            cout<<this_node->name<<" -> ";
        }
        // check childs ------
        int check_childs=1;
        for(int m=0;m<this_node->connects.size();m++){
            if(get_node_by_number(nodes,this_node->connects[m])->visited==0){
                check_childs = check_childs*get_node_by_number(nodes,this_node->connects[m])->visited;
            }
        }
        //-------
        if(this_node->size()>0 && check_childs==0){
            if(check_childs==0){
                for(int i=0;i<this_node->size();i++){
                    Node* next_node=get_node_by_number(nodes,this_node->connects[i]);
                    if(next_node->visited==0){
                        next_node->visited=1;
                        dfs(nodes,next_node->name,search);
                    }
                }
            }
            
        }else{
            dfs(nodes,this_node->parent,search);
        }
    }
    return 0;
}


int main(){
    cout<<"Type the file name: ";
    string file;
    cin >> file;
    ifstream in(file);
    string line;
    int from,to,weight;
    if(in.is_open()){
         while (in >> from >> to >> weight)
        {
            connects.push_back({from,to,weight});
        }
    }
    in.close();

    nodes = add_connections(connects);
    dfs(nodes, 1, 6);
    return 0;
}
