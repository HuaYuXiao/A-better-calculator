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

//操作符的优先级
int priority(char this_opt);									
string cvt_expr(string before);
double cvt_num(string this_num);
//判断输入类型：false为非法，true为合法
bool if_legal(string input);
//判断是否为开方函数
bool if_sqrt(string& input);
//计算开方函数
string cal_sqrt(string& input);
//判断是否为三角函数或反三角函数并返回对应的编号
int if_rec(string& input);
/*求三角函数里包含的数字并求出对应的结果*/
string cal_rec(string& input, int rec_kind);
/*判断是否为求阶乘*/
bool if_fac(string& input);
// 求阶乘所求的数字
int which_fac(string& theStr);
//利用数组求阶乘
int cal_fac(int this_num);	
//计算后缀表达式
double cal_nor(string this_expr);							
//求阶乘所用数组
int arr_fac[1000] = {0};

int main(){
    string input;
    cout << "input:" << endl;

    cin >> input;
    
    cout << "result:" << endl;
    
    if(if_sqrt(input)) cout << cal_sqrt(input) << endl; 
    //判断是否三角函数和反三角函数并求值
    else if(if_rec(input) != 0) {		
        string result ="";
        result = cal_rec(input, if_rec(input));
        cout << result << endl;           
    }
//求阶乘的情况
    else if (if_fac(input)) {
        int n = which_fac(input);
        for (int i = cal_fac(n); i > 0; i--) cout << arr_fac[i];
    }

    else cout << cal_nor(cvt_expr(input)) << endl;
    
    return 0;
}
