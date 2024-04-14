#include<iostream> 
#include<vector>
#include<algorithm>
using namespace std; 

bool compare(vector<int> &a, vector<int> &b){
    return a[1] < b[1];
}

int main(){
    int n; 
    cout << "Enter number of processes : ";
    cin >> n; 
    cout << endl;
    vector<vector<int>> process(n, vector<int>(4));
    cout << "Enter Burst Time : \n";
    for(int i = 0; i < n; i ++){
        cout << "P" << i+1 << " :"; 
        cin >> process[i][1];
        process[i][0] = i+1; 
    }
    sort(process.begin(), process.end(), compare);

    int total = 0; 
    process[0][2] = 0; // first process doesnt wait
    for(int i = 1; i < n; i++){
        process[i][2] = 0; 
        for(int j = 0; j < i; j++){
            process[i][2] += process[j][1];
        }
        total+= process[i][2];
    }
    float avg_wait_time = (float)total / (float)n;
    total = 0; 
    process[0][3] = process[0][1]; // TAT = BT for P1
    total += process[0][3];
    for(int i = 1; i < n; i++){
        process[i][3] = process[i][2] + process[i][1]; 
        total += process[i][3]; 
    }
    for(int i = 0; i < n; i++){
        cout << "P" << process[i][0] << " " << process[i][1] << " " << process[i][2] << " " << process[i][3] << endl;
    }
    float avg_tat = (float)total / (float)n; 
    cout << "Averag Waiting Time : " << avg_wait_time << endl;
    cout << "Average Turn Around Time : " << avg_tat << endl;
    return 0; 

}