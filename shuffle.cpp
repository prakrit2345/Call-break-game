#include<iostream>
#include<string>
#include <cstdlib>
#include <ctime>
using namespace std;
class Deck{
    public:
    int array[52];
    Deck(){
        
    for(int i=0;i<52;i++){
        array[i]=i+1;
    }
    
    for(int i=51;i>=0;i--){
        
        int j=rand()%(i+1);
        swap(array[i],array[j]);
    }
      }

};
int main(){
    srand(time(0));
    Deck s;
    
    
    for(int i=0;i<52;i++){
        cout<<s.array[i]<<" ";
    }
    cout <<endl;
    
    int p1[13],p2[13],p3[13],p4[13];
    int round=1;
    for(int i=0;i<52;i++){
        int k=i%4;
        switch (k)
        {
        case 0:
            p1[round-1]=s.array[i];
            break;
        case 1:
            p2[round-1]=s.array[i];
            break;
        case 2:
            p3[round-1]=s.array[i];
            
            break;
        
        default:
            p4[round-1]=s.array[i];
            round++;
            break;
         }   

        
    

    }
    for(int i=0;i<13;i++){
        cout<<p1[i]<<" ";
    }
    return 0;

}