#include <sstream>
#include<stdlib.h>
#include <math.h>
#include <complex>
#include <complex.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include <stack>
#include <iomanip>
#include <cstdlib>
#include <typeinfo>

using namespace std;

int priority(char this_opt){
    switch(this_opt){
        case '(':
        case ')':
            return 1;
        case '+':
        case '-':
            return 2;
        case '*':
        case '/':
            return 3;
        case '^':
            return 4;         
        default:
            return 0;
    }
}

double cal_nor(string this_expr){
    stack<double> stk_num;
    int i=0;
    int j=0;
    string current;

    while(this_expr[i]!='#'){
        bool if_num=false;
        if(this_expr[i]=='?'){
            for(j=i+1;; j++) if((this_expr[j]=='?')||(this_expr[j]=='#')) break;

            //获取"?a?"之间元素
            for(int k=i+1; k<j; k++) current+=this_expr[k];

            //判断是否为数值
            for(int k=0; k<current.size(); k++){
		    //数字
                if(current[k]>=48 && current[k]<=57){
			//strinf转double
                    istringstream iss(current);
                    double num;
                    iss >> num;
                    stk_num.push(num);
                    if_num=true;
                    break;
                }
            }
            //这段有待商榷
            // if_num=if_legal(current);
            // if(if_num) stk_num.push(cvt_num(current));

            if(!if_num){
                double num2 = stk_num.top();
                stk_num.pop();
                double num1 = stk_num.top();
                stk_num.pop();

                if(current=="+") stk_num.push(num1+num2);
                else if(current=="-") stk_num.push(num1-num2);
                else if(current=="*") stk_num.push(num1*num2);
                else if(current=="/") stk_num.push(num1/num2);
                else if(current=="^") stk_num.push(pow(num1, num2));   
            }
		//清空当前字符串;
            current="";
        }
        i++;
    }
    if(stk_num.size()!=1) cout << "error" << endl;
    else return stk_num.top();
    return 0;
}

string cvt_expr(string before){
    stack<char> stk_opt;
	//初始化后缀表达式
    string after = "";
    char this_opt;
    bool if_negative=false;
	//i为中缀当前指向 
    int i=0;
	//j为后缀当前指向
    int j = 0;
    while(before[i]!='\0'){
        if(i+1!='\0') if_negative=false;
	    //判断数字      
        if(before[i]>=48 && before[i]<=57){ 
		//j是后缀表达索引
            after[j++] = '?';
		//存储当前数字并指向下一个
            after[j++] =before[i];
		//整数部分
            while(before[++i]>=48 && before[i]<=57) after[j++] =before[i];
		//是小数
            if(before[i]=='.'){ 
                after[j++]='.';
		    //中缀索引 往后移
                i+=1;
		    //小数部分
                while(before[i]>=48 && before[i]<=57){ 
                    after[j++] =before[i];
                    i+=1;
                }
            }
        }
	    //如果读入(，因为左括号优先级最高，因此放入栈中，但是注意，当左括号放入栈中后，则优先级最低
        else if(before[i]=='(') stk_opt.push(before[i++]);
	    //如果读入），则将栈中运算符取出放入输出字符串，直到取出（为止
        else if(before[i]==')'){   
		//没有左括号
            if(stk_opt.empty()) cout<< "error" <<endl;
            else{
                this_opt = stk_opt.top();
                while(this_opt!='('){
                    after[j++]='?';
                    after[j++]=this_opt;
                    stk_opt.pop();
                    if(stk_opt.empty()){
                        cout << "error" << endl;
                        break;
                    }
                    this_opt = stk_opt.top();
                }
		    //删除栈中(
                stk_opt.pop();
                i++;
            }
        }

        else if(before[i]=='+'||before[i]=='-'||before[i]=='/'||before[i]=='*'){
            //判断负数
            if(before[i]=='-'){
		    //第一个为‘-’时为负号
                if(i==0) if_negative=true;
		    //如果前面有操作符则为负号
                else if(before[i-1]=='+'||before[i-1]=='-'||before[i-1]=='/'||before[i-1]=='*') if_negative=true;
                if(if_negative){
			//负号
                    after[j++] = '?';
                    after[j++] = '-';
                    i++;
			//负号
                    if(before[i]>=48 && before[i]<=57){ x                    
                        after[j++] =before[i];
						       //整数部分
                        while(before[++i]>=48 && before[i]<=57) after[j++] =before[i];
						       //是小数
                        if(before[i]=='.'){
                            after[j++]='.';
                            i++;
                            while(before[i]>=48 && before[i]<=57){ //小数部分
                                after[j++] =before[i];
                                i++;
                            }
                        }
                    }
                    continue;
                }
            }

            //如果读入一般运算符如+-*/，则放入堆栈，但是放入堆栈之前必须要检查栈顶
            if(stk_opt.empty()) stk_opt.push(before[i++]);         
            else{
                char top = stk_opt.top();//栈顶
                if(priority(top)<priority(before[i])) stk_opt.push(before[i++]);//放入的符号优先级低于栈顶,放入栈顶并指向下一个
                else{//如果放入的优先级较低，则需要将栈顶的运算符放入输出字符串。
                    while(priority(top)>=priority(before[i])){
                        after[j++]='?';
                        after[j++]=top;
                        stk_opt.pop();
                        if(!stk_opt.empty()) top = stk_opt.top();
                        else break;
                    }
                    stk_opt.push(before[i++]);//放入栈顶并指向下一个
                }
            }
        }
        else{
            cout << "error" << endl;
            i++;
        }
    }

    //顺序读完表达式，如果栈中还有操作符，则弹出，并放入输出字符串。
    while(!stk_opt.empty()){
        char to = stk_opt.top();
        after[j++]='?';
        after[j++]=to;
        stk_opt.pop();
    }
    after[j] = '#';//结束符
    return after;
}

