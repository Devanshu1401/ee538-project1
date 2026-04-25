#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int total_nodes = 0;

vector<int> opinions;
vector<vector<int>> adj;
vector<vector<int>> edge_list;

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

void build_adj_matrix()
{
    adj.clear();
    adj.resize(total_nodes);

    int edge_length = edge_list.size();
    for (int i = 0; i < edge_length; i++)
    {
        int source = edge_list[i][0];
        int target = edge_list[i][1];
        adj[target].push_back(source);
    }
}

double calculate_fraction_of_ones()
{
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

int get_majority_friend_opinions(int node)
{
    int ones = 0;
    int zeros = 0;

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

    if (ones > zeros)
    {
        return 1;
    }
    
    return 0; 
}

bool update_opinions()
{
    vector<int> next_opinion;
    next_opinion.reserve(total_nodes); 
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

    opinions = next_opinion;
    
    return changed;
}

int main() {
    read_opinions("opinions.txt"); 
    read_edges("edge_list.txt");

    build_adj_matrix();
    
    cout << "Total nodes: " << total_nodes << endl;
    
    int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;
    
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << calculate_fraction_of_ones() << endl;
    
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
