#include <iostream>
#include <regex>
#include <string>
#include <list>
#include <map>
using std::cout;
using std::endl;
using std::regex;
using std::string;
using std::smatch;
using std::list;
using std::ostream_iterator;
using std::map;
using std::pair;
using std::cin;

const char *c_re[] = {
	"^\\d+(\\.\\d+)?",//数字
	"^\\+",//+
	"^\\-",//-
	"^\\*",//*
	"^/",// /
	"^\\^",//^
	"^ln",//ln
	"^sin",//sin
	"^asin",//asin
	"^cos",//cos
	"^acos",//acos
	"^tan",//tan
	"^atan",//atan
	"^\\(",//(
	"^\\)",//)
	"^pi",//pi
	"^e"//e
};

regex pattern;


typedef struct CELL{
	bool isnum;
	string oper;
	double num;
} Cell;

bool build(list<Cell *> &vec, string str){
	unsigned int i, j;
	smatch res;
	string::iterator iter;
	Cell *cell;
	regex patt(c_re[0]);
	
	if(!vec.empty()){//如果link不为空，清空link
		for(list<Cell *>::iterator iter = vec.begin();iter != vec.end();iter++){
			delete *iter;
		}
		vec.clear();
	}
	
	for(i = 0, j = 0;i < str.length();i++){//为了支持负号，先在负号'-'前插入一个'0' 
		if(j==0 && str[i]=='-')
			str.insert(i, "0");
		if(str[i] == '(')
			j = 0;
		else
			j++;
	}
	
	for(iter = str.begin(); iter != str.end(); iter += res.str().size()){
		string tmp(iter, str.end());
		if(regex_search(tmp, res, pattern)){
			cell = new Cell;//new一个新的Cell用来存放数字或者运算符
			if(regex_search(res.str(), patt)){//判断是否为数字
				cell->isnum = true;
				cell->num = atof(res.str().c_str());//字符串转double
			}else{
				cell->isnum = false;
				cell->oper = res.str();
			}
			vec.push_back(cell);
		}else{
			return false;//不符合输入的要求，返回false
		}
	}
	return true;
}


