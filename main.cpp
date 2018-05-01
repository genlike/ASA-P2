#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <climits>
#include <set>
#include <map>
#include <queue>



#define S -1
#define T -2
#define WHITE 0
#define GRAY 1
#define BLACK 2

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
		uint _color = WHITE;
		pair<uint,uint>* _papi;
		bool _white=1;


	public:
		explicit Node (int id) { _id=id; }
		inline uint getId() { return _id; }
		//inline list<Node*>* getList(){ return _l; };
		inline void setId(int id ) { _id= id; }
		inline void setValue(uint index, uint value) {(*_values)[index] = value;}
		inline uint getValue(uint ind = 0) {return (*_values)[ind];}
		inline char getCP(){ return (_white)?'P':'C'; }
		inline void setWHITE() { _white = 0;}
		inline list<Node*>* getAdj(){ return _viz; }
		inline void addViz(Node* n){_viz->push_front(n);}
		inline uint getColor(){ return _color;}
		inline pair<uint,uint>* getPapi() { return _papi;}
		inline void setColor( uint color) { _color = color;	}
		inline void setPapi(pair<uint,uint>* papi) { _papi = papi; }

};


class Graph{
	private:
		vector<Node*>*_nodes;
		//unsigned long _m;
		map<pair<uint,uint>,pair<uint,uint*>>* _edges;
		//unsigned long _e=0;

	public:
		//inline unsigned long getM(){return _m;};

		inline pair<uint,uint*> getEdge(pair<uint,uint> nodes) {return (*_edges)[nodes];};
		inline Node* getNode(uint id) {
			return (*_nodes)[id];
		}
		inline void setEdge(pair<uint,uint> nodes, uint *value, uint fluxo =0) {(*_edges).insert(pair<pair<uint,uint>,pair<uint,uint*>>(nodes,make_pair(fluxo,value)));}
		//inline unsigned long getE(){return _e;};
		inline vector<Node*>* getNodes(){return _nodes;}
		inline map<pair<uint,uint>,pair<uint,uint*>>* getEdges(){ return _edges;}

		Graph(vector<Node*> *nodes, map<pair<uint,uint>,pair<uint,uint*>>* edges){
			_nodes=nodes;
			//cout << _nodes;
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
			for(uint i = 0; i<h;i++) _matrix[i] = new bool[w];
			_corte = new list<pair<uint,uint>>();
			_g = g;
		}

		uint getTotalScore() { return _total;};
		void setGraph( Graph* g) { _g = g;}
		Graph * getGraph() { return _g;}
		void printMatrix() {
			char c = 0;
			cout << _total << c << endl << endl;
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
		}
		void CalculateTotal() {
			for(uint i=0; i<_h;i++)
				for(uint j=0; j<_w;j++)
					_total += _g->getNode(i*j)->getValue(_matrix[i][j]);


		}
};


class PutoEdmond{
	private:
		Graph* _g;
		queue<Node*>* _q;
		vector<Node*>*  vNodes;
		map<pair<uint,uint>,pair<uint,uint*>>* _fc;
		Node* _s;
		Node* _t;
		list<pair<uint,uint>*> * caminho;
		uint maxFlow;
	public:
		PutoEdmond(Graph* g){
			_g = g;
			_q = new queue<Node*>;
			_fc = _g->getEdges();
			vNodes = _g->getNodes();
			initialize();
		}
		void initialize(){
			_s = new Node(S);
			_t = new Node(T);
			//s->setValue(0);
			/*_g->(*nodes)[M*N+1]->setValue(0,0);*/
			for (uint i=0; i<M*N; i++){
					uint* value_w = new uint((*vNodes)[i]->getValue());
					uint* value_b = new uint((*vNodes)[i]->getValue(1));
					(*vNodes)[i]->getAdj()->push_front(_t);
					_s->addViz((*vNodes)[i]);
					//cout << *value_w << '|' << *value_b << endl;
					if(*value_w > *value_b){
						_g->setEdge(make_pair(S,i),value_w, *value_w-*value_b);
						_g->setEdge(make_pair(i,T),value_b, *value_b);
					} else {
						_g->setEdge(make_pair(S,i),value_w, *value_w);
						_g->setEdge(make_pair(i,T),value_b, *value_b-*value_w);
					}
			}
		}

