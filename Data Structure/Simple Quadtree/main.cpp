#include <iostream>
#include <math.h>
using namespace std;

int input[1025][1025] = {};

int quadtree(int N, int x, int y){
    int check = input[x][y];
    for (int i=x;i<x+N;i++){
        for (int j=y;j<y+N;j++){
            if (input[i][j]!=check){
                return (quadtree(N/2,x,y) + quadtree(N/2,x+N/2,y) + quadtree(N/2,x,y+N/2) + quadtree(N/2,x+N/2,y+N/2) +1);
            }
        }
    }
    return 1;
}

int main(){
    char CoinInfo[1025];
    int k,n;
    while (cin>>k && !cin.eof()){
        n = pow(2,k);
        for (int i=0;i<n;i++){
            cin.ignore(1);
            cin.get(CoinInfo,n+1);
            for (int j=0;j<n;j++){
                input[i][j] = int (CoinInfo[j] - '0');
            }
            
        }
        cout<<quadtree(n,0,0)<<endl;
    }
    return 0;
}
