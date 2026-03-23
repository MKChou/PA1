#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct ListNode
{
    int val;
    struct ListNode *next;
};

/* TODO: Implement your code below */
bool hasCycle(struct ListNode *head)
{
    struct ListNode *slow = head;
    struct ListNode *fast = head;
    struct ListNode *meet = NULL;
    bool result = false;

    asm volatile(




  "mv      t0, %[head]          \n\t" // slow = head
        "mv      t1, %[head]          \n\t" // fast = head
        "li      %[result], 0         \n\t" // 預設 result = 0

        "1:                           \n\t"
        "beqz    t1, 2f               \n\t" // if (fast == NULL) break
        "ld      t1, 8(t1)            \n\t" // fast = fast->next
        "beqz    t1, 2f               \n\t" // if (fast == NULL) break
        "ld      t1, 8(t1)            \n\t" // fast = fast->next
        
        "ld      t0, 8(t0)            \n\t" // slow = slow->next

        "beq     t0, t1, 3f           \n\t" // if (slow == fast) 跳到處理環的邏輯
        "j       1b                   \n\t" // 繼續迴圈

        "3:                           \n\t" // --- 找到環了 ---
        "li      %[result], 1         \n\t" // result = 1
        "mv      %[meet], t0          \n\t" // 關鍵新增：將相遇的節點位址存回 meet！
        
        "2:                           \n\t" // 結束
        : [result] "+r" (result),
          [meet] "+r" (meet)                // 關鍵新增：把 meet 加入輸出變數
        : [head] "r" (head)
        : "t0", "t1", "memory"




    );

    if (result) 
        printf("%d\n", meet->val);
    else
        printf("NULL\n");
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

    int node_count, edge_count;
    
    // Read first line: node_count and edge_count
    if (fscanf(input, "%d %d", &node_count, &edge_count) != 2) {
        fprintf(stderr, "Error reading node and edge count\n");
        if (argc >= 2) fclose(input);
        return 1;
    }

    if (node_count == 0) {
        printf("NULL\n");
        printf("false\n");
        if (argc >= 2) fclose(input);
        return 0;
    }

    // Allocate nodes array
    struct ListNode **nodes = (struct ListNode**)malloc(node_count * sizeof(struct ListNode*));
    
    // Read second line: node values and create nodes
    for (int i = 0; i < node_count; i++) {
        int val;
        if (fscanf(input, "%d", &val) != 1) {
            fprintf(stderr, "Error reading node value\n");
            if (argc >= 2) fclose(input);
            return 1;
        }
        nodes[i] = (struct ListNode*)malloc(sizeof(struct ListNode));
        nodes[i]->val = val;
        nodes[i]->next = NULL;
    }

    // Read edges and set next pointers
    for (int i = 0; i < edge_count; i++) {
        int from_val, to_val;
        if (fscanf(input, "%d %d", &from_val, &to_val) != 2) {
            fprintf(stderr, "Error reading edge\n");
            if (argc >= 2) fclose(input);
            return 1;
        }

        // Find 'from' and 'to' nodes by value
        struct ListNode *from_node = NULL;
        struct ListNode *to_node = NULL;

        for (int j = 0; j < node_count; j++) {
            if (nodes[j]->val == from_val) {
                from_node = nodes[j];
            }
            if (nodes[j]->val == to_val) {
                to_node = nodes[j];
            }
        }

        if (from_node && to_node) {
            from_node->next = to_node;
        }
    }

    if (argc >= 2) {
        fclose(input);
    }

    struct ListNode *head = (node_count > 0) ? nodes[0] : NULL;

    bool has_cycle = hasCycle(head);

    // Print the result
    if (has_cycle) {
        printf("true\n");
    } else {
        printf("false\n");
    }

    for (int i = 0; i < node_count; i++) {
        free(nodes[i]);
    }
    free(nodes);

    return 0;
}