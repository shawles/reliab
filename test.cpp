//#include <iostream>

#include <iostream>
#include <cstdlib>
#include <set>
#include <vector>
#include <string>
using namespace std;

//#define LEN 8
//#define POW2 256
#define LEN 27
#define POW2 134217728

using std::set;

using std::vector;
using std::string;

class func {
	public:
		int x, y, a, b, nxt;
		vector <string> ops;
		void next_op(int&);
	};

void itoa(int n, int s[]) {
	int i;
	char c;
	for (i = 0; i < LEN; i++) {
		s[i] = 0;
	}
	i = 0;
	while (n > 0) {
		s[i] = n % 2;
		n /= 2;
		i++;
	}
	for (i = 0; i < LEN / 2; i++) {
		c = s[i];
		s[i] = s[LEN - i - 1];
		s[LEN - i - 1] = c;
	}
	
}

int get_state(int cf, int cg) {
	printf("[%i %i]\n", cf, cg);
	char st[4];
	int i;
	for (i = 0; i < 4; i++) {
		st[i] = '0';
	}
	if (cf < 3) {
		st[0] = '8';
	}
	else if (cf < 8) {
		st[0] = '3';
	}
	if (cf < 4) {
		st[1] = '8';
	}
	else {
		st[1] = '1';
	}
	printf("%i ", atoi(st));
	return atoi(st);
}

int process_order(int s[], set <int> states, func& F, func& G, int& h) {
	int cf = 0, cg = 0, i, st;
	F.x = -1;
	F.y = -1;
	F.nxt = 0;
	G.x = -1;
	G.y = -1;
	G.nxt = 0;
	for (i = 0; i < LEN; i++) {
		//cout << "i is " << i << endl;
		if (s[i] == 0) {
			cf++;
			F.next_op(h);
		}
		else {
			cg++;
			G.next_op(h);
		}
		cout << cf - 1 << "  " << cg - 1 << "  " << h << " " << F.x << "  " << F.y << "  " << G.x << "  " << G.y << endl;
	}
}

int sum(int ar[]) {
	int i, s = 0;
	for (i = 0; i < LEN; i++) {
		s = s + ar[i];
	}
	return s;
}



void func::next_op(int & h) {
	string op = ops[nxt];
	if (op[1] == '=') {
		if (op[0] == 'x') {
			x = op[2] - '0';
			cout << "x=" << op[2] << endl;
		}
		else if (op[0] == 'y') {
			y = op[2] - '0';
			cout << "y=" << op[2] << endl;
		}
		else {
			if (op[2] == 'x') {
				h = x;
				cout << "h=x" << x << endl;
			}
			else if (op[2] == 'a') {
				cout << "h=a" << endl;
				h = a;
			}
			else {
				h = op[2] - '0';
				cout << "h=op" << endl;
			}
		}
		nxt = nxt + 1;
	}
	else if (op[0] == 'e') { //end
		//do nothing, nxt is the same
	}
	else if (op[1] == '<') {
		int cmpl, cmpr;
		if (op[2] >= '0' && op[2] <= '9') {
			cmpr = op[2] - '0';
			if (op[3] != '?') {
				cmpr = cmpr * 10 + op[3] - '0';
			}
			if (op[0] == 'x') {
				cmpl = x;
				cout << "x<";
			}
			else {
				cmpl = y;
				cout << "y<";
			}
		}
		else {
			cout << "h<";
			if (op[2] == 'y') {
				cmpr = y + a;
			}
			else {
				cmpr = b - x;
			}
			cmpl = h;
		}
		cout << cmpr;
		if (cmpl >= cmpr) {
			int ind = op.find('?');
			nxt = op[ind + 1] - '0';
			if (op.length() != ind + 2) {
				nxt *= 10;
				nxt = nxt + op[ind + 2] - '0';
			}
		}
		else {
			nxt = nxt + 1;
		}
		cout << nxt << endl;
	}
	else if (op[0] == 'g') {
		nxt = op[2] - '0';
		cout << "g" << nxt << endl;
	}
	else { //h > 0 ? 7 
		if (h <= 0) {
			nxt = 7;
		}
		else {
			nxt = 6;
		}
		cout << "h>0 " << nxt << endl;
	}
}
		
			

int main(int argc, char* argv[]) {
	int i, j = 0;
	int buffer[LEN];
	set <int> states;
	func F, G;
	int h = -1;
	F.a = atoi(argv[1]);
	F.b = atoi(argv[2]);
	G.a = atoi(argv[3]);
	G.b = atoi(argv[4]);
	cout << F.a << endl;
	//F.ops = {"x=3", "y=1", "end"};
	F.ops = {"x=3", "y=1", "h=a", "h<y+a?6", "y=8", "gt9", "x<4?8", "x=4", "x=2", "end"};
	G.ops = {"x=3", "y=5", "h=2", "x=1", "x<7?6", "h>0?7", "end", "y<5?9", "h=x", "h<b-x?4", "x<10?12", "x=3", "y=0", "gt5"};
	
	//for (i = 0; i < POW2; i++) {
	int cnst = 119279615;
	for (i = cnst; i < cnst + 1; i++) {
		itoa(i, buffer);
		if (sum(buffer) == 18) {
			cout << "\n\n" << i << " ";
			for (int j = 0; j < LEN; j++) {
				cout << buffer[j];
			}
			cout << endl;
			cout << "cf cg h fx fy gx gy" << endl;
			process_order(buffer, states, F, G, h);
		}
		//j = i + 1;
		//cout << j << endl;
	}
	printf("%i\n", j);
	return 0;
}
