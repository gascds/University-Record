#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

int main() {
    string input;
    vector <int> position;
    vector<int> store;
    while(cin>>input) {
        for(int i=0;i<input.size();i++){
            if (input[i] == '(') {
                store.push_back(i);
            }else {
                position.push_back((i-store.back())/2+1);
                store.pop_back();
            }
        }
        int result = 0;
        for(int j=0;j<input.size()/2;j++){
            result = result^position[j];
        }
        cout<<result<<endl;
        position.clear();
        store.clear();
    }
    return 0;
}
