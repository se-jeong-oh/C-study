#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __node {
    char name; // 사람 이름 저장
    struct __node *father; // 아빠 노드를 가리키는 포인터
    struct __node *mother; // 엄마 노드를 가리키는 포인터
} Family; 

typedef struct __queue {
    struct __queue *next; // 다음 큐를 가리키는 포인터
    Family *person; // Family Tree의 어떤 노드(사람)를 가리키는 포인터
} Queue;

//함수 설명은 main 함수 밑에
Queue *q_pop(Queue **head); // 큐에서 pop
void q_push(Family *parent, Queue **head, Queue **tail); // 큐로 push
Family *child_search(Family *head, char name); // Tree에서 name과 동일한 이름을 가진 노드 탐색
void print_tree(Family *head); // Tree 출력

int main(){
    char *input = (char *)malloc(sizeof(char)*10); // 입력을 받을 변수
    char child, parent, classifier; // 자식의 이름, 부모의 이름, 분류자(M / F)
    Family *head = NULL; // Tree의 최상위 노드를 가리키는 포인터

    while(1){
        scanf("%s", input);
        if (strcmp(input, "$$$") == 0){ // input이 $$$ 이라면 프로그램 종료.
            break;
        }
        Family *curr; // 자녀 탐색 시 탐색 결과를 저장하는 노드
        Family *new_node = (Family *)malloc(sizeof(Family)); // 새롭게 추가할 노드.
        new_node->mother = NULL; // 초기화
        new_node->father = NULL; // 초기화

        child = input[0]; // 자녀 이름 저장
        classifier = input[1]; // 지시자 저장
        parent = input[2]; // 부모 이름 저장

        if (head == NULL){ // Tree가 비어있다면
            Family *node = (Family *)malloc(sizeof(Family)); // 자녀 노드, 최상위 노드가 될 예정.
            node->father = NULL;
            node->mother = NULL;

            head = node; // 최상위 노드
            node->name = child; // 자녀 이름 저장
            if (classifier == 'M'){ // 부모가 엄마이면
                node->mother = new_node; 
                new_node->name = parent; 
            }
            else { // 부모가 아빠이면
                node->father = new_node;
                new_node->name = parent;
            }
        }
        else { // Tree가 비어있지 않다면
            curr = child_search(head, child); // child를 Tree에서 검색. 없다면 curr == NULL.
            if(curr == NULL){ // 자녀가 Tree에 없다면, 부모가 최상위 노드 (예시 데이터에서 pFa의 경우, (자녀 p, 부모 a 인데 현재 a가 최상위 노드))
                new_node->name = child; 
                if(classifier == 'M') // 부모가 엄마라면
                    new_node->mother = head; // 현재 head가 가리키고 있는 건 최상위 노드이자, 부모 노드(pFa 에서 a를 가리키고 있는 상황)
                else
                    new_node->father = head;
                head = new_node; // 새로운 자녀가 추가 되었으므로, head를 최상위 노드로 변경.
            }
            else { // 자녀가 트리에서 발견된 경우.
                if (classifier == 'M') // 부모가 엄마라면
                    curr->mother = new_node; // 자녀 노드에 엄마 노드를 연결
                else
                    curr->father = new_node; 

                new_node->name = parent; // 새로운 노드의 이름을 입력
            }
        }
        print_tree(head); // 트리를 출력
    }
}

/*
INPUT
-Queue **head : "큐의 가장 앞쪽 노드를 가리키고 있는 포인터(*)" 를 가리키는 포인터(더블 포인터, **) / 큐의 헤드.

FUNCTION
큐의 head가 가리키는 노드를 pop 하고 반환.

RETURN
-Queue *curr : pop한 큐의 노드, person값을 가지고 있음.
*/
Queue *q_pop(Queue **head){
    Queue *curr = (Queue *)malloc(sizeof(Queue)); 
    curr = *head; // 여기서 head는 더블포인터이고 curr는 포인터. 따라서 curr = **head(그냥 구조체) 혹은 curr = head(더블포인터)는 틀린 표현.
    *head = (*head)->next; // *head(Queue 포인터)에서 '->'를 사용해서 한번 더 참조하면 "(*head)->"는 구조체를 의미.
    return curr;
}

