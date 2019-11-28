#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <omp.h>

using namespace std;

int main(int argc, char* argv[]) {

    vector<string> words = {"first", "second", "break", "this"};
    
    for(int i=1; i<16; i+=i) {
        double t1 = omp_get_wtime();
        #pragma omp parallel for num_threads(i)
        for(int w=0; w<words.size(); w++) {
            ifstream f;
            int count=0;
            f.open("words.txt");
            string word;
            while(f >> word) {
                if(word == words[w])
                    count++;
            }
            cout<<words[w]<<" present "<<count<<" times."<<endl;
        }
        double t2 = omp_get_wtime();
        cout<<"Time taken by "<<i<<" threads is "<<t2-t1<<endl;
    }
    
    return 0;
}