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

typedef unsigned ulint;
using namespace std;


ulint N;
ulint M;


class Node{
	private:
		ulint _id;
		vector<ulint>* _values = new vector<ulint>(2);
		list<Node*>* _viz = new list<Node*>;
		ulint _color = WHITE;
		pair<ulint,ulint>* _papi;
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
		inline list<Node*>* getAdj(){ return _viz; }
		inline void addViz(Node* n){_viz->push_front(n);}
		inline ulint getColor(){ return _color;}
		inline pair<ulint,ulint>* getPapi() { return _papi;}
		inline void setColor( ulint color) { _color = color;	}
		inline void setPapi(pair<ulint,ulint>* papi) { _papi = papi; }

};


class Graph{
	private:
		vector<Node*>*_nodes;
		//unsigned long _m;
		map<pair<ulint,ulint>,pair<int,ulint*>>* _edges;
		//unsigned long _e=0;

	public:
		//inline unsigned long getM(){return _m;};

		inline pair<int,ulint*> getEdge(pair<ulint,ulint> nodes) {return (*_edges)[nodes];};
		inline Node* getNode(ulint id) {
			return (*_nodes)[id];
		}
		inline void setEdge(pair<ulint,ulint> nodes, ulint *value, int fluxo =0) {(*_edges).insert(pair<pair<ulint,ulint>,pair<ulint,ulint*>>(nodes,make_pair(fluxo,value)));}
		//inline unsigned long getE(){return _e;};
		inline vector<Node*>* getNodes(){return _nodes;}
		inline map<pair<ulint,ulint>,pair<int,ulint*>>* getEdges(){ return _edges;}

