#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/********************DO NOT EDIT**********************/
// Function prototype. Defined later.
void read_opinions(string filename); // reads file into opinions vector and updates total_nodes as needed
void read_edges(string filename); // reads file into edge_list, defined later
void build_adj_matrix(); // convert edge_list to adjacency data structure

int total_nodes = 0; // We keep track of the total number of nodes based on largest node id.
/****************************************************************/

// simple vector to hold each node's opinion (0 or 1)
vector<int> opinions;

// Adjacency List: Each index 'i' holds a list of node IDs that influence 'i'
vector<vector<int>> adj;

// edge list: each row contains {source, target}
vector<vector<int>> edge_list;

void build_adj_matrix()
{
    // (1) allocate adj of appropriate size (total_nodes)
    adj.clear();
    adj.resize(total_nodes);

    // (2) run through edge list and populate adj
    int edge_length = edge_list.size();
    for (int i = 0; i < edge_length; i++)
    {
        int source = edge_list[i][0];
        int target = edge_list[i][1];
        
        // Target is influenced by source. 
        // We only store the incoming connections to speed up the lookup later.
        adj[target].push_back(source);
    }
}

double calculate_fraction_of_ones()
{
    // (3) Calculate the fraction of nodes with opinion 1 and return it.
    int ones_count = 0;
    for (int i = 0; i < total_nodes; i++)
    {
        if (opinions[i] == 1)
        {
            ones_count++;
        }
    }
    return (double)ones_count / total_nodes;
}

// For a given node, count majority opinion among its neighbours. Tie -> 0.
int get_majority_friend_opinions(int node)
{
    // (4) Count the number of neighbours with opinion 0 and opinion 1. 
    int ones = 0;
    int zeros = 0;

    // We only iterate over actual friends, skipping the O(N) matrix search
    int number_of_friends = adj[node].size();
    
    for (int k = 0; k < number_of_friends; k++)
    {
        int friend_id = adj[node][k];
        if (opinions[friend_id] == 1)
        {
            ones++;
        }
        else
        {
            zeros++;
        }
    }

    // Return the majority (0 or 1). If tie, return 0.
    if (ones > zeros)
    {
        return 1;
    }
    
    return 0; 
}

// Calculate new opinions for all voters and return if anyone's opinion changed
bool update_opinions()
{
    // (5) For each node, calculate the majority opinion among its neighbours
    vector<int> next_opinion;
    next_opinion.reserve(total_nodes); // minor optimization to avoid reallocation
    bool changed = false;

    for (int i = 0; i < total_nodes; i++)
    {
        int new_opinion = get_majority_friend_opinions(i);
        next_opinion.push_back(new_opinion);
        
        if (new_opinion != opinions[i])
        {
            changed = true;
        }
    }

    // Update the node's opinion.
    opinions = next_opinion;
    
    // Return true if any node's opinion changed, false otherwise.
    return changed;
}

int main() {
    // Read input files
    read_opinions("opinions.txt"); 
    read_edges("edge_list.txt");

    // convert edge list into adjacency list
    build_adj_matrix();
    
    cout << "Total nodes: " << total_nodes << endl;
    
    // Run simulation
    int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;
    
    // Print initial state
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << calculate_fraction_of_ones() << endl;
    
    // (6) Run until consensus or max iterations
    while ((iteration < max_iterations) && opinions_changed)
    {
        opinions_changed = update_opinions();
        iteration++;
        
        if (iteration % 10 == 0)
        {
            cout << "Iteration " << iteration << ": fraction of 1's = "  
                 << calculate_fraction_of_ones() << endl;
        }
    }

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
