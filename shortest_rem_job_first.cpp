// #include<iostream> 
// #include<vector>
// #include<algorithm>
// #include<queue>
// #include<set>
// using namespace std; 

// class Process{
//     public: 
//     int pid; 
//     int bt; 
//     int at; 
//     int rt; 
//     int start_time = -1; 
//     int end_time; 
// };

// bool compare(Process& a, Process& b) {
//     if(a.at == b.at){
//         return a.rt < a.rt;  
//     }
//     return a.at < b.at;
// }

// int main(){
//     cout << "Enter Number of Processes: " ; 
//     int n; 
//     cin >> n;
//     cout << endl; 

//     vector<Process> p(n); 
//     for(int i = 0; i < n; ++i){
//         cout << "P" << i+1 << " : " << endl; 
//         cout << "Enter Burst Time : ";
//         cin >> p[i].bt; 
//         p[i].rt = p[i].bt;
//         cout << endl;
//         cout << "Enter Arrival Time : "; 
//         cin >>p[i].at;
//     }

//     sort(p.begin(), p.end(), compare);
//     queue<Process> q; 
//     q.push(p[0]);
//     set<int> arrival_time; 
//     for(auto t : p){
//         arrival_time.insert(t.at);
//     }
//     // init the clock from 0
//     int clock = 0; 
    
//     // final state of the completed processes
//     vector<Process> final; 

//     // The Algorithm
//     while (!q.empty()) {
//         auto process = q.front(); q.pop();
//         bool fin = false;
//         if (process.start_time == -1) process.start_time = clock;

//         for (auto t : p) {
//             if (t.at == clock && t.rt < process.rt) {
//                 q.push(t); 
//                 break; 
//             }
//         }
//         process.rt--;
//         if (!process.rt) {
//             process.end_time = clock;
//             fin = true;
//         }
//         clock++;
//         if (fin) {
//             final.push_back(process);
//             q.pop();
//         } else {
//             q.push(process);
//         }
//     }

//     // Calculation of TAT and WT -> 
//     int total_wt = 0, total_tat = total_wt;
//     for(int i = 0 ; i < final.size(); i++){
//         total_wt += (final[i].end_time - final[i].start_time) - final[i].bt; 
//         total_tat += final[i].end_time - final[i].start_time;
//     }

//     cout << "Average WT : " << (float)total_wt/ (float)final.size(); 
//     cout << "Average TAT : "  << (float)total_tat / (float)final.size();

//     return 0;
// }
 

#include <iostream>
#include <algorithm> 
#include <iomanip>
#include <string.h> 
using namespace std;

struct process {
    int pid;
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
};

int main() {

    int n;
    struct process p[100];
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    float cpu_utilisation;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int total_idle_time = 0;
    float throughput;
    int burst_remaining[100];
    int is_completed[100];
    memset(is_completed,0,sizeof(is_completed));

    cout << setprecision(2) << fixed;

    cout<<"Enter the number of processes: ";
    cin>>n;

    for(int i = 0; i < n; i++) {
        cout<<"Enter arrival time of process "<<i+1<<": ";
        cin>>p[i].arrival_time;
        cout<<"Enter burst time of process "<<i+1<<": ";
        cin>>p[i].burst_time;
        p[i].pid = i+1;
        burst_remaining[i] = p[i].burst_time;
        cout<<endl;
    }

    int current_time = 0;
    int completed = 0;
    int prev = 0;

    while(completed != n) {
        int idx = -1;
        int mn = 10000000;
        for(int i = 0; i < n; i++) {
            if(p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if(burst_remaining[i] < mn) {
                    mn = burst_remaining[i];
                    idx = i;
                }
                if(burst_remaining[i] == mn) {
                    if(p[i].arrival_time < p[idx].arrival_time) {
                        mn = burst_remaining[i];
                        idx = i;
                    }
                }
            }
        }

        if(idx != -1) {
            if(burst_remaining[idx] == p[idx].burst_time) {
                p[idx].start_time = current_time;
                total_idle_time += p[idx].start_time - prev;
            }
            burst_remaining[idx] -= 1;
            current_time++;
            prev = current_time;
            
            if(burst_remaining[idx] == 0) {
                p[idx].completion_time = current_time;
                p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
                p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
                p[idx].response_time = p[idx].start_time - p[idx].arrival_time;

                total_turnaround_time += p[idx].turnaround_time;
                total_waiting_time += p[idx].waiting_time;
                total_response_time += p[idx].response_time;

                is_completed[idx] = 1;
                completed++;
            }
        }
        else {
             current_time++;
        }  
    }

    int min_arrival_time = 10000000;
    int max_completion_time = -1;
    for(int i = 0; i < n; i++) {
        min_arrival_time = min(min_arrival_time,p[i].arrival_time);
        max_completion_time = max(max_completion_time,p[i].completion_time);
    }

    avg_turnaround_time = (float) total_turnaround_time / n;
    avg_waiting_time = (float) total_waiting_time / n;
    avg_response_time = (float) total_response_time / n;
    cpu_utilisation = ((max_completion_time - total_idle_time) / (float) max_completion_time )*100;
    throughput = float(n) / (max_completion_time - min_arrival_time);

    cout<<endl<<endl;

    cout<<"#P\t"<<"AT\t"<<"BT\t"<<"ST\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<"\n"<<endl;

    for(int i = 0; i < n; i++) {
        cout<<p[i].pid<<"\t"<<p[i].arrival_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].start_time<<"\t"<<p[i].completion_time<<"\t"<<p[i].turnaround_time<<"\t"<<p[i].waiting_time<<"\t"<<p[i].response_time<<"\t"<<"\n"<<endl;
    }
    cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
    cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
    cout<<"Average Response Time = "<<avg_response_time<<endl;
    cout<<"CPU Utilization = "<<cpu_utilisation<<"%"<<endl;
    cout<<"Throughput = "<<throughput<<" process/unit time"<<endl;


}
