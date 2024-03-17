#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>
#include <cstring>
#include <queue>
#include <cmath>
using namespace std;
// #define endl '\n'

// Using this since relative addressing does not work in my MAC.
string currentDirPath = "/Users/nizamul/Desktop/Codes/Research/";
vector<string> dataset = {"DBLP collaboration network and ground-truth communities",
                          "Amazon product co-purchasing network and ground-truth communities"};
vector<string> graphs = {"Dataset/com-dblp.ungraph.txt",
                        "Dataset/com-amazon.ungraph.txt"};
vector<string> hypergraphs = {"Dataset/com-dblp.all.cmty.txt",
                              "Dataset/com-amazon.all.dedup.cmty.txt"};

const int MAXN = 6e5;
const int MAXHE = 8e4;
const double epsilon = 1e-9;
vector<int> graph[MAXN + MAXHE];
int level[MAXN];
bitset<MAXN> visited;
double farness[MAXN], closeness[MAXN];

void bfs(int source){
  queue<int> q;
  q.push(source);
  visited[source] = true;
  while(!q.empty()){
    int current_vertex = q.front();
    q.pop();
    for (auto node: graph[current_vertex])
      if (!visited[node]){
        q.push(node);
        visited[node] = true;
        level[node] = level[current_vertex] + 1;
      }
  }
}

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

    for (int j = 0; j < MAXN; ++j)
      graph[j].clear();

    int mxn = 0, u, v;
    while(fin>>u>>v){
      mxn = max(mxn, max(u, v));
      graph[u].push_back(v);
      graph[v].push_back(u);
    }

    memset(farness, 0, sizeof(farness));

    for (int j = 0; j <= mxn; ++j){
      visited.reset();
      level[j] = 0;
      bfs(j);
      for (int k = 0; k <= mxn; ++k)
        farness[j] += visited[k]*level[k];
      farness[j] /= max(int(visited.count()) - 1, 1);
      closeness[j] = 1/farness[j];

      // if (j%1000 == 0) printf("%d\n", j);
    }

    double mx = 0;
    for (int j = 0; j <= mxn; ++j)
      mx = max(mx, farness[j]);

    printf("For graph representation:\n");
    printf("The node(s) with the greatest farness are ");
    for (int j = 0; j <= mxn; ++j)
      if (abs(mx - farness[j]) < epsilon)
        printf("%d, ", j);
    printf("\b\b.\n");

    mx = 0;
    for (int j = 0; j <= mxn; ++j)
      mx = max(mx, closeness[j]);

      printf("The node(s) with the greatest closeness are ");
      for (int j = 0; j <= mxn; ++j)
        if (abs(mx - closeness[j]) < epsilon)
          printf("%d, ", j);
      printf("\b\b.\n\n");

    // printf("%d\n", mxn);

    fin.close();

    // Hypergraph
    int hyperEdge = 0;
    fin.open(currentDirPath + hypergraphs[i]);

    for (int j = 0; j < MAXN + MAXHE; ++j)
      graph[j].clear();

    string line;
    mxn = 0;
    while(getline(fin, line)){
      stringstream buffer(line);
      while(buffer>>u){
        graph[u].push_back(MAXN + hyperEdge);
        graph[MAXN + hyperEdge].push_back(u);
        mxn = max(mxn, u);
      }
      ++hyperEdge;
    }

    // Work in Progress!!!

    // printf("%d\n", mxn);

    fin.close();


    printf("%s\n", string(80, '=').c_str());
  }



  return 0;
}
