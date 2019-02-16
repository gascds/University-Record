#include<iostream>
#include<queue>
#include <algorithm>
using namespace std;

priority_queue<int,vector<int>,greater<int> > store;

int main()
{
    int n,temp,j,k;
    while(cin>>n){
        for(int i=0;i<n;i++){
            cin>>temp;
            store.push(temp);
        }
        int result = 0;
        while(store.size()>1){
            j = store.top();
            store.pop();
            k = store.top();
            store.pop();
            result+=j+k;
            store.push(j+k);
        }
        cout<<result<<endl;
        store = priority_queue<int,vector<int>,greater<int> >();
    }
    return 0;
}
