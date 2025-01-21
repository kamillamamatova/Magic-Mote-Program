#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

/* This program is designed to solve a "magic containment" problem, 
where you need to fit a given number of spherical objects 
(referred to as "motes") into cuboidal containers 
(referred to as "devices"). The goal is to minimize the total volume 
of motes that cannot be contained by any device. If a mote does not 
fit into any device, its volume contributes to the "uncontained volume." */

typedef struct {
    double volume;
    int original_index;
} VolumeIndex;

// Function to compute volume of a sphere (mote)
double compute_mote_volume(int radius) {
    return (4.0/3.0) * PI * pow(radius, 3);
}

// Function to compute volume of a box (device)
double compute_device_volume(int length, int width, int height) {
    return (double)length * width * height;
}

// Merge function for merge sort
void merge(VolumeIndex arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // Create temporary arrays
    VolumeIndex L[n1], R[n2];  // Changed to stack allocation
    
    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    i = 0;
    j = 0;
    k = left;
    
    while (i < n1 && j < n2) {
        if (L[i].volume >= R[j].volume) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(VolumeIndex arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Function to check if a mote fits in a device
int fits_in_device(double mote_volume, double device_volume) {
    return mote_volume <= device_volume + 1e-9;
}

double solve_magic_containment(int M, int D, int motes[], int devices[][3]) {
    VolumeIndex* mote_volumes = malloc(M * sizeof(VolumeIndex));
    VolumeIndex* device_volumes = malloc(D * sizeof(VolumeIndex));
    
    // Compute volumes
    for (int i = 0; i < M; i++) {
        mote_volumes[i].volume = compute_mote_volume(motes[i]);
        mote_volumes[i].original_index = i;
    }
    
    for (int i = 0; i < D; i++) {
        device_volumes[i].volume = compute_device_volume(devices[i][0], devices[i][1], devices[i][2]);
        device_volumes[i].original_index = i;
    }
    
    // Sort both arrays in descending order
    merge_sort(mote_volumes, 0, M - 1);
    merge_sort(device_volumes, 0, D - 1);
    
    // Track used devices
    int* device_used = calloc(D, sizeof(int));
    double uncontained_volume = 0.0;
    
    // Try to fit each mote
    for (int i = 0; i < M; i++) {
        int found_fit = 0;
        
        // Find smallest possible device that fits
        for (int j = D - 1; j >= 0; j--) {
            if (!device_used[j] && fits_in_device(mote_volumes[i].volume, device_volumes[j].volume)) {
                device_used[j] = 1;
                found_fit = 1;
                break;
            }
        }
        
        if (!found_fit) {
            uncontained_volume += mote_volumes[i].volume;
        }
    }
    
    free(mote_volumes);
    free(device_volumes);
    free(device_used);
    
    return uncontained_volume;
}

int main() {
    int M, D;
    scanf("%d %d", &M, &D);
    
    int* motes = malloc(M * sizeof(int));
    for (int i = 0; i < M; i++) {
        scanf("%d", &motes[i]);
    }
    
    int (*devices)[3] = malloc(D * sizeof(*devices));
    for (int i = 0; i < D; i++) {
        scanf("%d %d %d", &devices[i][0], &devices[i][1], &devices[i][2]);
    }
    
    double result = solve_magic_containment(M, D, motes, devices);
    printf("%.6f\n", result);
    
    free(motes);
    free(devices);
    
    return 0;
}
