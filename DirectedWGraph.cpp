#include <bits/stdc++.h>
using std::istream;
using std::ostream;
using std::vector;
using std::unordered_map;
using std::cout;
using std::cin;
using std::endl;
using std::ios_base;
using std::string;
using std::cerr;
using std::pair;
using std::unordered_map;
using std::map;
using std::queue;
using std::stack;
using std::priority_queue;
#define IOS ios_base::sync_with_stdio(false),cin.tie(NULL)
#define READ freopen("input.txt", "r", stdin); 
#define ERROR freopen("error.txt", "w", stderr); 
#define OUTPUT freopen("output.txt", "w", stdout); 
#define endl '\n'
	

	template<typename T> class directed_w_graph
	{
		template<typename U>friend ostream& operator<<(ostream &, directed_w_graph<U> &);

	protected:
		vector< vector< pair<int,int> > > G;
		vector<T> mapping_table;
		map<T,int> reverse_mapping_table;
	private:
		void bfs_utility(int,vector<bool> &, vector<T> &);
		void dfs_utility(int,vector<bool> &, vector<T> &);
		void kosarajus_algorithm(int,vector<bool> &,vector<int> &);
		void kosarajus_utility(int,vector<bool> &);
	public:

		// ctors and dtors 
		directed_w_graph(const vector<T> &init);
		~directed_w_graph(){}

		//input/output
		void cin_graph(int);// to input graph via istream
		void cin_graph(const vector<pair< pair<T,T> ,int> > &);//input graph via vector
		void add_edge(const T &,const T &, const int &); //to add an edge 
		void add_vertex(const T&); //to add an vertex
		void cout_graph();

		//traversal functions
		vector<T> bfs(const T &s);
		vector<T> bfs();
		vector<T> dfs(const T &s);
		vector<T> dfs();

		//misc functions
		bool is_cycle();
		int connected_compoenents();

		//shortest distance functions
		vector<int> shortest_distance(const T &);
		vector<int> shortest_distance_bf(const T &);

	
   };
template<typename T>void directed_w_graph<T>::kosarajus_utility(int s,vector<bool> &visited)
	{
		visited.at(s)=true;
		for(auto &i: G[s])
		{
			if(!visited.at(i.first)) kosarajus_utility(i.first,visited);
		}
	}
template<typename T>void directed_w_graph<T>::kosarajus_algorithm(int s,vector<bool> &visited,vector<int> &sequence)
	{
		visited.at(s)= true;
		for(auto &i: G[s])
		{
			if(!visited.at(i.first)) kosarajus_algorithm(i.first,visited,sequence);
		}
		sequence.emplace_back(s);
	}
template<typename T>int directed_w_graph<T>::connected_compoenents()
	{  int c{0};
		vector<bool> visited(G.size(),false);
		vector<int> sequence;
		for(int i=0;i<G.size();i++)
		{
			if(!visited.at(i))
			{
				kosarajus_algorithm(i,visited,sequence);
			}
		}
		std::fill(visited.begin(),visited.end(),false);
		for(int i=sequence.size()-1 ; i>=0 ; i--)
		{
			if(!visited.at(sequence[i]))
			{
				kosarajus_utility(sequence[i],visited);
				c++;
			}
		}
		return c;
	}
template<typename T> bool directed_w_graph<T>::is_cycle()
	{
		int c{0};
		vector<int> indegree(G.size(),0);
		for(auto &i: G)
		{
			for(auto &j: i)
			{
				indegree.at(j.first)++;
			}
		}
		queue<int> q;
		for(int i=0;i<indegree.size();i++)
		{
			if(indegree.at(i)==0)
			{
				q.push(i);
			}
		}
		while(!q.empty())
		{
			for(auto &i: G[q.front()])
			{
				indegree.at(i.first)--;
				if(indegree.at(i.first)==0) q.push(i.first);
			}
			q.pop();
			c++;
		}
		return !(c==G.size());
	}

