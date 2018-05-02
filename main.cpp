#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <climits>
#include <set>
#include <unordered_map>
#include <queue>
#include <functional>
#include <utility>


#define S -1
#define T -2
#define WHITE 0
#define GRAY 1
#define BLACK 2

#define getCoor(m,n) (N*m + n)

typedef unsigned ulint;
using namespace std;



ulint N;
ulint M;


class Node{
	private:
		ulint _id;
		vector<ulint>* _values = new vector<ulint>(2);
		list<pair<pair<void*, void*>, pair<ulint, ulint*>>*>* _viz = new list<pair<pair<void*, void*>, pair<ulint, ulint*>>*>;
		ulint _color = WHITE;
		pair<pair<void*,void* >, pair<ulint, ulint*>>* _papi;
		bool _white=1;


	public:
		explicit Node (ulint id) { _id=id; }
		inline ulint getId() { return _id; }
		//inline list<Node*>* getList(){ return _l; };
		inline void setId(ulint id ) { _id= id; }
		inline void setValue(ulint index, ulint value) {(*_values)[index] = value;}
		inline ulint getValue(ulint ind = 0) {return (*_values)[ind];}
		inline char getCP(){ return (_white)?'P':'C'; }
		inline void setWHITE() { _white = 0;}
		inline list<pair<pair<void *, void *>, pair<ulint, ulint*>>*>* getAdj(){ return _viz; }
		inline void addViz(Node* n, ulint* cap, ulint fluxo = 0){
			pair<pair<void *, void *>, pair<ulint, ulint*>>* a = new pair<pair<void *, void *>, pair<ulint, ulint*>>(make_pair(this,n),make_pair(fluxo,cap));
			_viz->push_front(a);
		}
		inline ulint getColor(){ return _color;}
		inline pair<pair<void *, void *>, pair<ulint, ulint*>>* getPapi() { return _papi;}
		inline void setColor( ulint color) { _color = color;	}
		inline void setPapi(pair<pair<void *, void *>, pair<ulint, ulint*>>* papi) { _papi = papi; }

};
typedef pair<pair<void *, void *>, pair<ulint, ulint*>> vfc;
class Graph{
	private:
		vector<Node*>*_nodes;
		//unsigned long _m;
		//unordered_map<pair<ulint,ulint>,pair<int,ulint*>,pair_hash>* _edges;
		//unsigned long _e=0;

	public:
    Graph(vector<Node*> *nodes){
      _nodes=nodes;
      //cout << _nodes;
      //_m=m;

      //_e=e;
    };
    ~Graph(){
      delete _nodes;

    };
		//inline unsigned long getM(){return _m;};

		//inline pair<int,ulint*> getEdge(pair<ulint,ulint> nodes) {return (*_edges).at(nodes);};
		inline Node* getNode(ulint id) {
			return (*_nodes)[id];
		}
		//inline void setEdge(pair<ulint,ulint> nodes, ulint *value, int fluxo =0) {(*_edges).insert(pair<pair<ulint,ulint>,pair<ulint,ulint*>>(nodes,make_pair(fluxo,value)));}
		//inline unsigned long getE(){return _e;};
		inline vector<Node*>* getNodes(){return _nodes;}
		//inline unordered_map<pair<ulint,ulint>,pair<int,ulint*>,pair_hash>* getEdges(){ return _edges;}


};

class PutoEdmond{
	private:
		Graph* _g;
		queue<Node*>* _q;
		vector<Node*>* vNodes;
		Node* _s;
		Node* _t;
		list<vfc*> * caminho;
		ulint maxFlow;
		int actualizadoFluxo;
		ulint totalFlow;
	public:
		PutoEdmond(Graph* g){
			_g = g;
			vNodes = _g->getNodes();
			initialize();
		}
		void initialize(){
			_s = new Node(S);
			_t = new Node(T);
			//s->setValue(0);

			for (ulint i=0; i<M*N; i++){
					ulint* value_w = new ulint((*vNodes)[i]->getValue());
					ulint* value_b = new ulint((*vNodes)[i]->getValue(1));
					//cout << *value_w << '|' << *value_b << endl;

					if(*value_w > *value_b){
						totalFlow += *value_b;
						*value_w -= *value_b;
						_s->addViz((*vNodes)[i],value_w);
						//_g->setEdge(make_pair(S,i),value_w);  //value fluxo
					} else if (*value_w < *value_b){
						(*vNodes)[i]->addViz(_t,value_b);
						totalFlow += *value_w;
						*value_b -= *value_w;
						//_g->setEdge(make_pair(i,T),value_b, 0);
					} else {
						totalFlow += *value_w;
					}
			}
		}

		void run(){
			do {
				//cout << "Pre BFS" << endl;
				//printResidual();
				actualizadoFluxo = 0;
				maxFlow = -1;
				BFS();
				//cout << "AQUI?" << actualizadoFluxo << endl;
				//cout << "Pro BFS" << endl;
				//printResidual();
				//cout << (*_fc)[make_pair(_s->getId(),0)].first;
				//printFinalResult();
			} while(actualizadoFluxo);
			//printResidual();
			printFinalResult();
			//DFS();
		}
		void printFinalResult(){

			char c;
			cout << totalFlow << endl << endl;
			for(ulint i=0; i<M; i++){
				c = ((*vNodes)[getCoor(i,0)]->getColor()==WHITE)?'P':'C';
				cout << c;
				for(ulint j=1; j<N; j++) {
					c = ((*vNodes)[getCoor(i,j)]->getColor()==WHITE)?'P':'C';
					cout << ' ' << c;
				}
				cout << ' ' << endl;
			}
		}

