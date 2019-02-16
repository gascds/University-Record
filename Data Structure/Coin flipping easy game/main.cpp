#include <iostream>
#include <string>
#include <math.h>
#include <string.h>
#include <bitset>
using namespace std;

int copy_arr[101][11] = {};
int input[101][11] = {};
string status[1025] = {};

void flipping_case(int m){
    string y ={};
    for (int i=0;i<pow(2,m);i++){
        y = bitset<10>(i).to_string();
        for (int j=(10-m);j<y.length();j++){
            status[i] = status[i] + y[j];
        }
    }
}

void copyarray(int n,int m){
    for (int i=0;i<n;i++){
        for (int j=0;j<m;j++){
            copy_arr[i][j] = input[i][j];
        }
    }
}

int main() {
    int n,m,resultOne, max, CountOne, CountZero;
    char CoinInfo[10];
    while (cin>>n>>m && !cin.eof()){
        max = 0;
        flipping_case(m);
        for (int i=0;i<n;i++){
            scanf("%s", CoinInfo);
            for (int j=0;j<m;j++){
                input[i][j] = int (CoinInfo[j] - '0');
            }
        }
        for (int z=0;z<pow(2,m);z++){ //no of combination
            copyarray(n,m); //create new array
            char current[status[z].length()+1];
            strcpy(current, status[z].c_str()); //decode the string of each combination
            
            for (int j=0;j<m;j++){ // flip col
                if (current[j]=='1'){
                    for (int k=0;k<n;k++){
                        if (copy_arr[k][j] == 1)
                            copy_arr[k][j] = 0;
                        else
                            copy_arr[k][j] = 1;
                    }
                }
            }
            
            //flipping row
            resultOne = 0;
            for (int i = 0; i<n;i++){
                CountOne = 0; CountZero = 0;
                for (int j=0;j<m;j++){
                    if (copy_arr[i][j]== 1)
                        CountOne ++ ;
                    else
                        CountZero ++;
                }
                if (CountZero>=CountOne){
                    resultOne+=CountZero;
                    for (int k=0;k<m;k++){
                        if (copy_arr[i][k] == 1)
                            copy_arr[i][k] = 0;
                        else
                            copy_arr[i][k] = 1;
                    }
                }else {
                    resultOne+=CountOne;
                }
            }
            
            if (resultOne>max) max = resultOne;
            if (max == n*m) break;
        }
        printf("%d\n",max);
    }
    return 0;
}