template<typename T> vector<int> directed_w_graph<T>::shortest_distance_bf(const T &s_)
	{
		auto it= reverse_mapping_table.find(s_);
		//RAISE AN EXCEPTION HERE
		if(it==reverse_mapping_table.end())
		{
			cerr<<"fuck bellman ford";
			exit(0);
		}
		int s=it->second;
		vector<int> distance(G.size(),INT_MAX);
		distance[s]=0;

		for(int i=0;i<(G.size()-1);i++)
		{

			for(int j=0;j<G.size();j++)
			{	
				if(distance[j]!=INT_MAX)
				{
					for(auto &k: G.at(j))
					{
						if(distance[j]!=INT_MAX and distance[k.first]>distance[j]+k.second)
						{
							distance[k.first]=distance[j]+k.second;
						}
					}
			    }
			}
		}      
		for(int i=0;i<G.size();i++)
		{
			if(distance[i]!=INT_MAX)
			{
			for(auto &k: G.at(i))
			{
				if(distance[k.first]> distance[i]+k.second)
				{
					throw "Negative cycle present nigga\n";
				}
			}
		    }
		}
		return distance;
	}
template<typename T> vector<int> directed_w_graph<T>::shortest_distance(const T &s)
	{
		//RAISE AN EXCEPTION IF SOURCE VERTEX DOESNT EXIST IN THE GRAPH
		auto it= reverse_mapping_table.find(s);
		if(it==reverse_mapping_table.end())
		{
			cerr<<"fuck dijsktras ";
			exit(1);
		}
		int _s=it->second;
		int sz{0};
		vector<bool> visited(G.size(),false);
		vector<int > distance(G.size(),INT_MAX);
		distance[_s]=0;
		priority_queue< pair<int,int> ,vector< pair<int,int> > ,std::greater<> > pq;
		for(long long  i=0;i<G.size();i++)
		{
			pq.push({distance[i],i});
		}
		while(!pq.empty() and sz!=G.size())
		{
			if(visited.at(pq.top().second)==false)
			{	sz++;
				pair<int,int> p=pq.top();
				pq.pop();
				visited.at(p.second)=true;
				distance.at(p.second)= p.first;
				if(distance.at(p.second)!=INT_MAX)
				{
					for(auto &i: G[p.second])
					{
						if( !visited.at(i.first) and distance[i.first]> distance[p.second]+i.second)
						{
							distance[i.first]=distance[p.second]+i.second;
							pq.push({distance[i.first],i.first});
						}
					}
				}
			}
			else pq.pop();
		}
		return distance;
	}

 template<typename T> vector<T> directed_w_graph<T>::dfs()
   {
   		vector<T> res;
   		if(mapping_table.empty())
   		{
   			return res;
   		}
   		vector<bool> visited(G.size(),false);
   		for(int i=0;i<G.size();i++)
   		{
   			if(!visited.at(i)) dfs_utility(i,visited,res);
   		}
   		return res;
   }
template<typename T> void directed_w_graph<T>::dfs_utility(int s,vector<bool> &visited,vector<T> &res)
	{
		visited.at(s)=true;
		res.emplace_back(mapping_table[s]);
		for(auto &i: G.at(s))
		{
			if(!visited.at(i.first)) dfs_utility(i.first,visited,res);
		}
	}
template<typename T> vector<T> directed_w_graph<T>::dfs(const T &s)
	{
		auto it= reverse_mapping_table.find(s);
		if(it==reverse_mapping_table.end())
		{
			cerr<<"fuck dfs";
			exit(1);
		}
		int _s=it->second;
		vector<bool> visited(G.size(),false);
		vector<T> res;
		for(int i=_s ; i<G.size();i++)
		{
			if(!visited.at(i)) dfs_utility(i,visited,res);
		}
		return res;
	}
template<typename T> vector<T> directed_w_graph<T>::bfs()
	{
		vector<T> res;
		if(mapping_table.empty())
		{
			return res;
		}
		vector<bool> visited(G.size(),false);
		for(int i=0;i<G.size();i++)
		{
			if(!visited.at(i)) bfs_utility(i,visited,res);
		}
		return res;
	}
