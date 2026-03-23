#include <stdio.h>
#include <string.h>

/* TODO: Implement your code below */
int insertionSort(float *p_a, int arr_size)
{
    int shift_cnt = 0;  /* shift count */

    for (int i = 1; i < arr_size; i++) {
        float *p_i = p_a + i;  /* index of i-th element */
        int j = i - 1;
        
        asm volatile(



"flw    fa0, 0(%[p_i])          \n\t" // 1. key = *p_i (載入要插入的值)
            
            "1:                             \n\t" // while 迴圈開始標籤
            "bltz   %[j], 2f                \n\t" // 2. if (j < 0) 跳到標籤 2 (結束迴圈)
            
            "slli   t0, %[j], 2             \n\t" // 3. t0 = j * 4 (計算位移量)
            "add    t1, %[p_a], t0          \n\t" // 4. t1 = p_a + t0 (指向 A[j] 的記憶體位址)
            "flw    fa1, 0(t1)              \n\t" // 5. fa1 = A[j] (載入 A[j])

            "flt.s  t2, fa0, fa1            \n\t" // 6. t2 = (key < A[j]) 
            "beqz   t2, 2f                  \n\t" // 7. if (key >= A[j]) 跳到標籤 2 (結束迴圈)

            "fsw    fa1, 4(t1)              \n\t" // 8. A[j+1] = A[j] (把較大的值往後挪)
            "addi   %[j], %[j], -1          \n\t" // 9. j = j - 1
            "addi   %[shift_cnt], %[shift_cnt], 1 \n\t" // 10. shift_cnt++
            "j      1b                      \n\t" // 11. 跳回標籤 1 繼續比

            "2:                             \n\t" // 結束標籤
            "addi   t0, %[j], 1             \n\t" // 12. t0 = j + 1
            "slli   t0, t0, 2               \n\t" // 13. 計算 (j+1)*4
            "add    t1, %[p_a], t0          \n\t" // 14. 找到 A[j+1] 的位址
            "fsw    fa0, 0(t1)              \n\t" // 15. A[j+1] = key (把數字插進去)

            : [j] "+r" (j), [shift_cnt] "+r" (shift_cnt) // 輸出與輸入
            : [p_a] "r" (p_a), [p_i] "r" (p_i)           // 輸入
            : "t0", "t1", "t2", "fa0", "fa1", "memory"   // 會動到的暫存器




        );
    }
    
    return shift_cnt;
}

int main(int argc, char *argv[])
{
    FILE *input = stdin;
    FILE *output_file = NULL;
    float target = -1;
    
    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "Error opening file: %s\n", argv[1]);
            return 1;
        }
    }
    
    // Read 'target' from the input file of problem 2 (2_*.txt) first
    if (argc >= 3) {
        output_file = fopen(argv[2], "r");
        if (output_file) {
            fscanf(output_file, "%f", &target);
            fclose(output_file);
        }
    }
    
    int arr_size;
    fscanf(input, "%d", &arr_size);
    float arr[arr_size];

    // Read 'floats' from input into the array
    for (int i = 0; i < arr_size; i++) {
        float data;
        fscanf(input, "%f", &data);
        arr[i] = data;
    }
    
    if (argc >= 2) {
        fclose(input);
    }

    float *p_a = &arr[0];
    
    // Call insertion sort function
    int shift_cnt = insertionSort(p_a, arr_size);
    
    p_a = &arr[0];
    
    // Write to output file
    if (argc >= 3 && argv[2]) {
        output_file = fopen(argv[2], "w");
        if (output_file) {
            // target
            fprintf(output_file, "%.1f\n", target);
            // arr_size
            fprintf(output_file, "%d\n", arr_size);
            // sorted array
            for (int i = 0; i < arr_size; i++) {
                fprintf(output_file, "%.1f ", arr[i]);
            }
            fprintf(output_file, "\n");
            fclose(output_file);
        }
    }
    
    // Print the result
    for (int i = 0; i < arr_size; i++) {
        printf("%.1f ", *p_a++);
    }
    printf("\n");
    printf("%d\n", shift_cnt);
    return 0;
}
