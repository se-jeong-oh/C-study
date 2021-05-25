#include <stdio.h>
#include <stdlib.h>

typedef struct __Node {
    int wait_num;
    int people_num; 
    struct __Node* up;
    struct __Node* left;
    struct __Node* right;    
}Node;

typedef Node* Node_pointer;

typedef struct __Queue {
    struct __Queue* next;
    int wait_num;
    int people_num;
}Queue;

typedef Queue* Queue_pointer;

void exchange(int *a, int *b);
Node_pointer find_insert(Node_pointer head, int heap_size);
void insert_sort_maxheap(Node_pointer insert_pos);
void root_sort_maxheap(Node_pointer head);
Node_pointer push_heap_linkedlist(Node_pointer head, Node_pointer new_node, int heap_size);
Node_pointer pop_heap_linkedlist(Node_pointer head, int* heap_size);
Queue_pointer push_queue(Queue_pointer queue_head, Node_pointer last);
Queue_pointer pop_queue(Queue_pointer* queue_head);
Node_pointer assign_table(Node_pointer* head, int* heap_size, int table_size);

int main() {
    int input;
    int heap_size = 0; // 현재 heap에 들어가 있는 사람 수 (대기 중인 인원 수)
    int curr_visit = 0; // 대기 인원 포함 오늘 식당에 온 사람 수 (대기번호와 관련)
    int table_size;

    Node_pointer head = NULL; // 처음엔 heap에 아무것도 없다.
    Node_pointer tmp = NULL; // 배정 시 대기번호와 인원수를 담기 위한 변수

    printf("Start of Program\n");
    printf("기능 : \n");
    printf("0.종료 1.대기접수 2.식탁배정\n");
    printf("식탁 대기수: 0\n");
    printf("----------------\n");

    while(1){
        printf("기능 선택? :");
        scanf("%d", &input);
        if (input == 0)
            break;
        else if (input == 1){ // 대기 접수
            if (curr_visit == 9) // 이미 오늘 9명이 왔다면
            {
                printf("예약이 마감되었습니다. 죄송합니다!\n"); // 이건 그냥 혹시 몰라서 넣은 예외처리
                continue;
            }
            Node_pointer new_node = (Node_pointer)malloc(sizeof(Node));
            new_node->up = NULL;
            new_node->left = NULL;
            new_node->right = NULL; // 초기화 과정
            new_node->wait_num = 9 - curr_visit; // 9명이 올 수 있는 최대 사람.
            curr_visit++;
            heap_size++;
            printf("대기번호 : %d\n", new_node->wait_num);
            printf("인원수? : ");
            scanf("%d", &(new_node->people_num));

            head = push_heap_linkedlist(head, new_node, heap_size);
        }
        else { // 식탁 배정
            printf("식탁 크기? : ");
            scanf("%d",&table_size);
            tmp = assign_table(&head, &heap_size, table_size);
            if (tmp == NULL) {
                printf("배정불가::\n");
            }
            else
                printf("배정 :: 대기번호: %d 인원수: %d\n", tmp->wait_num, tmp->people_num);
        }
        printf("식탁 대기수 : %d\n", heap_size);
        printf("----------------\n");
        if (curr_visit == 9 && heap_size == 0) {
            printf("영업 종료\n");
            break;
        }
    }
    printf("End of Program\n");
}

