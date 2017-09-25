//#include <iostream>

#include <iostream>
#include <cstdlib>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstring>
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
		void next_op(int&, bool);
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
	int h1 = h;
	if (h < 0) {
		h1 = 0;
	}
	return cf * 10000000 + cg * 100000 + h1 * 10000 + F.x * 1000 + F.y * 100 + G.x * 10 + G.y;
}

void print(ofstream& outfile, func F, func G, int h) {
	outfile << F.nums[F.nxt] - 1 << "   " << G.nums[G.nxt] - 1 << "   ";
	if (h == 6) {
		outfile << "#  ";
	}
	else {
		outfile << h << "  ";
	}
	if (F.x == 6) {
		outfile << "#  ";
	}
	else {
		outfile << F.x << "  ";
	}
	if (F.y == 6) {
		outfile << "#  ";
	}
	else {
		outfile << F.y << "  ";
	}
	if (G.x == 6) {
		outfile << "#  ";
	}
	else {
		outfile << G.x << "  ";
	}
	if (G.y == 6) {
		outfile << "#";
	}
	else {
		outfile << G.y;
	}
	outfile << endl;
}

void print(ostream& outfile, func F, func G, int h) {
	outfile << F.nums[F.nxt] - 1 << "   " << G.nums[G.nxt] - 1 << "   ";
	if (h == 6) {
		outfile << "#  ";
	}
	else {
		outfile << h << "  ";
	}
	if (F.x == 6) {
		outfile << "#  ";
	}
	else {
		outfile << F.x << "  ";
	}
	if (F.y == 6) {
		outfile << "#  ";
	}
	else {
		outfile << F.y << "  ";
	}
	if (G.x == 6) {
		outfile << "#  ";
	}
	else {
		outfile << G.x << "  ";
	}
	if (G.y == 6) {
		outfile << "#";
	}
	else {
		outfile << G.y;
	}
	outfile << endl;
}


