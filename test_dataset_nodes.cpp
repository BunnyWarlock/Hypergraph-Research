#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
using namespace std;
// #define endl '\n'

// Using this since relative addressing does not work in my MAC.
string currentDirPath = "/Users/nizamul/Desktop/Codes/Research/";
vector<string> dataset = {"Test Dataset",
                          "Youtube social network and ground-truth communities",
                          "DBLP collaboration network and ground-truth communities",
                          "Amazon product co-purchasing network and ground-truth communities"};
vector<string> graphs = {"Dataset/test.ungraph.txt",
                        "Dataset/com-youtube.ungraph.txt",
                        "Dataset/com-dblp.ungraph.txt",
                        "Dataset/com-amazon.ungraph.txt"};
vector<string> hypergraphs = {"Dataset/test.cmty.txt",
                              "Dataset/com-youtube.all.cmty.txt",
                              "Dataset/com-dblp.all.cmty.txt",
                              "Dataset/com-amazon.all.dedup.cmty.txt"};


int main(){
  ios_base::sync_with_stdio(false);
  // cin.tie(NULL);
  // cin.exceptions(cin.failbit);

  printf("%s\n", string(80, '=').c_str());
  for (int i = 0; i < dataset.size(); ++i){
    printf("Dataset #%d (%s):\n\n", i+1, dataset[i].c_str());
    int nodes, edges;

    // Graph
    ifstream fin(currentDirPath + graphs[i]);

    fin>>nodes>>edges;
    printf("(Given in graph representation) nodes: %d, edges: %d\n", nodes, edges);

    set<int> graphNodes;
    int u, v;
    edges = 0;
    while(fin>>u>>v){
      graphNodes.insert(u);
      graphNodes.insert(v);
      ++edges;
    }

    fin.close();

    printf("The graph representation has %d unique labeled nodes\n", graphNodes.size());





    // Hypergraph
    fin.open(currentDirPath + hypergraphs[i]);

    set<int> hypergraphNodes;
    int hyperedges = 0;
    string line;
    while(getline(fin, line)){
      stringstream buffer(line);
      while(buffer>>u)
        hypergraphNodes.insert(u);
      ++hyperedges;
    }

    fin.close();

    printf("The hypergraph representation has %d unique labeled nodes\n", hypergraphNodes.size());
    printf("(Given in dataset) edges: %d, hyperedges: %d\n", edges, hyperedges);





    int count = 0;
    for (auto itr1 = graphNodes.begin(); itr1 != graphNodes.end(); ){
      auto itr2 = hypergraphNodes.find(*itr1);
      ++itr1;
      if (itr2 == hypergraphNodes.end())
        continue;
      graphNodes.erase(prev(itr1));
      hypergraphNodes.erase(itr2);
      ++count;
    }

    printf("The graph and hypergraph representation shares %d nodes with the same label\n", count);
    printf("The graph representation had %d labeled nodes not in the hypergraph representation\n", graphNodes.size());
    printf("The hypergraph representation had %d labeled nodes not in the graph representation\n", hypergraphNodes.size());

    printf("%s\n", string(80, '=').c_str());
  }



  return 0;
}
