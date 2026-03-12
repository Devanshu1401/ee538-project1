#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/********************DO NOT EDIT**********************/
// Function prototype. Defined later.
void read_opinions(string filename); // reads file into opinions vector and updates total_nodes as needed
void read_edges(string filename); // reads file into edge_list, defined later
void build_adj_matrix(); // convert edge_list to adjacency matrix

int total_nodes = 0; // We keep track of the total number of nodes based on largest node id.



/****************************************************************/

/******** Create adjacency matrix and vector of opinions */
// simple vector to hold each node's opinion (0 or 1)
vector<int> opinions;

// global adjacency matrix initialized later

vector < vector < int>> adj;

// edge list: each row contains {source, target}

vector<vector <int>> edge_list;

void build_adj_matrix()

{  
    adj.clear(); 
 
    for (int i = 0; i <total_nodes; i++ )

        {

        vector <int> x;
        adj.push_back(x);

        for ( int j = 0; j < total_nodes; j++)
            {
            

                adj[i].push_back(0);
            
            }

        }
    
    int edge_length = edge_list.size();

    for (int i = 0; i < edge_length; i ++)

    {
    int r = edge_list[i][0];
    int c = edge_list[i][1];

    adj[r][c] =  1;
    }
    
}


double calculate_fraction_of_ones()
{
   int ans = 0;
   double frac;
    for (int i = 0; i < total_nodes; i++)

   {

    if (opinions[i]!=0)
        ans = ans +1;

   }

   frac = ( double)ans/total_nodes;
   return frac ;
}

// For a given node, count majority opinion among its neighbours. Tie -> 0.
int get_majority_friend_opinions(int node)
{
    
    int majority = 0;
    int zeros= 0;
    int ones = 0; 
    
    vector <int> friends;
    

    for (int i=0; i<total_nodes; i++)

    {
        if((adj[i][node]) == 1)
            friends.push_back(i);        
    }

    int number_of_friends = friends.size();

    for (int k = 0; k<number_of_friends ; k++)

    {
        
        int frnd = friends[k];
        int op = opinions[frnd];
        if (op==1)
        {
            ones = ones +1;
        }

        else 
        {
            zeros = zeros + 1;
        }


    }

    if (ones  > zeros)
   
    {
        majority = 1;
    }
    
    return majority;

}

// Calculate new opinions for all voters and return if anyone's opinion changed
bool update_opinions()
{

vector<int> next_opinion;
int changed = 0;

for (int i=0; i < total_nodes; i++)

    
    {
        next_opinion.push_back(get_majority_friend_opinions(i));
        if (next_opinion[i] != opinions[i])
            {
                changed=1;
            }

    }


opinions = next_opinion;

if (changed == 1)
{
    return true;
}

else 
{
    return false;
}


}

int main() {
    // no preallocation; vectors grow on demand

    // Read input files
    read_opinions("opinions.txt"); 
    read_edges("edge_list.txt");

    // convert edge list into adjacency matrix once we know total_nodes
    build_adj_matrix();
    
    cout << "Total nodes: " << total_nodes << endl;
    
    // Run simulation
    int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;
    
    // Print initial state
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << calculate_fraction_of_ones() << endl;
    
    /// (6)  //////////////////////////////////////////////
    while ( (iteration < max_iterations) && (opinions_changed))
    {
        
        opinions_changed = update_opinions();

        
        iteration = iteration + 1;
        if (iteration % 10 ==0)
            {
                double current_fraction = calculate_fraction_of_ones();
                cout << "Iteration " << iteration << ": fraction of 1's = "  << current_fraction << endl;
            }
    }



    ////////////////////////////////////////////////////////
    // Print final result
    double final_fraction = calculate_fraction_of_ones();
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << final_fraction << endl;
    
    if(final_fraction == 1.0)
        cout << "Consensus reached: all 1's" << endl;
    else if(final_fraction == 0.0)
        cout << "Consensus reached: all 0's" << endl;
    else
        cout << "No consensus reached after " << iteration << " iterations" << endl;
    
    return 0;
}


/*********** Functions to read files **************************/ 

// Read opinion vector from file.
void read_opinions(string filename)
{
    ifstream file(filename);
    int id, opinion;
    while(file >> id >> opinion)
    {
        opinions.push_back(opinion);
        if(id >= total_nodes) total_nodes = id+1;
    }
    file.close();
}

// Read edge list from file and update total nodes as needed.
void read_edges(string filename)
{
    ifstream file(filename);
    int source, target;
    
    while(file >> source >> target)
    {
        edge_list.push_back({source, target});
        if(source >= total_nodes) total_nodes = source+1;
        if(target >= total_nodes) total_nodes = target+1;
    }
    file.close();
}

/********************************************************************** */