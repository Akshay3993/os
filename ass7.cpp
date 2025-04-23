#include <bits/stdc++.h>
using namespace std;
// Function to display the step-by-step execution
void displayStep(int step, int page, vector<int> frame, bool page_fault) {
cout << setw(10) << step << setw(15) << page << " ";
for (int f : frame) cout << f << " ";
for (int i = frame.size(); i < 3; i++) cout << "- "; // Fill empty slots
cout << setw(10) << (page_fault ? "YES" : "NO") << endl;
}
// Function to find page faults using FCFS
int FCFS(int pages[], int n, int capacity) {
unordered_set<int> s;
queue<int> indexes;
int page_faults = 0;
vector<int> frame;
cout << "\nFCFS Step-by-Step Execution:\n";
cout << "-----------------------------------------\n";
cout << setw(10) << "Step" << setw(15) << "Page" << setw(10) << "Frames" << setw(10) <<
"Fault\n";
cout << "-----------------------------------------\n";
for (int i = 0; i < n; i++) {
bool page_fault = false;
if (s.size() < capacity) {
if (s.find(pages[i]) == s.end()) {
s.insert(pages[i]);
indexes.push(pages[i]);
frame.push_back(pages[i]);
page_fault = true;
page_faults++;
}
} else {
if (s.find(pages[i]) == s.end()) {
int val = indexes.front();
indexes.pop();
s.erase(val);
s.insert(pages[i]);
indexes.push(pages[i]);
replace(frame.begin(), frame.end(), val, pages[i]);
page_fault = true;
page_faults++;
}
}
displayStep(i + 1, pages[i], frame, page_fault);

}
return page_faults;
}
// Function to find page faults using LRU
int LRU(int pages[], int n, int capacity) {
unordered_set<int> s;
unordered_map<int, int> indices;
int page_faults = 0;
vector<int> frame;
cout << "\nLRU Step-by-Step Execution:\n";
cout << "-----------------------------------------\n";
cout << setw(10) << "Step" << setw(15) << "Page" << setw(10) << "Frames" << setw(10) <<
"Fault\n";
cout << "-----------------------------------------\n";
for (int i = 0; i < n; i++) {
bool page_fault = false;
if (s.size() < capacity) {
if (s.find(pages[i]) == s.end()) {
s.insert(pages[i]);
frame.push_back(pages[i]);
page_fault = true;
page_faults++;
}
} else {
if (s.find(pages[i]) == s.end()) {
int lru = INT_MAX, val;
for (auto it = s.begin(); it != s.end(); ++it) {
if (indices[*it] < lru) {
lru = indices[*it];
val = *it;
}
}
s.erase(val);
s.insert(pages[i]);
replace(frame.begin(), frame.end(), val, pages[i]);
page_fault = true;
page_faults++;
}
}
indices[pages[i]] = i;
displayStep(i + 1, pages[i], frame, page_fault);
}
return page_faults;
}
// Function to find page faults using Optimal
int Optimal(int pages[], int n, int capacity) {
unordered_set<int> s;

int page_faults = 0;
vector<int> frame;
cout << "\nOptimal Step-by-Step Execution:\n";
cout << "-----------------------------------------\n";
cout << setw(10) << "Step" << setw(15) << "Page" << setw(10) << "Frames" << setw(10) <<
"Fault\n";
cout << "-----------------------------------------\n";
for (int i = 0; i < n; i++) {
bool page_fault = false;
if (s.size() < capacity) {
if (s.find(pages[i]) == s.end()) {
s.insert(pages[i]);
frame.push_back(pages[i]);
page_fault = true;
page_faults++;
}
} else {
if (s.find(pages[i]) == s.end()) {
int farthest = -1, val;
for (auto it = s.begin(); it != s.end(); ++it) {
int j;
for (j = i + 1; j < n; j++) {
if (*it == pages[j]) break;
}
if (j == n) {
val = *it;
break;
}
if (j > farthest) {
farthest = j;
val = *it;
}
}
s.erase(val);
s.insert(pages[i]);
replace(frame.begin(), frame.end(), val, pages[i]);
page_fault = true;
page_faults++;
}
}
displayStep(i + 1, pages[i], frame, page_fault);
}
return page_faults;
}
// Main function with switch case
int main() {
int n, capacity, choice;

cout << "Enter number of pages: ";
cin >> n;
int pages[n];
cout << "Enter the page reference string: ";
for (int i = 0; i < n; i++) {
cin >> pages[i];
}
cout << "Enter the page frame capacity: ";
cin >> capacity;
int faults;
do {
cout << "\nChoose Page Replacement Algorithm:\n";
cout << "1. FCFS (First Come First Serve)\n";
cout << "2. LRU (Least Recently Used)\n";
cout << "3. Optimal Page Replacement\n";
cout << "4. Exit\n";
cout << "Enter your choice: ";
cin >> choice;
switch (choice) {
case 1:
faults = FCFS(pages, n, capacity);
cout << "\nTotal Page Faults using FCFS: " << faults << endl;
break;
case 2:
faults = LRU(pages, n, capacity);
cout << "\nTotal Page Faults using LRU: " << faults << endl;
break;
case 3:
faults = Optimal(pages, n, capacity);
cout << "\nTotal Page Faults using Optimal: " << faults << endl;
break;
case 4:
cout << "Exiting Program!\n";
break;
default:
cout << "Invalid choice, please try again.\n";
}
} while (choice != 4);
return 0;
}
