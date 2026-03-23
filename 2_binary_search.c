#include <stdio.h>

/* TODO: Implement your code below */
int binarySearch(float *p_a, int arr_size, float target)
{
    int result = -1;

    asm volatile(

        // --- 1. 初始化內部使用的暫存器 ---
        "li      t3, 0                  \n\t"   // t3 = left = 0
        "addi    t4, %[arr_size], -1    \n\t"   // t4 = right = arr_size - 1
        "fmv.s   fa0, %[target]         \n\t"   // fa0 = target

        "1:                             \n\t"   // while 迴圈開始
        "blt     t4, t3, 2f             \n\t"   // if (right < left) 結束

        // --- 2. mid = (left + right) / 2 ---
        "add     t0, t3, t4             \n\t"
        "srai    t0, t0, 1              \n\t"   // t0 = mid

        // --- 3. 讀取 A[mid] ---
        "slli    t1, t0, 2              \n\t"   // offset = mid * 4
        "add     t1, %[p_a], t1         \n\t"
        "flw     fa1, 0(t1)             \n\t"   // fa1 = A[mid]

        // --- 4. 比較 A[mid] == target ---
        "feq.s   t2, fa1, fa0           \n\t"
        "bnez    t2, 3f                 \n\t"   // 找到了，跳到標籤 3

        // --- 5. 比較 A[mid] < target ---
        "flt.s   t2, fa1, fa0           \n\t"
        "bnez    t2, 4f                 \n\t"   // 若小於，跳到 4 (往右找)

        // --- 6. Else: A[mid] > target (往左找) ---
        "addi    t4, t0, -1             \n\t"   // right = mid - 1
        "j       1b                     \n\t"

        "4:                             \n\t"   // Target in right half
        "addi    t3, t0, 1              \n\t"   // left = mid + 1
        "j       1b                     \n\t"

        "3:                             \n\t"   // Success!
        "mv      %[result], t0          \n\t"   // 將 mid 存入 result
        
        "2:                             \n\t"   // End
        : [result] "+r" (result)
        : [p_a] "r" (p_a), 
          [arr_size] "r" (arr_size), 
          [target] "f" (target)
        : "t0", "t1", "t2", "t3", "t4", "fa0", "fa1", "memory"

    );

    return result;
}

int main(int argc, char *argv[])
{
    FILE *input = stdin;
    
    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "Error opening file: %s\n", argv[1]);
            return 1;
        }
    }
    
    // Read 'target'
    float target;
    fscanf(input, "%f", &target);
    
    // Read 'arr_size'
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

    int index = binarySearch(p_a, arr_size, target);

    // Print the result
    printf("%d ", index);
    printf("\n");

    return 0;
}
