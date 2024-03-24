#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
// #define endl '\n'

struct DSU{
    vector<int> p;
    int comp; // No. of components
    DSU(int n): p(n, -1), comp(n){}
    void make() { p.push_back(-1); ++comp; }
    int size(int v) { return -p[find(v)]; }
    int find(int v) { return (p[v]<0)? v: p[v]=find(p[v]); }
    bool join(int a, int b){
      a = find(a), b = find(b);
      if (a == b) return false;
      if (p[a] > p[b]) swap(a, b);
      p[a] += p[b], p[b] = a, --comp;
      return true;
    }
};

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

    DSU graphConnectivity(0);
    map<int, int> hash1;
    int u, v;
    while(fin>>u>>v){
      if (!hash1.count(u)){
        hash1[u] = graphConnectivity.p.size();
        graphConnectivity.make();
      }
      if (!hash1.count(v)){
        hash1[v] = graphConnectivity.p.size();
        graphConnectivity.make();
      }
      graphConnectivity.join(hash1[u], hash1[v]);
    }

    fin.close();






    // Hypergraph
    fin.open(currentDirPath + hypergraphs[i]);

    map<int, int> hash2;
    DSU hypergraphConnectivity(0);
    string line;
    while(getline(fin, line)){
      stringstream buffer(line);
      buffer>>v;
      if (!hash2.count(v)){
        hash2[v] = hypergraphConnectivity.p.size();
        hypergraphConnectivity.make();
      }
      while(buffer>>u){
        if (!hash2.count(u)){
          hash2[u] = hypergraphConnectivity.p.size();
          hypergraphConnectivity.make();
        }
        hypergraphConnectivity.join(hash2[u], hash2[v]);
      }
    }

    fin.close();




    int cnt1, cnt2, cnt3;
    cnt1 = cnt2 = cnt3 = 0;
    for (auto itr1 = hash1.begin(); itr1 != hash1.end(); ){
      auto itr2 = hash2.find(itr1->first);
      ++itr1;
      if (itr2 == hash2.end())
        continue;

      int s1 = graphConnectivity.size(prev(itr1)->second);
      int s2 = hypergraphConnectivity.size(itr2->second);
      if (s1 == s2)
        ++cnt1;
      else if (s1 > s2)
        ++cnt2;
      else
        ++cnt3;

      hash1.erase(prev(itr1));
      hash2.erase(itr2);
    }
    cnt2 += hash1.size();
    cnt3 += hash2.size();

    printf("The graph representation has %d connected components\n", graphConnectivity.comp);
    printf("The hypergraph representation has %d connected components\n", hypergraphConnectivity.comp);
    printf("There are %d nodes connected to the same no. of nodes in graph and hypergraph representation\n", cnt1);
    printf("There are %d nodes in graph connected to more nodes than that in the hypergraph representation\n", cnt2);
    printf("There are %d nodes in graph connected to less nodes than that in the hypergraph representation\n", cnt3);

    printf("%s\n", string(80, '=').c_str());
  }



  return 0;
}
