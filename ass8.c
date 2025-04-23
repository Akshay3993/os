#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
void fifo(int requests[], int n, int head) {
int total_movement = 0;
int current_position = head;
printf("\nFIFO Disk Scheduling :- \n");
printf("Initial Head Position: %d\n", head);
printf("Request Sequence: ");
for (int i = 0; i < n; i++) {
printf("%d ", requests[i]);
}
printf("\n\n");
printf("Head Movement:\n");
for (int i = 0; i < n; i++) {
int distance = abs(requests[i] - current_position);
total_movement += distance;
printf("Move from %d -> %d (Distance: %d)\n",
current_position, requests[i], distance);
current_position = requests[i];
}
printf("\nTotal Head Movement: %d\n", total_movement);
printf("Average Seek Length: %.2f\n\n", (float)total_movement / n);
}
void sstf(int requests[], int n, int head) {
int total_movement = 0;
int current_position = head;
int serviced[n];
for (int i = 0; i < n; i++) serviced[i] = 0;
printf("\nSSTF Disk Scheduling\n");
printf("Initial Head Position: %d\n", head);

printf("Request Sequence: ");
for (int i = 0; i < n; i++) {
printf("%d ", requests[i]);
}
printf("\n\n");
printf("Head Movement:\n");
for (int count = 0; count < n; count++) {
int min_distance = INT_MAX;
int min_index = -1;
for (int i = 0; i < n; i++) {
if (!serviced[i]) {
int distance = abs(requests[i] - current_position);
if (distance < min_distance) {
min_distance = distance;
min_index = i;
}
}
}
if (min_index == -1) break;
serviced[min_index] = 1;
total_movement += min_distance;
printf("Move from %d -> %d (Distance: %d)\n",
current_position, requests[min_index], min_distance);
current_position = requests[min_index];
}
printf("\nTotal Head Movement: %d\n", total_movement);
printf("Average Seek Length: %.2f\n\n", (float)total_movement / n);
}
void scan(int requests[], int n, int head, int disk_size, int direction) {
int total_movement = 0;
int current_position = head;
int sorted_requests[n];
// Create a copy of requests and sort them
for (int i = 0; i < n; i++) sorted_requests[i] = requests[i];
for (int i = 0; i < n-1; i++) {

for (int j = 0; j < n-i-1; j++) {
if (sorted_requests[j] > sorted_requests[j+1]) {
int temp = sorted_requests[j];
sorted_requests[j] = sorted_requests[j+1];
sorted_requests[j+1] = temp;
}
}
}
printf("\nSCAN Disk Scheduling\n");
printf("Initial Head Position: %d\n", head);
printf("Direction: %s\n", direction ? "Right" : "Left");
printf("Request Sequence: ");
for (int i = 0; i < n; i++) {
printf("%d ", requests[i]);
}
printf("\n\n");
printf("Head Movement:\n");
if (direction) { // Moving right
// Service requests in the right direction
for (int i = 0; i < n; i++) {
if (sorted_requests[i] >= current_position) {
int distance = sorted_requests[i] - current_position;
total_movement += distance;
printf("Move from %d -> %d (Distance: %d)\n",
current_position, sorted_requests[i], distance);
current_position = sorted_requests[i];
}
}
// Go to the end of disk
int distance = disk_size - 1 - current_position;
total_movement += distance;
printf("Move from %d -> %d (Distance: %d)\n",
current_position, disk_size - 1, distance);
current_position = disk_size - 1;
// Service requests in the reverse direction
for (int i = n-1; i >= 0; i--) {
if (sorted_requests[i] < head) {
distance = current_position - sorted_requests[i];
total_movement += distance;

printf("Move from %d -> %d (Distance: %d)\n",
current_position, sorted_requests[i], distance);
current_position = sorted_requests[i];
}
}
} else { // Moving left
// Service requests in the left direction
for (int i = n-1; i >= 0; i--) {
if (sorted_requests[i] <= current_position) {
int distance = current_position - sorted_requests[i];
total_movement += distance;
printf("Move from %d -> %d (Distance: %d)\n",
current_position, sorted_requests[i], distance);
current_position = sorted_requests[i];
}
}
// Go to the start of disk
int distance = current_position;
total_movement += distance;
printf("Move from %d -> 0 (Distance: %d)\n", current_position, distance);
current_position = 0;
// Service requests in the reverse direction
for (int i = 0; i < n; i++) {
if (sorted_requests[i] > head) {
distance = sorted_requests[i] - current_position;
total_movement += distance;
printf("Move from %d -> %d (Distance: %d)\n",
current_position, sorted_requests[i], distance);
current_position = sorted_requests[i];
}
}
}
printf("\nTotal Head Movement: %d\n", total_movement);
printf("Average Seek Length: %.2f\n\n", (float)total_movement / n);
}
void cscan(int requests[], int n, int head, int disk_size, int direction) {
int total_movement = 0;
int current_position = head;
int sorted_requests[n];

// Create a copy of requests and sort them
for (int i = 0; i < n; i++) sorted_requests[i] = requests[i];
for (int i = 0; i < n-1; i++) {
for (int j = 0; j < n-i-1; j++) {
if (sorted_requests[j] > sorted_requests[j+1]) {
int temp = sorted_requests[j];
sorted_requests[j] = sorted_requests[j+1];
sorted_requests[j+1] = temp;
}
}
}
printf("\nC-SCAN Disk Scheduling\n");
printf("Initial Head Position: %d\n", head);
printf("Direction: %s\n", direction ? "Right" : "Left");
printf("Request Sequence: ");
for (int i = 0; i < n; i++) {
printf("%d ", requests[i]);
}
printf("\n\n");
printf("Head Movement:\n");
if (direction) { // Moving right
// Service requests in the right direction
for (int i = 0; i < n; i++) {
if (sorted_requests[i] >= current_position) {
int distance = sorted_requests[i] - current_position;
total_movement += distance;
printf("Move from %d -> %d (Distance: %d)\n",
current_position, sorted_requests[i], distance);
current_position = sorted_requests[i];
}
}
// Go to the end of disk and then jump to start
int distance = disk_size - 1 - current_position;
total_movement += distance;
printf("Move from %d -> %d (Distance: %d)\n",
current_position, disk_size - 1, distance);
printf("Jump from %d -> 0 (Distance: %d)\n", disk_size - 1, disk_size - 1);
total_movement += disk_size - 1;
current_position = 0;

// Service requests from the start again
for (int i = 0; i < n; i++) {
if (sorted_requests[i] < head) {
distance = sorted_requests[i] - current_position;
total_movement += distance;
printf("Move from %d -> %d (Distance: %d)\n",
current_position, sorted_requests[i], distance);
current_position = sorted_requests[i];
}
}
} else { // Moving left
// Service requests in the left direction
for (int i = n-1; i >= 0; i--) {
if (sorted_requests[i] <= current_position) {
int distance = current_position - sorted_requests[i];
total_movement += distance;
printf("Move from %d -> %d (Distance: %d)\n",
current_position, sorted_requests[i], distance);
current_position = sorted_requests[i];
}
}
// Go to the start of disk and then jump to end
int distance = current_position;
total_movement += distance;
printf("Move from %d -> 0 (Distance: %d)\n", current_position, distance);
printf("Jump from 0 -> %d (Distance: %d)\n", disk_size - 1, disk_size - 1);
total_movement += disk_size - 1;
current_position = disk_size - 1;
// Service requests from the end again
for (int i = n-1; i >= 0; i--) {
if (sorted_requests[i] > head) {
distance = current_position - sorted_requests[i];
total_movement += distance;
printf("Move from %d -> %d (Distance: %d)\n",
current_position, sorted_requests[i], distance);
current_position = sorted_requests[i];
}
}
}
printf("\nTotal Head Movement: %d\n", total_movement);
printf("Average Seek Length: %.2f\n\n", (float)total_movement / n);

}
int main() {
int n, head, disk_size, direction;
printf("Enter the number of disk requests: ");
scanf("%d", &n);
int req[n];
printf("Enter the disk request sequence:\n");
for (int i = 0; i < n; i++) {
scanf("%d", &req[i]);
}
printf("Enter the initial head position: ");
scanf("%d", &head);
printf("Enter the disk size (max track number): ");
scanf("%d", &disk_size);
printf("Enter the initial direction (0 for left, 1 for right): ");
scanf("%d", &direction);
fifo(req, n, head);
sstf(req, n, head);
scan(req, n, head, disk_size, direction);
cscan(req, n, head, disk_size, direction);
return 0;
}