void process_order(vector <int> s, set <int> &states, func& F, func& G, ofstream& outfile, bool oa) {
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
			if (states.find(st) == states.end()) {
				print(outfile, F, G, h);
				states.insert(st);
			}
			if (localstates.find(st) == localstates.end()) {
				localstates.insert(st);
				if (oa) {
					print(cout, F, G, h);
				}
			}
			else {
				break;
			}
			
		}
		if (s[i] == 0) {
			F.next_op(h, oa);
		}
		else {
			G.next_op(h, oa);
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



void func::next_op(int & h, bool oa) {
	string op = ops[nxt];
	if (op[1] == '=') {
		if (op[0] == 'x') {
			x = op[2] - '0';
			if (oa) {
				cout << "x=" << op[2] << endl;
			}
		}
		else if (op[0] == 'y') {
			y = op[2] - '0';
			if (oa) {
				cout << "y=" << op[2] << endl;
			}
		}
		else {
			if (op[2] == 'x') {
				h = x;
				if (oa) {
					cout << "h=x" << x << endl;
				}
			}
			else if (op[2] == 'a') {
				if (oa) {
					cout << "h=a" << a << endl;
				}
				h = a;
			}
			else {
				h = op[2] - '0';
				if (oa) {
					cout << "h=" << op[2] - '0' << endl;
				}
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
				if (oa) {
					cout << "x<";
				}
			}
			else {
				cmpl = y;
				if (oa) {
					cout << "y<";
				}
			}
		}
		else {
			if (oa) {
				cout << "h<";
			}
			if (op[2] == 'y') {
				cmpr = y + a;
			}
			else {
				cmpr = b - x;
			}
			cmpl = h;
		}
		if (oa) {
			cout << cmpr << " ";
		}
		if (cmpl >= cmpr) {
			int ind = op.find('?');
			nxt = op[ind + 1] - '0';
			if (int(op.length()) != ind + 2) {
				nxt *= 10;
				nxt = nxt + op[ind + 2] - '0';
			}
		}
		else {
			nxt = nxt + 1;
		}
		if (oa) {
			cout << nxt << endl;
		}
	}
	else if (op[0] == 'g') {
		nxt = op[2] - '0';
		if (op.length() > 3) {
			nxt = nxt * 10 + op[3] - '0';
		}
		if (oa) {
			cout << "gt " << nxt << endl;
		}
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
		if (oa) {
			cout << "h>0 " << nxt << endl;
		}
	}
}			

int check_in(int argc, char* argv[], string s) {
	int i = 1;
	while (i < argc) {
		if (!strcmp(argv[i], s.c_str())) {
			return i;
		}
		i++;
	}
	return 0;
}

void print_usage() {
	cout << "Software Reliability, 1st task." << endl;
	cout << "Usage: filename f_a f_b g_a g_b [-count] [-file filename] [-]" << endl;
	cout << "Use -count to output number of states to stdout" << endl;
	cout << "Use -file [filename] to output states to file" << endl;
	cout << "Use - to output everything to stdout" << endl;
	cout << "Ekaterina Shalimova, 420 gr., 2017" << endl;
}

int parse_args(int argc, char* argv[], bool& cnt, string& outname, bool& out_all) {
	int i;
	
	//cout << check_in(argc, argv, "asd") << endl;
	
	if (argc < 5) {
		print_usage();
		return 1;
	}
	else {
		//check f_a, f_b, g_a, g_b
		for (i = 1; i < 5; i++) {
			int j = 0; 
			while (argv[i][j] != '\0') {
				if (argv[i][j] <= '0' || argv[i][j] >= '9') {
					if (j != 0 || argv[i][j] != '-') {
						return 1;
					}
					else {
						j++;
					}
				}
				else {
					j++;
				}
			}
		}
		outname = "states.txt";
		cnt = false;
		out_all = false;
		if (argc == 5) {
			return 0;
		}
		else {
			if (check_in(argc, argv, "-count") != 0) {
				cnt = true;
			}
			if (check_in(argc, argv, "-out_all") != 0) {
				out_all = true;
			}
			int ind = check_in(argc, argv, "-file");
			if (ind == argc - 1) {
				print_usage();
				return 1;
			}
			if (ind != 0) {
				outname = string(argv[ind + 1]);
			}
			return 0;
		}
	}
	return 0;
}
		

int main(int argc, char* argv[]) {
	
	bool cnt;
	string outname = "states.txt";
	bool out_all = false;
	
	if (parse_args(argc, argv, cnt, outname, out_all)) {
		return 0;
	}
	
	long long i;
	ofstream outfile;
	outfile.open(outname);
	set <int> states;
	func F, G;
	F.a = atoi(argv[1]);
	F.b = atoi(argv[2]);
	G.a = atoi(argv[3]);
	G.b = atoi(argv[4]);
	F.ops = {"x=3", "y=1", "h=a", "h<y+a?6", "y=8", "gt9", "x<4?8", "x=4", "x=2", "end", "init"};
	F.nums = {2, 3, 4, 5, 6, -1, 7, 8, 9, 10, 1};
	G.ops = {"x=3", "y=5", "h=2", "x=1", "x<7?14", "h>0?7", "gt14", "y<5?9", "h=x", "h<b-x?4", "x<10?12", "x=3", "y=0", "gt4", "end", "init"};
	G.nums = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, -1, 15, 1};
	int len = LEN;
	int g_len = 26;
	if (F.a > 0) {
		len = 18; 
		g_len = 9;
	}
	else if (F.a >= G.b - 1) {
		len = 19;
		g_len = 10;
	}
	if (out_all) {
		cout << "need to check C(" << g_len << ", " << len << ")" << endl;
	}
	vector <int> buffer(len);
	int start = 1;
	for (i = 0; i < g_len; i++) {
		start *= 2;
	}
	itoa(start - 1, buffer, len);
	outfile << "c_f c_g h f.x f.y g.x g.y" << endl;
	do {
		if (out_all) {
			cout << "\n\n";
			for (int j = 0; j < len; j++) {
				cout << buffer[j];
			}
			cout << endl;
		}
		
		process_order(buffer, states, F, G, outfile, out_all);
	} while (std::next_permutation(buffer.begin(), buffer.end())); 
	if (cnt) {
		cout << states.size() << endl;
	}
	outfile.close();
	return 0;
}