		void BFS(){
			for (ulint i = 0; i< M*N; i++) {
				(*vNodes)[i]->setColor(WHITE);
			}


			_t->setColor(WHITE);
			//if (caminho){
			//	delete(caminho);
			//}
			caminho = new list<vfc*>;
			_q = new queue<Node*>;
			_q->push(_s);
			while (!_q->empty()){

				Node* u = _q->front();

				_q->pop();
				//cout << u << endl;
				for(vfc* v: (*u->getAdj())){
					//pair<int,ulint*> fc = (*_fc)[make_pair(u->getId(),v->getId())];
					ulint diff = *v->second.second - v->second.first; //cout << u->getId() << '-' << v->getId() <<'&' << fc.first << '|' << *fc.second << '='<< (*fc.second - fc.first) << endl;
					Node * l = ((Node*)v->first.second);
					if(l->getColor() == WHITE && (diff)>0){
						//cout << "ENTROU" << endl;

						//if( (diff) < maxFlow) maxFlow = diff;
						l->setColor(GRAY);
						l->setPapi(v);
						//cout << u->getId() << '|' << ((Node*)v.first.first)->getId()
						//	<< '-' << ((Node*)v.first.second)->getId() << '|'
						//	<< ((Node*)((Node*)v.first.second)->getPapi()->first.first)->getId() << '-'
						//	<< ((Node*)((Node*)v.first.second)->getPapi()->first.second)->getId() << endl;
						_q->push(l);
						//cout << v << '|' << _t << endl;
						if(l == _t){
							//cout << "actualizar Fluxo" << endl;
							actualizaFluxos(v);
							totalFlow += maxFlow;
							actualizadoFluxo=1;
							delete _q;
							return;
						}
					}
				}
				u->setColor(BLACK);
			}
		}

		void actualizaFluxos(vfc* ut){
			//cout << "-1-" << ut->first <<'|'<< ut->second << endl;
			//cout << caminho << '|' << ut << endl;
			caminho->push_front(ut);
			//cout << "-2-" << ut->first << endl;
			//cout << 'S' << _s->getId() << endl;
			//cout << 'T' << _t->getId() << endl;
			////cout << '-' << (*vNodes)[ut->first] << endl;
			//ut->second.first += maxFlow;
			ulint diff = *ut->second.second - ut->second.first;
			if( (diff) < maxFlow) maxFlow = diff;
			Node * v = ((Node*)ut->first.first);
			if( v->getId() != _s->getId()) actualizaFluxos(v->getPapi());
			else{
				for(vfc* uut : *caminho){
					uut->second.first += maxFlow;
				}
			}
			//cout << "-3-" << endl;
			//cout << "MF" << maxFlow << endl;
			//pair<ulint,ulint> inv_ut = make_pair(ut->second,ut->first);
			//totalFlow += maxFlow;
			//(*_fc)[*ut].first += maxFlow;
			//cout << "UT_INV" << ut->second << '-' << ut->first << '|'<< (*_fc)[inv_ut].first  << endl;
			//(*_fc)[inv_ut].first = -(*_fc)[*ut].first;
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

	vector<Node*>* nodes = new vector<Node*>(total); //MAIS 2 PARA S E T
	//unordered_map<pair<ulint,ulint>,pair<int,ulint*>,pair_hash>* edges = new unordered_map<pair<ulint,ulint>,pair<int,ulint*>,pair_hash>; //ESTÁ PARA PONTEIRO NAO PARA INT
	for (int i = 0 ; i<total; i++){
		Node* nd = new Node(i);
		(*nodes)[i] = nd;
	}

	ulint v_branco;
	for (int i = 0; i<total; i++) {
		cin >> v_branco;
		(*nodes)[i]->setValue(0,v_branco);
	}

	ulint v_preto;
	for (int i = 0; i<total; i++) {
		cin >> v_preto;
		(*nodes)[i]->setValue(1,v_preto);
	}

	ulint *v_hor;
	for (int i=0; i<m; i++){
		for (int j=0;j<n-1; j++){
			v_hor = new ulint();
			cin >> *v_hor;
			ulint id1 = getCoor(i,j);
			ulint id2 = getCoor(i,j)+1;

			(*nodes)[id1]->addViz((*nodes)[id2],v_hor);
			(*nodes)[id2]->addViz((*nodes)[id1],v_hor);

		}
	}

	ulint *v_ver;
	for (int i=0; i<m-1; i++){
		for (int j=0;j<n; j++){
			v_ver = new ulint();
			cin >> *v_ver;
			ulint id1 = getCoor(i,j);
			ulint id2 = getCoor(i,j)+N;

			(*nodes)[id1]->addViz((*nodes)[id2], v_ver);
			(*nodes)[id2]->addViz((*nodes)[id1], v_ver);
		}
	}

	return new Graph(nodes);
}

int main() {
	Graph *g = parse();
	PutoEdmond* PE = new PutoEdmond(g);
	PE->run();


//	Mask *m = new Mask(N,M,g);
/*	cout << 'M' << endl;*/

	return 0;
}