template<typename T> void directed_w_graph<T>::bfs_utility(int s,vector<bool> &visited,vector<T> &res)
	{
		queue<int> q;
		q.push(s);
		visited.at(s)=true;
		while(!q.empty())
		{
			for(auto &i: G[q.front()])
			{
				if(visited.at(i.first)==false)
				{
					q.push(i.first);
					visited.at(i.first)=true;
				}
			}
			res.emplace_back(mapping_table[q.front()]);
			q.pop();
		}
	}
template<typename T> vector<T> directed_w_graph<T>::bfs(const T &s)
	{
		int _s;
		auto it= reverse_mapping_table.find(s);
		if(it==reverse_mapping_table.end())
		{
			cerr<<"fuckbfs";
			exit(0);
		}
		_s=it->second;
		vector<bool> visited(G.size(),false);
		vector<T> res;
		for(int i=_s;i<G.size();i++)
		{
			if(visited.at(i)==false) bfs_utility(i,visited,res);
		}
		return res;
	}
template<typename T> void directed_w_graph<T>::cout_graph()
	{
		for(int i=0;i<G.size();i++)
		{
			cout<<mapping_table[i]<<"|->";
			for(int j=0;j<G.at(i).size();j++)
			{
				cout<<mapping_table[G[i][j].first]<<"("<<G[i][j].second<<")";
				if(j!=G.at(i).size()-1) cout<<"->";
			}
			cout<<"\n";
		}
	}
template<typename T> void directed_w_graph<T>::cin_graph(const vector< pair<pair<T,T>,int> > &temp)
	{
		for(auto &i: temp)
		{
			add_edge(i.first.first,i.first.second,i.second);
		}
	}
template<typename T> void directed_w_graph<T>::cin_graph(int E)
	{
		while(E--)
		{ 
			T from,to;
			int wt;
			cin>>from>>to;
			cin>>wt;
			add_edge(from,to,wt);
		}
	}
template<typename T> void directed_w_graph<T>::add_vertex(const T &temp)
	{
		mapping_table.emplace_back(temp);
		reverse_mapping_table[temp]=G.size();
		G.resize(G.size()+1);
	}
template<typename T> void directed_w_graph<T>::add_edge(const T &from,const T &to,const int &wt)
	{
		//RAISE AN XCEPTION IF TO OR FROM ARENT IN THE GRAPH
		auto it1= reverse_mapping_table.find(from);
		auto it2=reverse_mapping_table.find(to);
		if(it1==reverse_mapping_table.end() or it2==reverse_mapping_table.end())
		{
			cerr<<"fuck! add edge";
			exit(0);
		}
		int _from= it1->second;
		int _to= it2->second;
		G[_from].push_back({_to,wt});
	}
template<typename T> directed_w_graph<T>::directed_w_graph(const vector<T> &init)
	{
		for(int i=0;i<init.size();i++)
		{
			mapping_table.emplace_back(init[i]);
			reverse_mapping_table[init[i]]=i;
		}
		G.resize(init.size());
	}














