#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>
#include <queue>
#include <cmath>
#include <map>
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

const int MAXN = 2e6;
const int MAXHE = 8e4;
const double epsilon = 1e-9;
map<int, vector<int>> graph;
map<int, int> level;
map<int, double> farness;
map<int, double> closeness;
bitset<MAXN + MAXHE> visited;

void bfs(int source){
  queue<int> q;
  q.push(source);
  visited.reset();
  level[source] = 0;
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

    graph.clear();
    farness.clear();
    closeness.clear();
    level.clear();

    int u, v;
    while(fin>>u>>v){
      graph[u].push_back(v);
      graph[v].push_back(u);
    }

    for (auto& [j, v1]: graph){
      bfs(j);
      for (auto& [k, v2]: graph)
        farness[j] += visited[k]*level[k];
      farness[j] /= max(int(visited.count()) - 1, 1);
      closeness[j] = 1/farness[j];

      // printf("%d\n", j);
    }

    printf("Farness in the graph\n");
    for (auto& j: farness)
      printf("%d: %lf\n", j.first, j.second);
    printf("\n");

    printf("Closeness in the graph\n");
    for (auto& j: closeness)
      printf("%d: %lf\n", j.first, j.second);
    printf("\n");

    double mx = 0;
    for (auto& j: farness)
      mx = max(mx, j.second);

    printf("For graph representation:\n");
    printf("The node(s) with the greatest farness are ");
    for (auto& j: farness)
      if (abs(mx - j.second) < epsilon)
        printf("%d, ", j.first);
    printf("\b\b.\n");

    mx = 0;
    for (auto& j: closeness)
      mx = max(mx, j.second);

      printf("The node(s) with the greatest closeness are ");
      for (auto& j: closeness)
        if (abs(mx - j.second) < epsilon)
          printf("%d, ", j.first);
      printf("\b\b.\n\n");

    fin.close();








    // Hypergraph
    int hyperEdge = 0;
    fin.open(currentDirPath + hypergraphs[i]);

    graph.clear();
    farness.clear();
    closeness.clear();
    level.clear();

    string line;
    while(getline(fin, line)){
      stringstream buffer(line);
      while(buffer>>u){
        graph[u].push_back(MAXN + hyperEdge);
        graph[MAXN + hyperEdge].push_back(u);
      }
      ++hyperEdge;
    }

    for (auto& [j, v1]: graph){
      if (j >= MAXN)
        continue;
      bfs(j);
      for (auto& [k, v2]: graph)
        if (k < MAXN)
          farness[j] += visited[k]*(level[k]/2);
      farness[j] /= max(int((visited<<MAXHE).count()) - 1, 1);
      closeness[j] = 1/farness[j];

      // printf("%d\n", j);
    }

    printf("Farness in the hypergraph\n");
    for (auto& j: farness)
      if (j.first < MAXN)
        printf("%d: %lf\n", j.first, j.second);
    printf("\n");

    printf("Closeness in the hypergraph\n");
    for (auto& j: closeness)
      if (j.first < MAXN)
        printf("%d: %lf\n", j.first, j.second);
    printf("\n");

    mx = 0;
    for (auto& j: farness)
      if (j.first < MAXN)
        mx = max(mx, j.second);

    printf("For graph representation:\n");
    printf("The node(s) with the greatest farness are ");
    for (auto& j: farness)
      if (j.first < MAXN && abs(mx - j.second) < epsilon)
        printf("%d, ", j.first);
    printf("\b\b.\n");

    mx = 0;
    for (auto& j: closeness)
      mx = max(mx, j.second);

      printf("The node(s) with the greatest closeness are ");
      for (auto& j: closeness)
        if (j.first < MAXN && abs(mx - j.second) < epsilon)
          printf("%d, ", j.first);
      printf("\b\b.\n");

    fin.close();


    printf("%s\n", string(80, '=').c_str());
  }



  return 0;
}
