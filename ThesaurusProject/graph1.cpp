// store all words of data into a weighted graph, so that edges can be added if synonym and weight can be changed based on user response
// might end up with seperate graphs, in this case can use another datastructure to point to the correct graph

class graph
{
public:
  void addNode(string word); // dynamically allocate new memeory for string, all have edge to root
  int findNode(string word); // returns location of word from access array
  void findSyn(int wordloc); // takes in array of 3 nodes, clears it and returns top 3 synonyms for this node
  void addEdge_changeWeight(int weight, string a, string b); // should add connection between nodes a and b
private:
  string top3[3];
  // nodes will be dynamically allocated connected to a nullptr at head
  vector<vector<int>> edges;// adjacency int list, stores weights. if weight == 0 then no connection
  vector<*string> acc; // TODO make into a hash table, stores pointers to dynamically allocated nodes from root
  graph();
  ~graph();
};
graph::graph()
{
  for (int i = 0; i < 3; i++)
  {
    top3[i] = "";
  }
}
graph::~graph()
{
  for (int i = 0; i < 3; i++)
  {
    top3[i] = NULL;
  }
  for (int i = 0; i < edges.size(); i++)
  {
    edges[i].clear();
  }
  edges.clear();
  for (int i = 0; i < acc.size(); i++)
  {
    delete acc[i];
  }
  acc.clear();
}
void graph::addNode(string word)
{
  string *temp = new string;
  *temp = word; // dereference temp to assign data
  edges.push_back(0); // add edge between root and this
  int size = edges.size();
  for(int i = 0; i < size - 1; i++) // adds a new row to all but last element
  {
    edges[i].push_back(0);
  }
  for(int i = 0; i < size; i++)
  {
    edges[size].push_back(0); // this should make the array n by n size
  }
  acc.push_back(temp); // this should make this a vector of addresses to new strings, add new node to access vector
  return;
}
void graph::addEdge_changeWeight(int weight, string a, string b)
{
  int aloc = findNode(a);
  int bloc = findNode(b);
  if (aloc == -1 || bloc == -1)
  {
    break; // maybe add an error statement
  }
  edges[a][b] = weight;
  edges[b][a] = weight;
  // therefore there is a link between these two nodes, undirected or the link has been updated
  return;
}
int graph::findNode(string word)
{
  for (int i = 0; i < acc.size(); i++) // go through ptr array to find node
  {
    if (*acc[i] == word)
    {
      return i; // return location
    }
  }
  return -1; // node not found
}
void graph::findSyn(int wordloc)
{
  for (int i = 0; i < 3; i++)
  {
    top3[i] = NULL; // or empty string
  }
  // TODO pass HEIGHEST weights to top3
}