//______________________________________________________________________________________
	template<> class directed_w_graph<int>
	{
		friend ostream& operator<<(ostream &, directed_w_graph<int> &);
	protected:
		vector< vector< pair<int,int> > > G;

	private:
		void bfs_utility(int,vector<bool> &,vector<int> &); //bfs utility function
		void dfs_utility(int,vector<bool> &, vector<int> &); //dfs utility function
		void kosarajus_algorithm(int,vector<bool> &,vector<int> &);
		void kosarajus_utility(int, vector<bool> &);
	public:
		//ctors and dtors
		directed_w_graph(int s=0);// ctor 
		~directed_w_graph(){}; //dtor

		//input/output functions 
		void cin_graph(int);  // to input graph from istream
		void cin_graph(const vector< pair<pair<int,int>,int> >&); // to take input from vector
		void add_edge(const int &,const int &,const int &); //add and edge 
		void add_vertex(); //add a vertex
		void cout_graph(); //to print the graph via ostream 

		//traversal functions 
		vector<int> bfs(int s=0);  //bfs driver function
		vector<int> dfs(int s=0);  //dfs driver function

		//misc functions
		bool is_cycle();
		int connected_compoenents();

		//shortest distance 
		vector<int> shortest_distance(int s=0);
		vector<int> shortest_distance_bf(int s=0);

	};
	void directed_w_graph<int>::kosarajus_utility(int s,vector<bool> &visited)
	{
		visited.at(s)=true;
		for(auto &i: G[s])
		{
			if(!visited.at(i.first)) kosarajus_utility(i.first,visited);
		}
	}
	void directed_w_graph<int>::kosarajus_algorithm(int s,vector<bool> &visited,vector<int> &sequence)
	{
		visited.at(s)= true;
		for(auto &i: G[s])
		{
			if(!visited.at(i.first)) kosarajus_algorithm(i.first,visited,sequence);
		}
		sequence.emplace_back(s);
	}
	int directed_w_graph<int>::connected_compoenents()
	{  int c{0};
		vector<bool> visited(G.size(),false);
		vector<int> sequence;
		for(int i=0;i<G.size();i++)
		{
			if(!visited.at(i))
			{
				kosarajus_algorithm(i,visited,sequence);
			}
		}
		std::fill(visited.begin(),visited.end(),false);
		for(int i=sequence.size()-1 ; i>=0 ; i--)
		{
			if(!visited.at(sequence[i]))
			{
				kosarajus_utility(sequence[i],visited);
				c++;
			}
		}
		return c;
	}
	bool directed_w_graph<int>::is_cycle()
	{
		int c{0};
		vector<int> indegree(G.size(),0);
		for(auto &i: G)
		{
			for(auto &j: i)
			{
				indegree.at(j.first)++;
			}
		}
		queue<int> q;
		for(int i=0;i<indegree.size();i++)
		{
			if(indegree.at(i)==0)
			{
				q.push(i);
			}
		}
		while(!q.empty())
		{
			for(auto &i: G[q.front()])
			{
				indegree.at(i.first)--;
				if(indegree.at(i.first)==0) q.push(i.first);
			}
			q.pop();
			c++;
		}
		return !(c==G.size());
	}
	vector<int> directed_w_graph<int>::shortest_distance_bf(int s)
	{
		vector<int> distance(G.size(),INT_MAX);
		distance[s]=0;

		for(int i=0;i<(G.size()-1);i++)
		{

			for(int j=0;j<G.size();j++)
			{	
				if(distance[j]!=INT_MAX)
				{
					for(auto &k: G.at(j))
					{
						if(distance[j]!=INT_MAX and distance[k.first]>distance[j]+k.second)
						{
							distance[k.first]=distance[j]+k.second;
						}
					}
			    }
			}
		}
		for(int i=0;i<G.size();i++)
		{
			if(distance[i]!=INT_MAX)
			{
			for(auto &k: G.at(i))
			{
				if(distance[k.first]> distance[i]+k.second)
				{
					throw "Negative cycle present nigga\n";
				}
			}
		    }
		}
		return distance;
	}
	vector<int> directed_w_graph<int>::shortest_distance(int s)
	{
		//RAISE AN EXCEPTION IF SOURCE VERTEX DOESNT EXIST IN THE GRAPH
		int sz{0};
		vector<bool> visited(G.size(),false);
		vector<int > distance(G.size(),INT_MAX);
		distance[s]=0;
		priority_queue< pair<int,int> ,vector< pair<int,int> > ,std::greater<> > pq;
		for(long long  i=0;i<G.size();i++)
		{
			pq.push({distance[i],i});
		}
		while(!pq.empty() and sz!=G.size())
		{
			if(visited.at(pq.top().second)==false)
			{	sz++;
				pair<int,int> p=pq.top();
				pq.pop();
				visited.at(p.second)=true;
				distance.at(p.second)= p.first;
				if(distance.at(p.second)!=INT_MAX)
				{
					for(auto &i: G[p.second])
					{
						if( !visited.at(i.first) and distance[i.first]> distance[p.second]+i.second)
						{
							distance[i.first]=distance[p.second]+i.second;
							pq.push({distance[i.first],i.first});
						}
					}
				}
			}
			else pq.pop();
		}
		return distance;
	}
	

	void directed_w_graph<int>::dfs_utility(int s,vector<bool> &visited,vector<int> &res)
	{
		visited.at(s)=true;
		res.emplace_back(s);
		for(auto &i: G[s])
		{
			if(visited.at(i.first)==false) dfs_utility(i.first,visited,res);
		}
	}
	vector<int> directed_w_graph<int>::dfs(int s)
	{	//RAISE AN EXCEPTION IF THE SOURCE VERTEX DOENT EXIST IN THE GRAPH
		vector<bool> visited(G.size(),false);
		vector<int> res;
		for(int i=s;i<G.size();i++)
		{
			if(!visited.at(i)) dfs_utility(i,visited,res);
		}
		return res;
	}
	void directed_w_graph<int>::bfs_utility(int s,vector<bool> &visited,vector<int> &res)
	{
		queue<int> q;
		q.push(s);
		visited.at(s)=true;
		while(!q.empty())
		{
			for(auto &i: G[q.front()])
			{
				if(visited.at(i.first)==false)
				{
					q.push(i.first);
					visited.at(i.first)=true;
				}
			}
			res.emplace_back(q.front());
			q.pop();
		}
	}
	vector<int> directed_w_graph<int>::bfs(int s)
	{
		//RAISE AN EXCEPTION IF SOURCE DOEESNT EXIST IN THE GRAPH CONTAINER
		vector<bool> visited(G.size(),false);
		vector<int> res;
		for(int i=s;i<G.size();i++)
		{
			if(!visited.at(i)) bfs_utility(i,visited,res);
		}
		return res;
	}
	void directed_w_graph<int>::cout_graph()
	{
		for(int i=0;i<G.size();i++)
		{	cout<<i<<"|->";
			for(int j=0;j<G.at(i).size();j++)
			{
				cout<<G[i][j].first<<"("<<G[i][j].second<<")";
				if(j!=G[i].size()-1)
				{
					cout<<"->";
				}
			}
			cout<<"\n";
		}
	}
	void directed_w_graph<int>::cin_graph(const vector<pair<pair<int,int>,int> > &temp)
	{
		for(auto &i: temp)
		{
			add_edge(i.first.first,i.first.second,i.second);
		}
	}
	void directed_w_graph<int>::cin_graph(int E)
	{
		while(E--)
		{
			int from,to,wt;
			cin>>from>>to>>wt;
			add_edge(from,to,wt);
		}
	}
	void directed_w_graph<int>::add_edge(const int &from,const int &to,const int &wt)
	{
		//RAISE AN EXCEPTION IF to or from does'nt exist in the graph
		G[from].push_back({to,wt});
	}
	void directed_w_graph<int>::add_vertex()
	{
		G.resize(G.size()+1);
	}
	directed_w_graph<int>::directed_w_graph(int s) //ctor
	{
		G.resize(s);
	}

