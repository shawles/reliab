//#include <iostream>

#include <iostream>
#include <cstdlib>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;

//#define LEN 8
//#define POW2 256
#define LEN 36
#define POW2 34359738368

using std::set;

using std::vector;
using std::string;

class func {
	public:
		int x, y, a, b, nxt;
		vector <string> ops;
		vector <int> nums;
		void next_op(int&);
	};

void itoa(long long n, vector <int> & s, int len) {
	int i;
	char c;
	for (i = 0; i < len; i++) {
		s[i] = 0;
	}
	i = 0;
	while (n > 0) {
		s[i] = n % 2;
		n /= 2;
		i++;
	}
	for (i = 0; i < len / 2; i++) {
		c = s[i];
		s[i] = s[len - i - 1];
		s[len - i - 1] = c;
	}
	
}

int get_state(int cf, int cg, int h, func &F, func &G) {
	return cf * 10000000 + cg * 100000 + h * 10000 + F.x * 1000 + F.y * 100 + G.x * 10 + G.y;
}

int process_order(vector <int> s, set <int> &states, func& F, func& G) {
	int cf = 0, cg = 0, i, st, len;
	len = s.size();
	F.x = 6;
	F.y = 6;
	F.nxt = F.ops.size() - 1;
	G.x = 6;
	G.y = 6;
	G.nxt = G.ops.size() - 1;
	int h = 6;
	set <int> localstates;
	for (i = 0; i < len; i++) {
		//cout << "i is " << i << endl;
		if (s[i] == 0) {
			cf++;
			//F.next_op(h);
		}
		else {
			cg++;
			//G.next_op(h);
		}
		if (F.nums[F.nxt] != -1 && G.nums[G.nxt] != -1) {
			st = get_state(F.nums[F.nxt], G.nums[G.nxt], h, F, G);
			//
			states.insert(st);
			if (localstates.find(st) == localstates.end()) {
				localstates.insert(st);
				outfile << F.nums[F.nxt] - 1 << " " << G.nums[G.nxt] - 1 << " ";
				if (h == 6) {
					outfile << '# ';
				}
				else {
					outfile << h << " ";
				}
				 outfile << "  " << h << " " << F.x << "  " << F.y << "  " << G.x << "  " << G.y << endl;
				}
			//else {
			//	break;
			//}
		}
		if (s[i] == 0) {
			F.next_op(h);
		}
		else {
			G.next_op(h);
		}
	}
}

int sum(vector <int> ar) {
	int i, s = 0;
	int len = ar.size();
	for (i = 0; i < len; i++) {
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
				cout << "h=a" << a << endl;
				h = a;
			}
			else {
				h = op[2] - '0';
				cout << "h=" << op[2] - '0' << endl;
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
		if (op.length() > 3) {
			nxt = nxt * 10 + op[3] - '0';
		}
		cout << "g" << nxt << endl;
	}
	else if (op[0] == 'i') {
		//init
		nxt = 0;
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
	long long i, j = 0;
	ofstream outfile;
	
	
	outfile.open("states.txt");
	set <int> states;
	func F, G;
	int h = -1;
	F.a = atoi(argv[1]);
	F.b = atoi(argv[2]);
	G.a = atoi(argv[3]);
	G.b = atoi(argv[4]);
	cout << F.a << endl;
	//F.ops = {"x=3", "y=1", "end"};
	F.ops = {"x=3", "y=1", "h=a", "h<y+a?6", "y=8", "gt9", "x<4?8", "x=4", "x=2", "end", "init"};
	F.nums = {2, 3, 4, 5, 6, -1, 7, 8, 9, 10, 1};
	G.ops = {"x=3", "y=5", "h=2", "x=1", "x<7?14", "h>0?7", "gt14", "y<5?9", "h=x", "h<b-x?4", "x<10?12", "x=3", "y=0", "gt4", "end", "init"};
	G.nums = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, -1, 15, 1};
	long long max_check = POW2;
	int len = LEN;
	int g_len = 26;
	if (F.a > 0) {
		max_check = 262144;
		len = 18; 
		g_len = 9;
	}
	else if (F.a >= G.b - 1) {
		max_check = 524288;
		len = 19;
		g_len = 10;
	}
	cout << "len is " << len << endl;
	vector <int> buffer(len);
	int start = 1;
	for (i = 0; i < g_len; i++) {
		start *= 2;
	}
	itoa(start - 1, buffer, len);
	//for (i = start - 1; i < max_check; i++) {
	do {
	//int cnst = 343249;
	//for (i = cnst; i < cnst + 1; i++) {
		//itoa(i, buffer, len);
		//if (1 || sum(buffer) == g_len) {
		cout << "\n\n" << i << " ";
		for (int j = 0; j < len; j++) {
			cout << buffer[j];
		}
		cout << endl;
		outfile << "c_f c_g h f.x f.y g.x g.y" << endl;
		process_order(buffer, states, F, G);
		//j = i + 1;
		//cout << j << endl;
	} while (std::next_permutation(buffer.begin(), buffer.end())); 
	cout << states.size() << endl;
	outfile.close();
	return 0;
}
