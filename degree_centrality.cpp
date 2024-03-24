#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
// #define endl '\n'

// Using this since relative addressing does not work in my MAC.
string currentDirPath = "/Users/nizamul/Desktop/Codes/Research/";
vector<string> dataset = {"Youtube social network and ground-truth communities",
                          "DBLP collaboration network and ground-truth communities",
                          "Amazon product co-purchasing network and ground-truth communities"};
vector<string> graphs = {"Dataset/com-youtube.ungraph.txt",
                        "Dataset/com-dblp.ungraph.txt",
                        "Dataset/com-amazon.ungraph.txt"};
vector<string> hypergraphs = {"Dataset/com-youtube.all.cmty.txt",
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

    int u, v;
    map<int, int> degree;
    while(fin>>u>>v){
      ++degree[u];
      ++degree[v];
    }

    int mx = 0;
    for (auto& d: degree)
      mx = max(mx, d.second);

    printf("For graph representation:\n");
    printf("The node(s) with the highest degree are ");
    for (auto& d: degree)
      if (d.second == mx)
        printf("%d, ", d.first);
    printf("\b\b.\n\n");

    fin.close();

    // Hypergraph
    fin.open(currentDirPath + hypergraphs[i]);

    degree.clear();

    string line;
    while(getline(fin, line)){
      stringstream buffer(line);
      while(buffer>>u)
        ++degree[u];
    }

    mx = 0;
    for (auto& d: degree)
      mx = max(mx, d.second);

    printf("For hypergraph representation:\n");
    printf("The node(s) inside the most hyperedges are ");
    for (auto& d: degree)
      if (d.second == mx)
        printf("%d, ", d.first);
    printf("\b\b.\n\n");

    fin.close();

    printf("%s\n", string(80, '=').c_str());
  }



  return 0;
}
