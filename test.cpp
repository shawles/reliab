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

//максимальная длина последовательности операторов
#define LEN 38
#define POW2 34359738368 * 4

using std::set;
using std::vector;
using std::string;

//класс для хранения состояния функций
//x, y, a, b - локальные переменные
//nxt - индекс следующего оператора в ops
//ops - вектор операторов
//nums - nums[i]-той строке соответствует i-й оператор
//next_op - функция, которая выполняет следующий оператор
class func {
	public:
		int x, y, a, b, nxt;
		vector <string> ops;
		vector <int> nums;
		void next_op(int&, bool);
	};


//получение представления состояния в виде одного числа int, 
//две первых цифры - c_f, потом c_g и так далее.
//это корректно, потому что эти значения (кроме счетчиков)
//в основном могут быть только однозначными
int get_state(int cf, int cg, int h, func &F, func &G) {
	int h1 = h;
	if ((h < 0) || (h == F.a && F.a >= 10)) {
		h1 = 0;
	}
	return cf * 10000000 + cg * 100000 + h1 * 10000 + F.x * 1000 + F.y * 100 + G.x * 10 + G.y;
}

//функция для печати состояния, в т.ч. замены неиниц. на #.
//6 - это число для неинициализированных значений (вместо -1),
//потому что 6 может быть равно только h и то в случае, если F.a == h == 6.
void print(ofstream& outfile, func F, func G, int h) {
	int fn = F.nums[F.nxt], gn = G.nums[G.nxt];
	if (fn == -1) {
		fn = F.nums[F.nxt - 1];
	}
	if (gn == -1) {
		gn = G.nums[G.nxt - 1];
	}
	outfile << fn - 1 << "   " << gn - 1 << "   ";
	if ((h == 6) && (F.a != 6)) {
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

//то же самое для ostream
void print(ostream& outfile, func F, func G, int h) {
	int fn = F.nums[F.nxt], gn = G.nums[G.nxt];
	if (fn == -1) {
		fn = F.nums[F.nxt - 1];
	}
	if (gn == -1) {
		gn = G.nums[G.nxt - 1];
	}
	outfile << fn - 1 << "   " << gn - 1 << "   ";
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

//функция, обрабатывающая один взаимный порядок операторов, задаваемый s
void process_order(vector <int> s, set <int> &states, func& F, func& G, ofstream& outfile, bool oa) {
	int i, st, len;
	//инициализация; про 6 объяснялось раньше
	len = s.size();
	F.x = 6;
	F.y = 6;
	F.nxt = 11;
	G.x = 6;
	G.y = 6;
	G.nxt = 16;
	int h = 6;
	//для печати состояний каждого порядка
	set <int> localstates;
	//цикл по всем операторам обеих функций в порядке их выполнения
	for (i = 0; i < len; i++) {
		//проверка, надо ли печатать следующее состояние 
		//(некоторые не имеют аналогов в оригинальном коде)
		//if (F.nums[F.nxt] != -1 && G.nums[G.nxt] != -1) {
		if (true) {
			//получить представление состояния в int
			st = get_state(F.nums[F.nxt], G.nums[G.nxt], h, F, G);
			
			//если такого еще не было, добавить в set и в выходной файл
			if (states.find(st) == states.end()) {
				print(outfile, F, G, h);
				states.insert(st);
			}
			//если такого не было в этом порядке, вывести, если вообще надо печатать
			if (localstates.find(st) == localstates.end()) {
				localstates.insert(st);
			}
			if (oa) {
				print(cout, F, G, h);
			}			
		}
		//else {
		//	cout << "-1: ";
		//	print(cout, F, G, h);
		//}
		//выполнить очередной оператор в F или G
		if (s[i] == 0) {
			F.next_op(h, oa);
		}
		else {
			G.next_op(h, oa);
		}
	}
}

//суммировать значения вектора, вроде уже не нужна
int sum(vector <int> ar) {
	int i, s = 0;
	int len = ar.size();
	for (i = 0; i < len; i++) {
		s = s + ar[i];
	}
	return s;
}


//реализация чего-то вроде конечного автомата, моделирующего работу функций
void func::next_op(int & h, bool oa) {
	string op = ops[nxt];
	if (oa) {
		if (ops.size() == 12) {
			cout << "f: ";
		}
		else {
			cout << "g: ";
		}
	}
	//оператор присваивания: определить, что и кому присваивается, 
	//перейти к следующей команде
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
	//конечное состояние, никуда не переходить
	else if (op[0] == 'e') { //end
		//do nothing, nxt is the same
	}
	//if A < B ? C: если !(A < B), перейти к команде С, иначе к следующей
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
	//безусловный переход к команде
	else if (op[0] == 'g') {
		nxt = op[2] - '0';
		if (op.length() > 3) {
			nxt = nxt * 10 + op[3] - '0';
		}
		if (oa) {
			cout << "gt " << nxt << endl;
		}
	}
	//инициализация переменных х, у
	else if (op[0] == 'i') {
		//init
		if (oa) {
			cout << "int x, y" << endl;
		}
		nxt = 0;
	}
	//операторы x > ..., это всегда False, поэтому просто переход
	//к следующей команде
	else if (op[0] == 'x' && op[1] == '>') {
		nxt = nxt + 1;
		if (oa) {
			cout << "x>? " << endl;
		}
	}
	//оператор h > 0 в G, надо проверить условие и, возможно, перейти в конец
	else { //h > 0 ? 7 
		if (h <= 0) {
			nxt = 8;
		}
		else {
			nxt = 7;
		}
		if (oa) {
			cout << "h>0 " << nxt << endl;
		}
	}
}			

//проверить, есть ли в argv строка s 
int check_in(int argc, char* argv[], string s) {
	int i = 1;
	//проверяем всё, кроме имени программы
	while (i < argc) {
		if (!strcmp(argv[i], s.c_str())) {
			return i;
		}
		i++;
	}
	return 0;
}

//Данные о программе, авторе, аргументах, вот это всё.
void print_usage() {
	cout << "Software Reliability, 1st task." << endl;
	cout << "Usage: filename f_a f_b g_a g_b [-count] [-file filename] [-]" << endl;
	cout << "Use -count to output number of states to stdout" << endl;
	cout << "Use -file [filename] to output states to file" << endl;
	cout << "Use -out_all to output everything to stdout" << endl;
	cout << "Ekaterina Shalimova, 420 gr., 2017" << endl;
}

//проверка аргументов командной строки на то, достаточно ли их
//и есть ли всякие дополнительные
int parse_args(int argc, char* argv[], bool& cnt, string& outname, bool& out_all) {
	int i;
	if (argc < 5) {
		//недостаточно аргументов
		print_usage();
		return 1;
	}
	else {
		//проверить, числа ли f_a, f_b, g_a, g_b
		for (i = 1; i < 5; i++) {
			int j = 0; 
			while (argv[i][j] != '\0') {
				if (argv[i][j] < '0' || argv[i][j] > '9') {
					if (j != 0 || argv[i][j] != '-') {
						print_usage();
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
		//других аргументов нет
		if (argc == 5) {
			return 0;
		}
		//или там еще что-то есть
		else {
			if (check_in(argc, argv, "-count") != 0) {
				cnt = true;
			}
			if (check_in(argc, argv, "-out_all") != 0) {
				out_all = true;
			}
			//-file есть, а имени файла нет
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
	
	//наличие или отсутствие аргумента командной строки -count
	bool cnt;
	//имя файла, куда сохранять состояния
	string outname = "states.txt";
	//out_all - подробный вывод всех операций и состояний в stdout
	bool out_all = false;
	
	//если parse_args вернет не 0, то что-то не то с аргументами командной строки
	if (parse_args(argc, argv, cnt, outname, out_all)) {
		return 0;
	}
	//для циклов
	long long i;
	ofstream outfile;
	outfile.open(outname);
	//set состояний
	set <int> states;
	//задание функций, их операторов, локальных a и b
	//описание, какие поля класса зачем, есть в описании класса
	func F, G;
	F.a = atoi(argv[1]);
	F.b = atoi(argv[2]);
	G.a = atoi(argv[3]);
	G.b = atoi(argv[4]);
	F.ops = {"x=3", "y=1", "h=a", "h<y+a?7", "x>7?", "y=8", "gt10", "x<4?9", "x=4", "x=2", "end", "init"};
	F.nums = {2,     3,      4,    5,         6,      7,     7,     8,       9,     10,     11,    1};
	G.ops = {"x=3", "y=5", "h=2", "x>6?", "x=1", "x<7?15", "h>0?8", "gt15", "y<5?9", "h=x", "h<b-x?5", "x<10?13", "x=3", "y=0", "gt5", "end", "init"};
	G.nums = {2,     3,     4,     5,      6,     7,        8,       9,      10,      11,    12,        13,        14,    15,    5,    16,    1};
	//длина последовательности операторов, которые надо перебирать
	int len = LEN;
	//количество операторов функции G
	int g_len = 28;
	//если F.a > 0, то можно обойтись меньшим количеством
	if (F.a > 0) {
		len = 20; 
		g_len = 10;
	}
	//и в этом случае тоже
	else if (F.a >= G.b - 1) {
		len = 22;
		g_len = 12;
	}
	//нужно проверить столько состояний: len! / (g_len! * (len - g_len)!)
	if (out_all) {
		cout << "need to check C(" << g_len << ", " << len << ")" << endl;
	}
	//для хранения последовательности выполнения операторов: 0 - выполнить 
	//следующий оператор F, 1 - G
	vector <int> buffer(len);
	for (i = 0; i < len; i++) {
		if (i < (len - g_len)) {
			buffer[i] = 0;
		}
		else {
			buffer[i] = 1;
		}
	}
	outfile << "c_f c_g h f.x f.y g.x g.y" << endl;
	//обрабатываем порядок, получаем следующую перестановку, 
	//проверяем, не последняя ли она (есть ли еще такие)
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
	
	//вывести количество состояний в stdout, если надо
	if (cnt) {
		cout << states.size() << endl;
	}
	outfile.close();
	return 0;
}
