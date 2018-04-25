#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <climits>
#include <set>
#include <map>




#define UP 0;
#define RIGHT 1;
#define DOWN 2;
#define LEFT 3;

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
		inline uint getEdge(pair<uint,uint> nodes) {return *(*_edges)[nodes];};
		inline Node* getNode(uint id) {return (*_nodes)[id];};
		inline void setEdge(pair<uint,uint> nodes, uint *value) {(*_edges).insert(pair<pair<uint,uint>,uint*>(nodes,value));};
		//inline unsigned long getE(){return _e;};
		inline vector<Node*>* getAdjList(){return _nodes;};

		Graph(vector<Node*> *nodes, map<pair<uint,uint>,uint*>* edges){
			_nodes=nodes;
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
		Mask(uint h, uint w) {
			_h=h;_w=w;
			_matrix = new bool*[h];
			for(int i = 0; i<h;i++) _matrix[i] = new bool[w];
			_corte = new list<pair<uint,uint>>();
		}
		uint getTotalScore() { return _total;};
		void setGraph( Graph* g) { _g = g;}
		Graph * getGraph() { return _g;}
		void printMatrix() {
			char c;
			cout << _total << endl << endl;
			for(uint i=0; i<_h;i++){
				c = (_matrix[i][0])?'C':'P';
				cout << c;
				for(uint j=1; j<_w;j++) {
					c = (_matrix[i][0])?'C':'P';
					cout << ' ' << c;
				}
				cout << endl;
			}
			
		}
		void CalculateTotal() {
			Node * u;
			for(uint i=0; i<_h;i++)
				for(uint j=0; j<_w;j++)
					_total += _g->getNode(i*j)->getValue(_matrix[i][j]);

			for(pair<uint,uint> p : *_corte){
				_total += _g->getEdge(p);
			}
		}
};


Graph* parse() {
	int m;
	int n;
	
	cin >> m;
	cin >> n;
	M=m;
	N=n;

	int total = m*n;
	
	vector<Node*>* nodes = new vector<Node*>(total);
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

	uint *v_hor = new uint();
	for (int i=0; i<m; i++){
		for (int j=0;j<n-1; j++){
			cin >> *v_hor;
			edges->insert(pair<pair<uint,uint>,uint*>(make_pair(getCoor(i,j),getCoor(i,j)+1),v_hor));
		}
	}

	uint *v_ver = new uint();
	for (int i=0; i<m-1; i++){
		for (int j=0;j<n; j++){
			cin >> *v_ver;
			edges->insert(pair<pair<uint,uint>,uint*>(make_pair(getCoor(i,j),getCoor(i,j)+N),v_ver));
		}
	}

	return new Graph(nodes,edges);
}

int main() {
	Graph *g = parse();
	Mask *m = new Mask(N,M);
	cout << 'M' << endl;
	m->printMatrix();
	return 0;
}