void initPattern(){
	string str;
	int i;
	str.append(c_re[0]);
	for(i = 1;i < 17;i++){//17为c_re的长度
		str.append("|");
		str.append(c_re[i]); 
	}
	pattern = str;
}

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
bool cal(list<Cell *> &link){
	unsigned int i, j, k, key;
	double num1, num2;
	map <unsigned int, list<Cell *>::iterator> m;
	list<Cell *>::iterator p, p1, p2;
	map<unsigned int, list<Cell *>::iterator>::iterator iter;

	/*
	unisnumed int 32 bits
	8 bits: 括号优先级 :k
	8 bits: 运算优先级 :j
	16bits: 顺序优先级 :i
	*/
	for(k = 0xff, i = 0, p = link.begin();p != link.end();i++, p++){
		if(!(*p)->isnum){
			//j = (*p)->dat.oper->prio;//优先级
			if((*p)->oper == "+")
				j = 8;
			else if((*p)->oper == "-")
				j = 8;
			else if((*p)->oper == "*")
				j = 7;
			else if((*p)->oper == "/")
				j = 7;
			else if((*p)->oper == "^")
				j = 6;
			else if((*p)->oper == "ln")
				j = 6;
			else if((*p)->oper == "sin")
				j = 5;
			else if((*p)->oper == "asin")
				j = 5;
			else if((*p)->oper == "cos")
				j = 5;
			else if((*p)->oper == "acos")
				j = 5;
			else if((*p)->oper == "tan")
				j = 5;
			else if((*p)->oper == "atan")
				j = 5;
			else if((*p)->oper == "(")
				j = 1;
			else if((*p)->oper == ")")
				j = 1;
			else if((*p)->oper == "pi")
				j = 0;
			else if((*p)->oper == "e")
				j = 0;
			else
				return false;//

			if((*p)->oper == "(")
				k--;
			else if((*p)->oper == ")")
				k++;

			key = (k<<24) | (j<<16) | i;
			m.insert(pair<unsigned int, list<Cell *>::iterator>(key, p));//加入map中
		}
	}

	for(iter = m.begin(); iter != m.end(); iter ++){
		p = iter->second;
		if((*p)->oper == "+"){
			p1 = --p;
			p2 = ++++p;
			p--;
			if(p == link.begin() || !(*p1)->isnum) return false;
			if(p2 == link.end() || !(*p2)->isnum) return false;
			num1 = (*p1)->num;
			num2 = (*p2)->num;
			(*p1)->num = num1+num2;
			delete *p;
			delete *p2;
			link.erase(p);
			link.erase(p2);
		}else if((*p)->oper == "-"){
			p1 = --p;
			p2 = ++++p;
			p--;
			if(p == link.begin() || !(*p1)->isnum) return false;
			if(p2 == link.end() || !(*p2)->isnum) return false;
			num1 = (*p1)->num;
			num2 = (*p2)->num;
			(*p1)->num = num1-num2;
			delete *p;
			delete *p2;
			link.erase(p2);
			link.erase(p);
		}else if((*p)->oper == "*"){
			p1 = --p;
			p2 = ++++p;
			p--;
			if(p == link.begin() || !(*p1)->isnum) return false;
			if(p2 == link.end() || !(*p2)->isnum) return false;
			num1 = (*p1)->num;
			num2 = (*p2)->num;
			(*p1)->num = num1*num2;
			delete *p;
			delete *p2;
			link.erase(p2);
			link.erase(p);
		}else if((*p)->oper == "/"){
			p1 = --p;
			p2 = ++++p;
			p--;
			if(p == link.begin() || !(*p1)->isnum) return false;
			if(p2 == link.end() || !(*p2)->isnum) return false;
			num1 = (*p1)->num;
			num2 = (*p2)->num;
			(*p1)->num = num1/num2;
			delete *p;
			delete *p2;
			link.erase(p2);
			link.erase(p);
		}else if((*p)->oper == "^"){
			p1 = --p;
			p2 = ++++p;
			p--;
			if(p == link.begin() || !(*p1)->isnum) return false;
			if(p2 == link.end() || !(*p2)->isnum) return false;
			num1 = (*p1)->num;
			num2 = (*p2)->num;
			(*p1)->num = pow(num1, num2);
			delete *p;
			delete *p2;
			link.erase(p2);
			link.erase(p);
		}else if((*p)->oper == "e"){
			(*p)->isnum = true;//变为数字
			(*p)->num = 2.71828182845904523536;
		}else if((*p)->oper == "pi"){
			(*p)->isnum = true;//变为数字
			(*p)->num = 3.14159265358979323846;
		}else if((*p)->oper == "tan"){
			p2 = ++p;
			p--;
			if(p2 == link.end() || !(*p2)->isnum) return false;
			num1 = (*p2)->num;
			(*p2)->num = tan(num1);
			delete *p;
			link.erase(p);
		}else if((*p)->oper == "atan"){
			p2 = ++p;
			p--;
			if(p2 == link.end() || !(*p2)->isnum) return false;
			num1 = (*p2)->num;
			(*p2)->num = atan(num1);
			delete *p;
			link.erase(p);
		}else if((*p)->oper == "cos"){
			p2 = ++p;
			p--;
			if(p2 == link.end() || !(*p2)->isnum) return false;
			num1 = (*p2)->num;
			(*p2)->num = cos(num1);
			delete *p;
			link.erase(p);
		}else if((*p)->oper == "acos"){
			p2 = ++p;
			p--;
			if(p2 == link.end() || !(*p2)->isnum) return false;
			num1 = (*p2)->num;
			(*p2)->num = acos(num1);
			delete *p;
			link.erase(p);
		}else if((*p)->oper == "sin"){
			p2 = ++p;
			p--;
			if(p2 == link.end() || !(*p2)->isnum) return false;
			num1 = (*p2)->num;
			(*p2)->num = sin(num1);
			delete *p;
			link.erase(p);
		}else if((*p)->oper == "asin"){
			p2 = ++p;
			p--;
			if(p2 == link.end() || !(*p2)->isnum) return false;
			num1 = (*p2)->num;
			(*p2)->num = asin(num1);
			delete *p;
			link.erase(p);
		}else if((*p)->oper == "ln"){
			p2 = ++p;
			p--;
			if(p2 == link.end() || !(*p2)->isnum) return false;
			num1 = (*p2)->num;
			(*p2)->num = log(num1);
			delete *p;
			link.erase(p);
		}else if((*p)->oper == ")"){
			delete *p;
			link.erase(p);
		}else if((*p)->oper == "("){
			delete *p;
			link.erase(p);
		}else{
			cout << "error!@cal()" << endl;
		}
	}
	return true;
}

/*编译时加上： -std=c++11*/
int main(){
	list<Cell *> link;
	list<Cell *>::iterator iter;
	string str;
	cout << "*****************************************************************" << endl;
	cout << "*   Operator suported: '+' '-' '*' '/' '^' 'tan' 'atan' 'cos'   *" << endl;
	cout << "*                  'acos' 'sin' 'asin' '(' ')'                  *" << endl;
	cout << "*    'e' represent constant number of 2.71828182845904523536    *" << endl;
	cout << "*    'pi'represent constant number of 3.14159265358979323834    *" << endl;
	cout << "*****************************************************************" << endl;
	initPattern();//初始化正则表达式 
	while(cin >> str){
		if(build(link, str)){
			cout << "depart number and operator: ";
			for(iter = link.begin();iter != link.end();iter++){
				if((*iter)->isnum)
					cout << (*iter)->num << ' ';
				else
					cout << (*iter)->oper << ' ';
			}
			cout << endl;
			if(cal(link)){
				cout << "result :";
				for(iter = link.begin();iter != link.end();iter++){
					if((*iter)->isnum)
						cout << (*iter)->num << ' ';
					else
						cout << (*iter)->oper << ' ';
				}
				cout << endl;
			}else
				cout << "input error1!@main" << endl;
		}else
			cout << "input error2!@main" << endl;
	}
	
	return 0;
}


