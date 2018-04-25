#include <iostream>
#include <list>
#include <stack>
#include <utility>

typedef unsigned int uint;

using namespace std;


class Mask{
	private:
		bool ** _matrix;
		uint _h,_w;
		uint _total;
		Graph* _g;
		list<pair<uint,uint>>* _corte = new list<pair<uint,uint>>;
	public:
		Mask(uint h, uint w) {
			_h=h;_w=w;
			_matrix = new bool*[h];
			for(int i = 0; i<h;i++) _matrix[i] = new bool[w];
			
		}
		uint getTotalWhiteScore() { return _totalWhite;};
		void addWhiteNode( Node* n) { _white.push_front (n); };
		void addBlackNode( Node* n) { _black.push_front (n); };
		void setGraph( Graph* g) { _g = g;};
		Graph * getGraph() { return _g;};
		void printMatrix() {
			for(uint i=0; i<_h;i++){
				cout << (bool[i][0])?'C':'P';
				for(uint j=1; j<_w;j++)
					cout << ' ' << (bool[i][j])?'C':'P';
			}
				cout << endl;
		};
		void CalculateTotal() {
			Node * u;
			for(uint i=0; i<_h;i++)
				for(uint j=0; j<_w;j++)
					_total += _g.getNode(i*j).getValue(_matrix[i][j]);

			for(pair<uint,uint> p : _corte){
				_total += getEdge(p);
			}
		};
}