void exchange(int *a, int *b){
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

Node_pointer find_insert(Node_pointer head, int heap_size) { // heap_size에 해당하는 위치의 노드 주소를 반환

    int mod, port;
    Node_pointer curr = head;
    Node_pointer tmp;
    port = heap_size;

    if (heap_size == 1) {
        return head;
    }

    while(1) {
        mod = port % 2; // heap_size를 2로 나눈 나머지.
        port = (port - mod) / 2; // heap_size를 2로 나눈 몫.
        if (port == 1) {
            if (mod == 0) {
                if (curr->left != NULL) { // 이미 존재할 때는
                    curr = curr->left;
                    return curr;
                }
                curr->left = (Node_pointer)malloc(sizeof(Node));
                tmp = curr;
                curr = curr->left;
                curr->up = tmp;
                curr->left = NULL;
                curr->right = NULL;
            }
            else {
                if (curr->right != NULL) {
                    curr = curr->right;
                    return curr;
                }
                curr->right = (Node_pointer)malloc(sizeof(Node));
                tmp = curr;
                curr = curr->right;
                curr->up = tmp;
                curr->left = NULL;
                curr->right = NULL;
            }
            return curr;
        }
        if (mod == 0)
            curr = curr->left;
        else
            curr = curr->right;
    }
}

void insert_sort_maxheap(Node_pointer insert_pos) {
    while(1) {
        if (insert_pos->up == NULL || (insert_pos->wait_num < insert_pos->up->wait_num))
            break;
        else {
            exchange(&(insert_pos->wait_num), &(insert_pos->up->wait_num));
            exchange(&(insert_pos->people_num), &(insert_pos->up->people_num));
            insert_pos = insert_pos->up;
        }        
    }
}

void root_sort_maxheap(Node_pointer head) { // head에 새로운 원소가 추가되었을 때 (pop 연산 중) 정렬하는 함수

    Node_pointer curr = head;

    while(1) {
        if ((curr->left == NULL) && (curr->right == NULL)) {// 마지막 노드에 도달하면 함수 종료 (아래와 비교할 것이 없음)
            break;
        }
        else {
            if (curr->right == NULL) { // curr 기준 왼쪽에만 노드가 있다면, 그 노드와만 값을 비교하면 됨.
                if (curr->wait_num < curr->left->wait_num) {
                    exchange(&(curr->wait_num), &(curr->left->wait_num));
                    exchange(&(curr->people_num), &(curr->left->people_num));
                    curr = curr->left;
                }
                else {
                    break;
                }
            }
            else { // curr 기준 양쪽 다 노드가 있다면,
                if (curr->left->wait_num > curr->right->wait_num) { // 왼쪽이 오른쪽보다 더 크다면,
                    if (curr->wait_num < curr->left->wait_num) { // 그리고 그 왼쪽보다 현재 curr가 더 작다면,
                        exchange(&(curr->wait_num), &(curr->left->wait_num));
                        exchange(&(curr->people_num), &(curr->left->people_num));
                        curr = curr->left;
                    }
                    else { // 하지만 curr가 그 왼쪽보다 더 크다면,
                        break;
                    }
                }
                else { // 오른쪽이 왼쪽보다 더 크다면,
                    if (curr->wait_num < curr->right->wait_num) { // 그리고 그 오른쪽보다 현재 curr가 더 작다면,
                        exchange(&(curr->wait_num), &(curr->right->wait_num));
                        exchange(&(curr->people_num), &(curr->right->people_num));
                        curr = curr->right;
                    }
                    else { // 하지만 curr가 그 오른쪽보다 더 크다면,
                        break;
                    }
                }
            }
        }
    }
    return;
}

Node_pointer push_heap_linkedlist(Node_pointer head, Node_pointer new_node, int heap_size) { //heap_size는 push하고 난 뒤의 값.
    
    Node_pointer insert_pos;

    if(head == NULL) // 아직 heap에 아무것도 없다면.
        head = new_node;
    else {
        insert_pos = find_insert(head, heap_size); // heap_size에 해당하는 위치의 주소를 반환
        insert_pos->people_num = new_node->people_num;
        insert_pos->wait_num = new_node->wait_num;
        insert_sort_maxheap(insert_pos);
    }
    return head;
}

Node_pointer pop_heap_linkedlist(Node_pointer head, int* heap_size) { // 무조건 root 노드 pop
    
    Node_pointer last = find_insert(head, *heap_size); // head와 교환할 마지막 노드 주소
    Node_pointer tmp;
    if (head == last) { // heap에 head밖에 남아있지 않다면.
        return head;
    }
    exchange(&(head->people_num), &(last->people_num));
    exchange(&(head->wait_num), &(last->wait_num));
    tmp = last->up;
    last->up = NULL;

    if (tmp->right == last)
        tmp->right = NULL;
    else
        tmp->left = NULL; // 여기까지 노드 삭제 과정
    (*heap_size)--;
    root_sort_maxheap(head);
    return last;
}

Queue_pointer push_queue(Queue_pointer queue_head, Node_pointer last) { // last를 queue에 push

    Queue_pointer new_queue = (Queue_pointer)malloc(sizeof(Queue));
    new_queue -> next = NULL;
    new_queue -> wait_num = last->wait_num;
    new_queue -> people_num = last->people_num;

    Queue_pointer curr;

    if (queue_head == NULL)
        queue_head = new_queue;
    else {
        for (curr = queue_head; curr->next != NULL; curr = curr->next); // 끝나면 curr가 queue의 마지막을 가리킴
        curr->next = new_queue;
    }

    return queue_head;
}

Queue_pointer pop_queue(Queue_pointer* queue_head) {

    Queue_pointer curr = *queue_head;
    if (curr->next == NULL) {
        *queue_head = NULL;
        return curr;
    }
    else {
        *queue_head = curr->next;
        return curr;
    }
}

Node_pointer assign_table(Node_pointer* head, int* heap_size, int table_size) {
    
    Queue_pointer queue_head = NULL;
    Queue_pointer tmp = NULL;
    Node_pointer new_node = (Node_pointer)malloc(sizeof(Node));
    new_node->up = NULL;
    new_node->left = NULL;
    new_node->right = NULL;

    Node_pointer last = NULL;

    
    while(1) {
        if((*heap_size)== 1) { // heap에 head만 남아있다면
            if((*head)->people_num > table_size) {
                last = NULL;
                while(1) {
                    if (queue_head == NULL)
                        break;
                    tmp = pop_queue(&queue_head); // 큐에서 pop
                    new_node->people_num = tmp->people_num;
                    new_node->wait_num = tmp->wait_num;
                    (*heap_size)++;
                    *head = push_heap_linkedlist(*head, new_node, *heap_size);
                }
                break;
            }
            else {
                (*heap_size)--;
                last = *head;
                *head = NULL;
                while(1) {
                    if (queue_head == NULL) {
                        break;
                    }
                    tmp = pop_queue(&queue_head); // 큐에서 pop
                    new_node->people_num = tmp->people_num;
                    new_node->wait_num = tmp->wait_num;
                    (*heap_size)++;
                    *head = push_heap_linkedlist(*head, new_node, *heap_size);
                }
                break;
            }            
        }
        last = pop_heap_linkedlist(*head, heap_size); // 일단 heap 에서 pop
        if (last -> people_num > table_size) { // 사람 수가 배정할 테이블의 크기보다 크다면
            queue_head = push_queue(queue_head, last); // 배정할 수 없으므로 queue에 삽입
            continue;
        }
        else {// 배정할 수 있다면
            while(1) {
                if (queue_head == NULL)
                    break;
                tmp = pop_queue(&queue_head); // 큐에서 pop
                new_node->people_num = tmp->people_num;
                new_node->wait_num = tmp->wait_num;
                (*heap_size)++;
                *head = push_heap_linkedlist(*head, new_node, *heap_size);
            }
            break;
        }
    }
    return last;
}