		void run(){
			do {
				cout << "Pre BFS" << endl;
				//printResidual();
				maxFlow = -1;
				BFS();
				//cout << "Pro BFS" << endl;
				//printResidual();
			} while( maxFlow != (uint)-1);
			printResidual();
			printFinalResult();
			//DFS();
		}
		void printFinalResult(){
			uint soma=0;
			for (uint i=0; i < M*N; i++){
				//cout << "FLUXO " << (*_fc)[make_pair(_s->getId(),i)].first << '-' << *(*_fc)[make_pair(_s->getId(),i)].second << endl;
				soma += (*_fc)[make_pair(_s->getId(),i)].first;
			}
			char c;
			cout << soma << endl << endl;
			for(uint i=0; i<M; i++){
				c = ((*vNodes)[getCoor(i,0)]->getColor()==WHITE)?'P':'C';
				cout << c;
				for(uint j=1; j<N; j++) {
					c = ((*vNodes)[getCoor(i,j)]->getColor()==WHITE)?'P':'C';
					cout << ' ' << c;
				}
				cout << endl;
			}
		}
		void printResidual(){
			map<pair<uint,uint>,pair<uint,uint*>>::iterator it = _fc->begin();
			for(;it != _fc->end(); it++){
				cout << '[' << it->first.first << ',' << it->first.second << ']' << "-("
					<< it->second.first << ',' << *it->second.second << ')' << endl;
			}
		}
		void BFS(){
			for (uint i = 0; i< M*N; i++) {
				(*vNodes)[i]->setColor(WHITE);
				(*vNodes)[i]->setPapi(NULL);
			}

			caminho = new list<pair<uint,uint>*>;
			_q->push(_s);

			while (!_q->empty()){

				Node* u = _q->front();
				_q->pop();
				//cout << u << endl;
				for(Node * v: (*u->getAdj())){
					pair<uint,uint*> fc = (*_fc)[make_pair(u->getId(),v->getId())];
					//cout << u->getId() << '-' << v->getId() <<'&' << fc.first << '|' << *fc.second << '='<< (*fc.second - fc.first) << endl;
					if(v->getColor() == WHITE && (*fc.second - fc.first)>0){
						cout << "ENTROU" << endl;
						v->setColor(GRAY);
						pair<uint,uint> * e = new pair<uint,uint> (u->getId(), v->getId());
						v->setPapi(e);
						_q->push(v);
						//cout << v << '|' << _t << endl;
						if(v == _t){
							cout << "actualizar Fluxo" << endl;
							actualizaFluxos(e);
							return;
						}
					}
				}
				u->setColor(BLACK);
			}
		}

		void actualizaFluxos(pair<uint,uint>* ut){
			uint edgeFlow = *(*_fc)[*ut].second - (*_fc)[*ut].first;

			if(edgeFlow < maxFlow){
				maxFlow = edgeFlow;
			}

			caminho->push_front(ut);

			if(ut->first != _s->getId()){
				actualizaFluxos((*vNodes)[ut->first]->getPapi());
			}
			cout <<"MF" << maxFlow << endl;
			(*_fc)[*ut].first +=maxFlow;
		}
/*
		void DFS(){
			for (uint i = 0; i< M*N; i++) {
				(*vNodes)[i]->setColor(WHITE);
			}
			visitDFS(_s);
		}
		void visitDFS(Node * u){
			u->setColor(GRAY);
			u->setWHITE();

			for(Node * v: (*u->getAdj())){
				pair<uint,uint*> fc = (*_fc)[make_pair(u->getId(),v->getId())];
				if(v->getColor() == WHITE && (*fc.second - fc.first)>0)
					visitDFS(v);
			}
			u->setColor(BLACK);
		}
*/
};


Graph* parse() {
	int m;
	int n;

	cin >> m;
	cin >> n;
	M=m;
	N=n;

	int total = m*n;

	vector<Node*>* nodes = new vector<Node*>(total+2); //MAIS 2 PARA S E T
	map<pair<uint,uint>,pair<uint,uint*>>* edges = new map<pair<uint,uint>,pair<uint,uint*>>; //ESTÁ PARA PONTEIRO NAO PARA INT
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
			uint id1 = getCoor(i,j);
			uint id2 = getCoor(i,j)+1;
			edges->insert(pair<pair<uint,uint>,pair<uint, uint*>>(make_pair(id1,id2),make_pair(0,v_hor)));
			edges->insert(pair<pair<uint,uint>,pair<uint, uint*>>(make_pair(id2, id1),make_pair(0,v_hor)));
			(*nodes)[id1]->addViz((*nodes)[id2]);
			(*nodes)[id2]->addViz((*nodes)[id1]);

		}
	}

	uint *v_ver ;
	for (int i=0; i<m-1; i++){
		for (int j=0;j<n; j++){
			v_ver = new uint();
			cin >> *v_ver;
			uint id1 = getCoor(i,j);
			uint id2 = getCoor(i,j)+N;
			edges->insert(pair<pair<uint,uint>,pair<uint, uint*>>(make_pair(id1,id2),make_pair(0,v_ver)));
			edges->insert(pair<pair<uint,uint>,pair<uint, uint*>>(make_pair(id2,id1),make_pair(0,v_ver)));
			(*nodes)[id1]->addViz((*nodes)[id2]);
			(*nodes)[id2]->addViz((*nodes)[id1]);
		}
	}

	return new Graph(nodes,edges);
}

int main() {
	Graph *g = parse();
	PutoEdmond* PE = new PutoEdmond(g);
	PE->run();
//	Mask *m = new Mask(N,M,g);
/*	cout << 'M' << endl;*/

	return 0;
}