/*
INPUT
-Queue **head : "큐의 가장 앞쪽 노드를 가리키고 있는 노드(포인터, *)" 를 가리키는 노드(더블 포인터, **)
-Queue **tail : "큐의 가장 뒷쪽 노드를 가리키고 있는 노드(포인터, *)" 를 가리키는 노드(더블 포인터, **)
-Family *parent : Family Tree에 있는 노드를 가리키고 있는 노드.

FUNCTION
-큐에 가장 마지막(tail)에 노드를 추가.

RETURN
-None
*/
void q_push(Family *parent, Queue **head, Queue **tail){
    Queue *node = (Queue *)malloc(sizeof(Queue));
    node->person = parent;
    node->next = NULL;
    if ((*head) == NULL){ // 큐가 비어있을 경우
        *head = node; // 헤드가 신규 노드를 가리키도록
        *tail = node; // 꼬리가 신규 노드를 가리키도록
        return;
    }
    else {
        (*tail)->next = node; // 꼬리가 가리키는 노드의 next가 신규 노드를 가리키도록
        *tail = node; // 꼬리가 신규 노드를 가리키도록
        return;
    }
}

/*
INPUT
-Family *head : Family Tree의 최상위 노드를 가리키고 있는 포인터(*) / Family Tree의 헤드.
 pop과 push는 더블포인터 head를 쓴 이유:
    search 함수는 트리에 변화가 없고 단지 검색이 목적이지만, pop과 push는 구성 노드가 바뀌는 함수이기 때문.
-char name : 찾기 위한 자녀의 이름

FUNCTION
-찾는 child의 이름을 가진 노드를 Tree에서 찾아서 포인터를 반환
+) Level-Order traversal 를 구현하기 위해서 링크드리스트로 구현된 큐를 사용. push연산으로 리스트 마지막에 삽입하고, pop연산으로 리스트 처음에서 꺼낸다.

RETURN
-Family *curr : 찾는 이름을 가진 노드
*/
Family *child_search(Family *head, char name) {
    
    Queue *queue_head = NULL, *queue_tail = NULL;
    Queue *queue;
    Family *curr;

    curr = head; // Tree의 가장 최상위 노드부터 탐색 시작.
    while(1) {
        if(curr->name == name) // 현재 노드의 이름과 찾는 이름이 같다면
            return curr; 
        if(curr->father != NULL) // 현재 노드의 아빠 노드가 있다면
            q_push(curr->father, &queue_head, &queue_tail); // 해당 노드를 큐에 push
        if(curr->mother != NULL) // 현재 노드의 엄마 노드가 있다면
            q_push(curr->mother, &queue_head, &queue_tail); // 해당 노드를 큐에 push
        if(queue_head == NULL) // 큐가 완전히 비었다면(Tree를 전부 탐색했다면)
            break;
        queue = q_pop(&queue_head); // 큐에 가장 앞에 있는 노드를 pop
        curr = queue->person; // pop된 queue 안에 있는 Family Tree 노드를 curr에 전달.
    }

    return NULL; // 탐색이 끝났는데 해당 이름을 찾지 못했다면, 널을 반환.
}

/*
INPUT
-Family *head : Family Tree의 최상위 노드를 가리키고 있는 포인터(*) / Family Tree의 헤드.

FUNCTION
-트리의 노드를 차례대로 출력(level-order traversal 순서대로)

RETURN
-None
*/
void print_tree(Family *head) {
    Queue *queue_head = NULL, *queue_tail = NULL;
    Family *curr;
    Queue *queue;
    // 이하 구조는 출력부분을 제외하고 child_search와 동일
    curr = head;
    while(1) {
        printf("%c", curr->name); // 출력 부분
        if(curr->father != NULL)
            q_push(curr->father, &queue_head, &queue_tail);
        if(curr->mother != NULL)
            q_push(curr->mother, &queue_head, &queue_tail);
        if(queue_head == NULL)
            break;
        queue = q_pop(&queue_head);
        curr = queue->person;
    }
    printf("\n");
}

