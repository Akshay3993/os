#include <iostream>
#include <cstdlib>
#include <queue>
#include <cstdio>
#include <algorithm>
using namespace std;
typedef struct process {
int id, at, bt, remaining_bt, completion_time, pr;
float wt, tat;
} process;
process p[10], p1[10], temp;
queue<int> q1;
int accept(int ch);
void turnwait(int n);
void display(int n);
void gantt_fcfs(int n);
void sjf_non_preemptive(int n);
void priority_non_preemptive(int n);
void priority_preemptive(int n, int* gantt_chart);
void ganttrr(int n);
void fcfs(int n);
void gantt_sjf(int n);
void gantt_priority_preemptive(int n, int* gantt_chart);
int main() {
int n, ch;
cout << "Choose Scheduling Algorithm: \n";
cout << "1. FCFS\n";
cout << "2. SJF (Non-Preemptive)\n";
cout << "3. Preemptive Priority Scheduling\n";
cout << "4. Non-Preemptive Priority Scheduling\n";
cout << "5. Round Robin\n";
cin >> ch;
n = accept(ch);
switch (ch) {
case 1:
fcfs(n);
break;
case 2:
sjf_non_preemptive(n);
break;
case 3: {
int gantt_chart[100] = {0};
priority_preemptive(n, gantt_chart);
turnwait(n);
display(n);
gantt_priority_preemptive(n, gantt_chart);
break;
}
case 4:
priority_non_preemptive(n);
break;
case 5:
ganttrr(n);
break;
default:
cout << "Invalid choice!" << endl;

exit(1);
}
if (ch != 3) {
turnwait(n);
display(n);
}
return 0;
}
int accept(int ch) {
int i, n;
cout << "Enter the Total Number of Processes: ";
cin >> n;
if (n == 0) {
cout << "Invalid number of processes!" << endl;
exit(1);
}
cout << endl;
for (i = 1; i <= n; i++) {
cout << "Enter the Arrival Time of Process P" << i << ": ";
cin >> p[i].at;
p[i].id = i;
}
cout << endl;
for (i = 1; i <= n; i++) {
cout << "Enter the Burst Time of Process P" << i << ": ";
cin >> p[i].bt;
p[i].remaining_bt = p[i].bt;
}
if (ch == 3 || ch == 4) {
for (i = 1; i <= n; i++) {
cout << "Enter the Priority of Process P" << i << ": ";
cin >> p[i].pr;
}
}
for (i = 1; i <= n; i++) {
p1[i] = p[i];
}
return n;
}
void turnwait(int n) {
int i;
for (i = 1; i <= n; i++) {
p[i].tat = p[i].completion_time - p[i].at;
p[i].wt = p[i].tat - p[i].bt;
p[0].tat = p[0].tat + p[i].tat;
p[0].wt = p[0].wt + p[i].wt;
}
p[0].tat = p[0].tat / n;
p[0].wt = p[0].wt / n;
}
void display(int n) {
int i;
cout << "\n=====================================================\n";
cout << "\n\nHere AT = Arrival Time\nBT = Burst Time\nCT = Completion Time\nTAT = Turn Around Time\nWT = Waiting Time\n";

cout << "\n===================TABLE==============================\n";
cout << "Process\tAT\tBT\tCT\tTAT\t\tWT\n";
for (i = 1; i <= n; i++) {
printf("P%d\t%d\t%d\t%d\t%f\t%f\n", p[i].id, p[i].at, p[i].bt, p[i].completion_time, p[i].tat, p[i].wt);
}
cout << "\n=====================================================\n";
printf("\nAverage Turn Around Time: %f", p[0].tat);

printf("\nAverage Waiting Time: %f\n", p[0].wt);
}
void fcfs(int n) {
int i, current_time = 0;
// Sort processes based on arrival time
sort(p + 1, p + n + 1, [](process a, process b) {
return a.at < b.at;
});
for (i = 1; i <= n; i++) {
// If the current time is less than the arrival time of the process,
// update the current time to the arrival time
if (current_time < p[i].at) {
current_time = p[i].at;
}
// The completion time is the current time plus the burst time
p[i].completion_time = current_time + p[i].bt;
// Update the current time to the completion time
current_time = p[i].completion_time;
}
gantt_fcfs(n);
}
void gantt_fcfs(int n) {
cout << "\nGantt Chart for FCFS Scheduling\n";
for (int i = 1; i <= n; i++) {
cout << "P" << p[i].id << " ";
}
cout << endl;
}
void sjf_non_preemptive(int n) {
int completed = 0, current_time = 0, smallest;
bool is_completed[10] = {false};
while (completed < n) {
smallest = -1;
int min_burst_time = 9999;
for (int i = 1; i <= n; i++) {
if (!is_completed[i] && p[i].at <= current_time && p[i].bt < min_burst_time) {
min_burst_time = p[i].bt;
smallest = i;
}
}
if (smallest != -1) {
current_time += p[smallest].bt;
p[smallest].completion_time = current_time;
is_completed[smallest] = true;
completed++;
} else {
current_time++;
}
}
gantt_sjf(n);
}
void gantt_sjf(int n) {
cout << "\nGantt Chart for SJF Scheduling\n";
for (int i = 1; i <= n; i++) {
cout << "P" << p[i].id << " ";
}
cout << endl;
}