		Graph(vector<Node*> *nodes, map<pair<ulint,ulint>,pair<int,ulint*>>* edges){
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
		ulint _h,_w;
		ulint _total;
		Graph* _g;
		list<pair<ulint,ulint>>* _corte;
	public:
		Mask(ulint h, ulint w, Graph* g) {
			_h=h;_w=w;
			_matrix = new bool*[h];
			for(ulint i = 0; i<h;i++) _matrix[i] = new bool[w];
			_corte = new list<pair<ulint,ulint>>();
			_g = g;
		}

		ulint getTotalScore() { return _total;};
		void setGraph( Graph* g) { _g = g;}
		Graph * getGraph() { return _g;}
		void printMatrix() {
			char c = 0;
			cout << _total << c << endl << endl;
			for(ulint i=0; i<_h;i++){
				c = (_matrix[i][0])?'C':'P';
				cout << _g->getNode((i*_w))->getValue(_matrix[i][0]) << '|' << i << '-' << 0;
				for(ulint j=1; j<_w;j++) {
					c = (_matrix[i][0])?'C':'P';
					/*cout << ' ' << c;*/
					cout << ' ' << _g->getNode((i*_w)+(j))->getValue(_matrix[i][j])<< '|' << i+1 << '-' << j;;
				}
				cout << endl;
			}
			cout << 'C' << endl;
		}
		void CalculateTotal() {
			for(ulint i=0; i<_h;i++)
				for(ulint j=0; j<_w;j++)
					_total += _g->getNode(i*j)->getValue(_matrix[i][j]);


		}
};


class PutoEdmond{
	private:
		Graph* _g;
		queue<Node*>* _q;
		vector<Node*>*  vNodes;
		map<pair<ulint,ulint>,pair<int,ulint*>>* _fc;
		Node* _s;
		Node* _t;
		list<pair<ulint,ulint>*> * caminho;
		ulint maxFlow;
		int actualizadoFluxo;
		ulint totalFlow;
	public:
		PutoEdmond(Graph* g){
			_g = g;
			_fc = _g->getEdges();
			vNodes = _g->getNodes();
			initialize();
		}
		void initialize(){
			_s = new Node(S);
			_t = new Node(T);
			//s->setValue(0);
			/*_g->(*nodes)[M*N+1]->setValue(0,0);*/
			for (ulint i=0; i<M*N; i++){
					ulint* value_w = new ulint((*vNodes)[i]->getValue());
					ulint* value_b = new ulint((*vNodes)[i]->getValue(1));
					//cout << *value_w << '|' << *value_b << endl;

					if(*value_w > *value_b){
						totalFlow += *value_b;
						*value_w -= *value_b;
						_s->addViz((*vNodes)[i]);
						_g->setEdge(make_pair(S,i),value_w, 0);  //value fluxo
					} else if (*value_w < *value_b){
						(*vNodes)[i]->getAdj()->push_front(_t);
						totalFlow += *value_w;
						*value_b -= *value_w;
						_g->setEdge(make_pair(i,T),value_b, 0);
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
			} while(actualizadoFluxo == 1);
			//printResidual();
			printFinalResult();
			//DFS();
		}
		void printFinalResult(){
			/*ulint soma=0;
			for (ulint i=0; i < M*N; i++){
				soma += (*_fc)[make_pair(_s->getId(),i)].first;
			}*/
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
		void printResidual(){
			map<pair<ulint,ulint>,pair<int,ulint*>>::iterator it = _fc->begin();
			for(;it != _fc->end(); it++){
				cout << '[' << it->first.first << ',' << it->first.second << ']' << "-("
					<< it->second.first << ',' << *it->second.second << ')' << endl;
			}
		}
		void BFS(){
			for (ulint i = 0; i< M*N; i++) {
				(*vNodes)[i]->setColor(WHITE);
				if (!(*vNodes)[i]->getPapi()) delete((*vNodes)[i]->getPapi());
			}
			_t->setColor(WHITE);
			if (caminho){
				delete(caminho);
			}
			//caminho = new list<pair<ulint,ulint>*>;
			_q = new queue<Node*>;
			_q->push(_s);

			while (!_q->empty()){

				Node* u = _q->front();

				_q->pop();
				//cout << u << endl;
				for(Node * v: (*u->getAdj())){
					//cout << u->getId() << '-' << v->getId() << endl;
					pair<int,ulint*> fc = (*_fc)[make_pair(u->getId(),v->getId())];
					uint diff = *fc.second - fc.first; //cout << u->getId() << '-' << v->getId() <<'&' << fc.first << '|' << *fc.second << '='<< (*fc.second - fc.first) << endl;
					if(v->getColor() == WHITE && (diff)>0){
						//cout << "ENTROU" << endl;
						if( (diff) < maxFlow) maxFlow = diff;
						v->setColor(GRAY);
						pair<ulint,ulint> * e = new pair<ulint,ulint> (u->getId(), v->getId());
						v->setPapi(e);
						_q->push(v);
						//cout << v << '|' << _t << endl;
						if(v == _t){
							//cout << "actualizar Fluxo" << endl;
							actualizaFluxos(e);
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

		void actualizaFluxos(pair<ulint,ulint>* ut){
			//cout << "-1-" << ut->first <<'|'<< ut->second << endl;
			//cout << caminho << '|' << ut << endl;
			////caminho->push_front(ut);
			//cout << "-2-" << ut->first << endl;
			//cout << 'S' << _s->getId() << endl;
			//cout << 'T' << _t->getId() << endl;
			////cout << '-' << (*vNodes)[ut->first] << endl;
			(*_fc)[*ut].first += maxFlow;
			if(ut->first != _s->getId()){
				actualizaFluxos((*vNodes)[ut->first]->getPapi());
			}
			//cout << "-3-" << endl;
			//cout << "MF" << maxFlow << endl;
			//pair<ulint,ulint> inv_ut = make_pair(ut->second,ut->first);
			//totalFlow += maxFlow;
			//(*_fc)[*ut].first += maxFlow;
			//cout << "UT_INV" << ut->second << '-' << ut->first << '|'<< (*_fc)[inv_ut].first  << endl;
			//(*_fc)[inv_ut].first = -(*_fc)[*ut].first;
		}
/*
		void DFS(){
			for (ulint i = 0; i< M*N; i++) {
				(*vNodes)[i]->setColor(WHITE);
			}
			visitDFS(_s);
		}
		void visitDFS(Node * u){
			u->setColor(GRAY);
			u->setWHITE();

			for(Node * v: (*u->getAdj())){
				pair<ulint,ulint*> fc = (*_fc)[make_pair(u->getId(),v->getId())];
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
	map<pair<ulint,ulint>,pair<int,ulint*>>* edges = new map<pair<ulint,ulint>,pair<int,ulint*>>; //ESTÁ PARA PONTEIRO NAO PARA INT
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
			edges->insert(pair<pair<ulint,ulint>,pair<int, ulint*>>(make_pair(id1,id2),make_pair(0,v_hor)));
			edges->insert(pair<pair<ulint,ulint>,pair<int, ulint*>>(make_pair(id2, id1),make_pair(0,v_hor)));

			(*nodes)[id1]->addViz((*nodes)[id2]);
			(*nodes)[id2]->addViz((*nodes)[id1]);

		}
	}

	ulint *v_ver;
	for (int i=0; i<m-1; i++){
		for (int j=0;j<n; j++){
			v_ver = new ulint();
			cin >> *v_ver;
			ulint id1 = getCoor(i,j);
			ulint id2 = getCoor(i,j)+N;
			edges->insert(pair<pair<ulint,ulint>,pair<int, ulint*>>(make_pair(id1,id2),make_pair(0,v_ver)));
			edges->insert(pair<pair<ulint,ulint>,pair<int, ulint*>>(make_pair(id2,id1),make_pair(0,v_ver)));

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

