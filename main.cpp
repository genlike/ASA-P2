#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <climits>
#include <set>
#include <map>




#define S -1;
#define T -2;

#define getCoor(m,n) (N*m + n)
namespace PP {
	typedef unsigned int uint;
}
using namespace std;
using namespace PP;

uint N;
uint M;


class Node{
	private:
		uint _id;
		vector<uint>* _values = new vector<uint>(2);
		list<Node*>* _viz = new list<Node*>;		


	public:
		explicit Node (int id) { _id=id; };
		inline uint getId() { return _id; };
		//inline list<Node*>* getList(){ return _l; };
		inline void setId(int id ) { _id= id; };
		inline void setValue(uint index, uint value) {(*_values)[index] = value;};
		inline uint getValue(uint ind = 0) {return (*_values)[ind];};
		inline list<Node*>* getAdj(){ return _viz; };

};


class Graph{
	private:
		vector<Node*>*_nodes;
		//unsigned long _m;
		map<pair<uint,uint>,uint*>* _edges;
		//unsigned long _e=0; 

	public:
		//inline unsigned long getM(){return _m;};
		void printEdges() {
		
		for (map<pair<uint,uint>,uint*>::iterator it=_edges->begin(); it!=_edges->end(); ++it)
		    cout << (it->first.first+1) << '-' << (1+it->first.second) << " => " << *it->second << '\n';
		}
		
		inline uint getEdge(pair<uint,uint> nodes) {return *(*_edges)[nodes];};
		inline Node* getNode(uint id) {
			return (*_nodes)[id];
		}
		inline void setEdge(pair<uint,uint> nodes, uint *value) {(*_edges).insert(pair<pair<uint,uint>,uint*>(nodes,value));};
		//inline unsigned long getE(){return _e;};
		inline vector<Node*>* getAdjList(){return _nodes;};

		Graph(vector<Node*> *nodes, map<pair<uint,uint>,uint*>* edges){
			_nodes=nodes;
			cout << _nodes;
			//_m=m;
			_edges=edges;
			//_e=e;
		};
		~Graph(){
			delete _nodes;
			delete _edges;
		};

};

class Mask{
	private:
		bool ** _matrix;
		uint _h,_w;
		uint _total;
		Graph* _g;
		list<pair<uint,uint>>* _corte;
	public:
		Mask(uint h, uint w, Graph* g) {
			_h=h;_w=w;
			_matrix = new bool*[h];
			for(int i = 0; i<h;i++) _matrix[i] = new bool[w];
			_corte = new list<pair<uint,uint>>();
			_g = g;
		}
		uint getTotalScore() { return _total;};
		void setGraph( Graph* g) { _g = g;}
		Graph * getGraph() { return _g;}
		void printMatrix() {
			char c;
			cout << _total << endl << endl;
			for(uint i=0; i<_h;i++){
				c = (_matrix[i][0])?'C':'P';
				cout << _g->getNode((i*_w))->getValue(_matrix[i][0]) << '|' << i << '-' << 0;
				for(uint j=1; j<_w;j++) {
					c = (_matrix[i][0])?'C':'P';
					/*cout << ' ' << c;*/
					cout << ' ' << _g->getNode((i*_w)+(j))->getValue(_matrix[i][j])<< '|' << i+1 << '-' << j;;
				}
				cout << endl;
			}
			cout << 'C' << endl;
			_g->printEdges();

			for(pair<uint,uint> p : *_corte){
				cout << p.first << '|' << p.second << '|' << _g->getEdge(p);
			}
		}
		void CalculateTotal() {
			Node * u;
			for(uint i=0; i<_h;i++)
				for(uint j=0; j<_w;j++)
					_total += _g->getNode(i*j)->getValue(_matrix[i][j]);

		
		}
};


class Relabel{
	private:
		Graph* _g;

	public:
		void initialize(){
			Node* s = new Node(S);
			Node* t = new Node(T);
			_g->(*nodes)[M*N+0]->setValue(0,0); 
			_g->(*nodes)[M*N+1]->setValue(0,0); 
			for (int i=0; i<M; i++){
				for (int j=0;j<N; j++){
					int coor = getCoor(i,j);
					uint value = _g->(*nodes)[coor]->getValue(0);
					_g->edges->insert(pair<pair<uint,uint>,uint*>(make_pair(S,coor),value));
					_g->edges->insert(pair<pair<uint,uint>,uint*>(make_pair(coor,S),value));
					_g->edges->insert(pair<pair<uint,uint>,uint*>(make_pair(T,coor),value));
					_g->edges->insert(pair<pair<uint,uint>,uint*>(make_pair(coor,T),value));
				}
			}
		}

		qualquer coisa run(){

		}


}


Graph* parse() {
	int m;
	int n;
	
	cin >> m;
	cin >> n;
	M=m;
	N=n;

	int total = m*n;
	
	vector<Node*>* nodes = new vector<Node*>(total+2); //MAIS 2 PARA S E T
	map<pair<uint,uint>,uint*>* edges = new map<pair<uint,uint>,uint*>; //ESTÁ PARA PONTEIRO NAO PARA INT
	for (int i = 0 ; i<total; i++){
		Node* nd = new Node(i);
		(*nodes)[i] = nd;
	}

	uint v_branco;
	for (int i = 0; i<total; i++) {
		cin >> v_branco;
		(*nodes)[i]->setValue(0,v_branco); 
	}

	uint v_preto;
	for (int i = 0; i<total; i++) {
		cin >> v_preto;
		(*nodes)[i]->setValue(1,v_preto);
	}

	uint *v_hor;
	for (int i=0; i<m; i++){
		for (int j=0;j<n-1; j++){
			v_hor = new uint();			
			cin >> *v_hor;			
			edges->insert(pair<pair<uint,uint>,uint*>(make_pair(getCoor(i,j),getCoor(i,j)+1),v_hor));
			edges->insert(pair<pair<uint,uint>,uint*>(make_pair(getCoor(i,j)+1, getCoor(i,j)),v_hor));
		}
	}

	uint *v_ver ;
	for (int i=0; i<m-1; i++){
		for (int j=0;j<n; j++){
			v_ver = new uint();
			cin >> *v_ver;
			edges->insert(pair<pair<uint,uint>,uint*>(make_pair(getCoor(i,j),getCoor(i,j)+N),v_ver));
			edges->insert(pair<pair<uint,uint>,uint*>(make_pair(getCoor(i,j)+N,getCoor(i,j)),v_ver));
		}
	}

	return new Graph(nodes,edges);
}

int main() {
	Graph *g = parse();
	Mask *m = new Mask(N,M,g);
/*	cout << 'M' << endl;*/
	m->printMatrix();
	return 0;
}