void priority_non_preemptive(int n) {
int completed = 0, current_time = 0, smallest;
bool is_completed[10] = {false};
while (completed < n) {
smallest = -1;
int min_priority = 9999;
for (int i = 1; i <= n; i++) {
if (!is_completed[i] && p[i].at <= current_time && p[i].pr < min_priority) {
min_priority = p[i].pr;
smallest = i;
}
}
if (smallest != -1) {
current_time += p[smallest].bt;
p[smallest].completion_time = current_time;
is_completed[smallest] = true;
completed++;
} else {
current_time++;
}
}
}
void priority_preemptive(int n, int* gantt_chart) {
int completed = 0, current_time = 0;
bool is_completed[10] = {false};
int current_process = -1;
while (completed < n) {
int highest_priority = 9999, next_process = -1;
for (int i = 1; i <= n; i++) {
if (!is_completed[i] && p[i].at <= current_time && p[i].pr < highest_priority && p[i].remaining_bt > 0) {
highest_priority = p[i].pr;
next_process = i;
}
}
if (next_process != -1) {
if (current_process != next_process) {
current_process = next_process;
}
p[current_process].remaining_bt--;
gantt_chart[current_time] = current_process;
current_time++;
if (p[current_process].remaining_bt == 0) {
p[current_process].completion_time = current_time;
is_completed[current_process] = true;
completed++;
current_process = -1;
}
} else {
gantt_chart[current_time] = 0;
current_time++;
}
}
}
void gantt_priority_preemptive(int n, int* gantt_chart) {
cout << "\nGantt Chart for Preemptive Priority Scheduling\n";
for (int i = 0; i < 20; i++) {
if (gantt_chart[i] != 0) {
cout << "P" << gantt_chart[i] << " ";
} else {
cout << "Idle ";
}
}

cout << endl;
}
void ganttrr(int n) {
int i, ts, m, nextval, nextarr;
nextval = p1[1].at;
i = 1;
cout << "\nEnter the Time Slice or Quantum: ";
cin >> ts;
for (i = 1; i <= n && p1[i].at <= nextval; i++) {
q1.push(p1[i].id);
}
while (!q1.empty()) {
m = q1.front();
q1.pop();
if (p1[m].bt >= ts) {
nextval = nextval + ts;
} else {
nextval = nextval + p1[m].bt;
}
if (p1[m].bt >= ts) {
p1[m].bt = p1[m].bt - ts;
} else {
p1[m].bt = 0;
}
while (i <= n && p1[i].at <= nextval) {
q1.push(p1[i].id);
i++;
}
if (p1[m].bt > 0) {
q1.push(m);
}
if (p1[m].bt <= 0) {
p[m].completion_time = nextval;
}
}
}