double cvt_num(string this_num){
    return stod(this_num);
}

bool if_sqrt(string& input){
	string str_sqrt = "sqrt";

	const char* show;

    show = strstr(input.c_str(), str_sqrt.c_str());
    
	if (show != NULL) return true;
	
	return false;
}

string cal_sqrt(string& input){
    bool if_neg=false;
	int number = 0;
    string result="";

	for (int i = 0; input[i] != ')'; i++){
        if(input[i]=='-') if_neg=true;
           
		if (isdigit(input[i])){
			number *= 10;
			number += input[i]-'0';
		}
	}

    if(if_neg){
        result = std::to_string(sqrt(number))+"i";
    }else{
        result=std::to_string(sqrt(number));
    }
	return result;
}

int if_rec(string& input){
	string str_sin = "sin";
	string str_cos = "cos";
	string str_tan = "tan";
	string str_arcsin = "arcsin";
	string str_arccos = "arccos";
	string str_arctan = "arctan";

	const char* show1, *show2, *show3, *show4, *show5, *show6;

    show1 = strstr(input.c_str(), str_sin.c_str());
	if (show1 != NULL) return 1;
	show2 = strstr(input.c_str(), str_cos.c_str());
	if (show2 != NULL) return 2;
	show3 = strstr(input.c_str(), str_tan.c_str());
	if (show3 != NULL) return 3;
	show4 = strstr(input.c_str(), str_arcsin.c_str());
	if (show4 != NULL) return 4;
	show5 = strstr(input.c_str(), str_arccos.c_str());
	if (show5 != NULL) return 5;
	show6 = strstr(input.c_str(), str_arctan.c_str());
	if (show6 != NULL) return 6;	

	return 0;
}

string cal_rec(string& input, int rec_kind){
    bool if_neg=false;
	int number = 0;
    double res_db = 0;
    double res_abs=0;
    bool raw_neg=false;
    string result="";
    
	for (int i = 0; input[i] != ')'; i++){
        if(input[i]=='-') if_neg=true;
        
		if (isdigit(input[i])){
			number *= 10;
			number += input[i]-'0';
		}
	}

	switch (rec_kind){
	case 1:res_db = sin(number); break;
	case 2:res_db = cos(number); break;
	case 3:res_db = tan(number); break;
	case 4:res_db = asin(number); break;
	case 5:res_db = acos(number); break;
	case 6:res_db = atan(number); break;
	default:return 0;break;
	}

    res_abs=abs(res_db);

    if(res_db<0) raw_neg=true;

    if((rec_kind==2)||(rec_kind==5)) result = std::to_string(res_db);
    else{
        if(raw_neg==if_neg) result = std::to_string(res_abs);
        else result=std::to_string(res_db);
    }
    
	return result;
}

bool if_fac(string& input){
	int len = input.length();
	for (int i = 0; i < len; i++) if (input[i] == '!') return true;
	return false;
}

int which_fac(string& input){
	int result = 0;
	for (int i = 0; input[i] != '!'; i++){
		if (isdigit(input[i])) {
			result *= 10;
			result += input[i]-'0';
		}
	}
	return result;
}

int cal_fac(int this_num){
	arr_fac[0] = 1;
	arr_fac[1] = 1;
	int digit = 1;
	for (int i = 1; i <= this_num; i++){
		for (int j = 1; j <= digit; j++) arr_fac[j] *= i;	
		for (int j = 1; j <= digit; j++){		
			if (arr_fac[j] > 10){			
				for (int k = 1; k <= digit; k++){
					if (arr_fac[digit] > 9)
						digit++;
						arr_fac[k + 1] += arr_fac[k] / 10;
						arr_fac[k] %= 10;
				}
			}
		}
	}
	return digit;
}

bool if_legal(string input){
    //分割出每个字符
    char test_char [input.length()];
    strcpy(test_char, input.c_str());

    //判断是否为负数，如果是，检查从第二位开始
    int start_from=0;
    if(test_char[0]=='-') start_from=1;
    else start_from=0;   

    bool expect_dot=true;//引入判断是否为小数点 
    bool expect_e=true;//引入判断是否为e
    bool expect_neg=false;//引入判断是否为负号，默认false

    //判断每一位是否为数字、小数点、e
    for(int i=start_from;i<sizeof(test_char);i++){
        if(isdigit(test_char[i])) continue;//判断数字
        else if((test_char[i]=='.')&&(expect_dot)) expect_dot=false;//判断小数点          
        else if((test_char[i]=='e')&&(expect_e)){//判断科学计数法
            expect_dot=false;
            expect_e=false;

            //判断e之后是否为负号
            if((test_char[i+1]!='-')) expect_neg=false;
            else expect_neg=true;
        }else if((test_char[i]=='-')&&(expect_neg)) expect_neg=false;//判断科学计数负数
        else return true;
    }
    return false;
}
