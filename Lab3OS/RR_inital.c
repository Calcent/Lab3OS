#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
} Process;



// Function to calculate waiting times and execution order for all processes
void calculateWaitingTimeAndExecutionOrder(Process proc[], int n, int quantum) {
    int* finish_time = malloc(n * sizeof(int));
    int* queue = malloc(n * sizeof(int));
    int* order = malloc(1000 * sizeof(int));
    int front = 0, rear = 0, orderCount = 0;
    int completed = 0, current_time, i;

    //Initialize remaining times
    for (i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
    }

    //Find earliest arrival
    int min_arr = proc[0].arrival_time;
    for (i = 1; i < n; i++)
        if (proc[i].arrival_time < min_arr)
            min_arr = proc[i].arrival_time;
    current_time = min_arr;

    //Enqueue any processes that have arrived at current_time
    for (i = 0; i < n; i++)
        if (proc[i].arrival_time <= current_time)
            queue[rear++] = i;

    //Simulation loop
    while (completed < n) {
        if (front == rear) {
            // no ready process -> idle one unit
            current_time++;
            for (i = 0; i < n; i++)
                if (proc[i].arrival_time == current_time)
                    queue[rear++] = i;
        }
        else {
            int idx = queue[front++];
            if (front == n) front = 0;

            int slice = proc[idx].remaining_time < quantum
                ? proc[idx].remaining_time
                : quantum;
            order[orderCount++] = proc[idx].process_id;

            proc[idx].remaining_time -= slice;
            current_time += slice;

            //Enqueue any newly arrived during this slice
            for (i = 0; i < n; i++) {
                if (proc[i].arrival_time > current_time - slice
                    && proc[i].arrival_time <= current_time) {
                    queue[rear++] = i;
                    if (rear == n) rear = 0;
                }
            }

            if (proc[idx].remaining_time > 0) {
                queue[rear++] = idx;
                if (rear == n) rear = 0;
            }
            else {
                finish_time[idx] = current_time;
                completed++;
            }
        }
    }

    //Compute waiting times
    for (i = 0; i < n; i++) {
        proc[i].waiting_time = finish_time[i]
            - proc[i].arrival_time
                - proc[i].burst_time;
    }

    //Print execution order
    printf("RR Execution Order: ");
    for (i = 0; i < orderCount; i++)
        printf("%d ", order[i]);
    printf("\n\n");

    free(finish_time);
    free(queue);
    free(order);
    
}

// Function to calculate turnaround times for all processes
void calculateTurnaroundTime(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].turnaround_time = proc[i].burst_time
            + proc[i].waiting_time;
    }
}

void roundRobin(Process proc[], int n, int quantum) {
    calculateWaitingTimeAndExecutionOrder(proc, n, quantum);
    calculateTurnaroundTime(proc, n);
}

void printProcesses(Process proc[], int n) {
    printf("Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               proc[i].process_id, proc[i].arrival_time, proc[i].burst_time,
               proc[i].waiting_time, proc[i].turnaround_time);
    }
}

int main() {
    Process proc[] = {{1, 0, 24}, {2, 0, 3}, {3, 0, 3}};
    int n = sizeof(proc) / sizeof(proc[0]); // Number of processes
    int quantum = 4; // Time quantum for Round Robin scheduling
 

    roundRobin(proc, n, quantum);
    printProcesses(proc, n);

    return 0;
}
