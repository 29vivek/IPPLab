#include <iostream>
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

struct Point {
    int x;
    int y;
    Point(int a, int b): x(a), y(b) {}
};

void display(vector<Point>& p) {
    for(int i=0; i<p.size(); i++) {
        cout<<"("<<p[i].x<<","<<p[i].y<<"), ";
    }
    cout<<endl;
}

int main(int argc, char* argv[]) {

    int range = 100;
    int num = atoi(argv[1]);
    int threads = atoi(argv[2]);
    srand(time(NULL));

    vector<Point> points;
    vector<Point> cluster;

    for(int i=0; i<4; i++) {
        cluster.push_back({20*i, 20*i});
    }
    vector<int> result(cluster.size(), 0);

    for(int i=0; i<num; i++) {
        points.push_back({i, i});
    }

    double t1 = omp_get_wtime();
    #pragma omp parallel for num_threads(threads) shared(result)
    for(int i=0; i<num; i++) {
        double minDist = 10000.0;
        int index = -1;
        #pragma omp critical
        for(int j=0; j<4; j++) {
            double dx = points[i].x - cluster[j].x;
            double dy = points[i].y - cluster[j].y;
            double dist = sqrt(dx*dx + dy*dy);
            
            if(dist < minDist) {
                index = j;
                minDist = dist;
            }
        }
        result[index]++;
    }
    double t2 = omp_get_wtime();
    
    cout<<"Points: "<<endl;
    display(points);

    cout<<"Cluster: "<<endl;
    display(cluster);

    for(int i=0; i<result.size(); i++) {
        cout<<result[i]<<" ";
    }
    cout<<endl;

    cout<<num<<" points took "<<t2-t1<<" seconds to be classified."<<endl;

    return 0;
}
