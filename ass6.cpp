#include <iostream>
#include <iomanip>
using namespace std;
#define MAX 100
void firstFit(int blockSize[], int m, int processSize[], int n);
void bestFit(int blockSize[], int m, int processSize[], int n);
void worstFit(int blockSize[], int m, int processSize[], int n);
void nextFit(int blockSize[], int m, int processSize[], int n);
int main() {
int m, n, choice;
int blockSize[MAX], processSize[MAX], blockCopy[MAX];
cout << "Enter the number of memory blocks: ";
cin >> m;
cout << "Enter sizes of " << m << " memory blocks: ";
for (int i = 0; i < m; i++)
cin >> blockSize[i];
cout << "Enter the number of processes: ";
cin >> n;
cout << "Enter sizes of " << n << " processes: ";
for (int i = 0; i < n; i++)
cin >> processSize[i];
do {
for (int i = 0; i < m; i++)
blockCopy[i] = blockSize[i]; // Reset block sizes
cout << "\nChoose Memory Allocation Strategy:\n";
cout << "1. First Fit\n";
cout << "2. Best Fit\n";
cout << "3. Worst Fit\n";
cout << "4. Next Fit\n";
cout << "5. Exit\n";
cout << "Enter your choice: ";
cin >> choice;
switch (choice) {

case 1:
firstFit(blockCopy, m, processSize, n);
break;
case 2:
bestFit(blockCopy, m, processSize, n);
break;
case 3:
worstFit(blockCopy, m, processSize, n);
break;
case 4:
nextFit(blockCopy, m, processSize, n);
break;
case 5:
cout << "Exiting program.\n";
break;
default:
cout << "Invalid choice! Please enter again.\n";
}
} while (choice != 5);
return 0;
}
// Display Allocation and Fragmentation Table
void displayAllocation(int processSize[], int allocation[], int blockSize[], int n, int m) {
cout << "\nProcess No.\tProcess Size\tBlock No.\tFragmented Memory\n";
for (int i = 0; i < n; i++) {
if (allocation[i] == -1)
cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\tNot Allocated\t-" << endl;
else
cout << " " << i + 1 << "\t\t\t\t" << processSize[i] << "\t\t\t\t" << allocation[i]
<< "\t\t\t\t" << blockSize[allocation[i] - 1] << endl;
}
}
// First Fit
void firstFit(int blockSize[], int m, int processSize[], int n) {
int allocation[n];
fill_n(allocation, n, -1);
for (int i = 0; i < n; i++) {
for (int j = 0; j < m; j++) {
if (blockSize[j] >= processSize[i]) {
allocation[i] = j + 1;
blockSize[j] -= processSize[i];
break;

}
}
}
displayAllocation(processSize, allocation, blockSize, n, m);
}
// Best Fit
void bestFit(int blockSize[], int m, int processSize[], int n) {
int allocation[n];
fill_n(allocation, n, -1);
for (int i = 0; i < n; i++) {
int bestIdx = -1;
for (int j = 0; j < m; j++) {
if (blockSize[j] >= processSize[i] && (bestIdx == -1 || blockSize[j] <
blockSize[bestIdx])) {
bestIdx = j;
}
}
if (bestIdx != -1) {
allocation[i] = bestIdx + 1;
blockSize[bestIdx] -= processSize[i];
}
}
displayAllocation(processSize, allocation, blockSize, n, m);
}
// Worst Fit
void worstFit(int blockSize[], int m, int processSize[], int n) {
int allocation[n];
fill_n(allocation, n, -1);
for (int i = 0; i < n; i++) {
int worstIdx = -1;
for (int j = 0; j < m; j++) {
if (blockSize[j] >= processSize[i] && (worstIdx == -1 || blockSize[j] >
blockSize[worstIdx])) {
worstIdx = j;
}
}
if (worstIdx != -1) {
allocation[i] = worstIdx + 1;
blockSize[worstIdx] -= processSize[i];
}
}
displayAllocation(processSize, allocation, blockSize, n, m);

}
// Next Fit
void nextFit(int blockSize[], int m, int processSize[], int n) {
int allocation[n];
fill_n(allocation, n, -1);
int lastAllocated = 0;
for (int i = 0; i < n; i++) {
int j = lastAllocated;
do {
if (blockSize[j] >= processSize[i]) {
allocation[i] = j + 1;
blockSize[j] -= processSize[i];
lastAllocated = j;
break;
}
j = (j + 1) % m;
} while (j != lastAllocated);
}
displayAllocation(processSize, allocation, blockSize, n, m);
}
