void swap(int* x, int* y) {
    int z = *x;
    *x = *y;
    *y = z;
}

void sort(int array[], int size) {
    for (int i = 0; i < size; i++) {
        int* min = &array[i];
        for (int j = i + 1; j < size; j++) {
            if (array[j] < *min) min = &array[j];
        }
        swap(&array[i], min);
    }
}
