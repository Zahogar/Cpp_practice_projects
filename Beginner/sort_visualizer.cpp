#include <iostream> 


void array_printer(int array[]) {
    for (int i = 0; i <= sizeof(array); i++) {
        std::cout << array[i] << " "; 
    }
    std::cout << "\n";
    return;
}

int* bubble_sort(int array[], int size){
    
    int *sorted = (int*)malloc(size * sizeof(int));
    if (!sorted) return NULL;
    memcpy(sorted, array, size * sizeof(int));
    
    array_printer(sorted);

    for (int i = 0; i < size -1; i++){
        for (int j = 0; j < size -i-1; j++) {
            
            if (sorted[j] > sorted[j + 1]) {
                int temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
        array_printer(sorted);
    }

    return sorted;
}

int* selection_sort(int array[], int size) {

    int *sorted = (int*)malloc(size * sizeof(int));
    if (!sorted) return NULL;
    memcpy(sorted, array, size * sizeof(int));

    array_printer(sorted);

    for (int i = 0; i < size; i++) {
        int min = i;
        for (int j = i; j < size; j++) {
            if (sorted[min] > sorted[j]) min = j;
        }
        int temp = sorted[i];
        sorted[i] = sorted[min];
        sorted[min] = temp;

        array_printer(sorted);
    }

    return sorted;
}

int* insertion_sort(int array[], int size) {
    
    int *sorted = (int*)malloc(size * sizeof(int));
    if (!sorted) return NULL;
    memcpy(sorted, array, size * sizeof(int));

    array_printer(sorted);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (sorted[i] < sorted[j]) {
                if (i > j) {
                    int value = sorted[i];
                    for (int k = i; k > j; k--) {
                        sorted[k] = sorted[k-1];
                    }
                    sorted[j] = value;
                }
            }
        }

        array_printer(sorted);
    }
    
    return sorted;
}

int main(int argc, char const *argv[]) {
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int scrambled_array[] = {5, 4, 3, 8, 1, 9, 7, 2, 6};

    int size_array = sizeof(array)/sizeof(int);

    std::cout << "Execution of bubble sort:\n";
    int* bubble = bubble_sort(scrambled_array, size_array);

    std::cout << "Execution of selective sort:\n";
    int* selection = selection_sort(scrambled_array, size_array);

    std::cout << "Execution of insertion sort:\n";
    int* insertion = insertion_sort(scrambled_array, size_array);

    std::cout << "Original array = "; 
    array_printer(scrambled_array);
    std::cout << "\n";

    std::cout << "Expected answer = ";
    array_printer(array);
    std::cout << "\n";
    
    std::cout << "Bubble sort = ";
    array_printer(bubble);
    std::cout << "\n";
    
    std::cout << "Selection sort = ";
    array_printer(selection);
    std::cout << "\n";

    std::cout << "Insertion sort = ";
    array_printer(insertion);
    std::cout << "\n";  

    free(insertion);
    free(selection);
    free(bubble);

    return 0;
}