template<typename T>ostream& operator<<(ostream &os,directed_w_graph<T> &obj)
	{ 
		for(int i=0;i<obj.G.size();i++)
		{	os<<obj.mapping_table[i]<<"|->";
			for(int j=0;j<obj.G.at(i).size();j++)
			{
				os<<obj.mapping_table[obj.G[i][j].first]<<"("<<obj.G[i][j].second<<")";
				if(j!=obj.G[i].size()-1)
				{
					os<<"->";
				}
			}
			os<<"\n";
		}
		return os;
	}
	ostream& operator<<(ostream &os,directed_w_graph<int> &obj)
	{ 
		for(int i=0;i<obj.G.size();i++)
		{	os<<i<<"|->";
			for(int j=0;j<obj.G.at(i).size();j++)
			{
				os<<obj.G[i][j].first<<"("<<obj.G[i][j].second<<")";
				if(j!=obj.G[i].size()-1)
				{
					os<<"->";
				}
			}
			os<<"\n";
		}
		return os;
	}
int main() 
{ 
	IOS;
#ifndef ONLINE_JUDGE 
  	READ;
  	ERROR;
  	OUTPUT;
#endif 

	int t;
	// cin>>t;  
	t=1;
	while(t--) 
	{ 
		directed_w_graph<int> G(4);
		G.cin_graph(5);
		cout<<G;
	} 
	cerr<<"time taken : "<<(float)clock()/CLOCKS_PER_SEC<<" secs"<<"\n"; 
return 0; 
} 
  

  						//  FINISHED //