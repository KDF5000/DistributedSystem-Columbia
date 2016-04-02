#include<iostream>
#include<map>
using namespace std;

int main(){
    map<string,string> myMap;
    myMap.insert(make_pair("a","b"));
    myMap.insert(make_pair("b","badadada"));
    myMap.insert(make_pair("d","badadada"));
    myMap.insert(make_pair("c","badadada"));

    map<string,string>::iterator iter = myMap.begin();

    cout<< iter->first<<endl;

    myMap.erase("a");

    cout<< myMap.size()<<endl;
    return 0;
}
