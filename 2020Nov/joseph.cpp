#include <cstdio>
#include <cstdlib>

typedef struct node
{
    int name;
    int code;
    struct node *next;
}Node;

Node *getList(int n){
    Node *list, *p, *q;
    int i;
    list = (Node*)malloc(sizeof(Node));
    p = list;
    scanf("%d", &p->code);
    p->name = 1;


    for(i=1;i<n;i++){
        q = p;
        p = (Node*)malloc(sizeof(Node));
        scanf("%d", &p->code);
        p->name = i+1;
        q->next = p;
    }
    p->next = list;
    return list;
}

void JosephRing(Node *list, int m, int n){
    int i, j;
    Node *p, *q;
    p = list;
    for(j=0;j<n;j++){
        for(i=1;i<m;i++){
            q = p;
            p = p->next;
        }
        printf("%d ", p->name);
        q->next = p->next;
        m = p->code;
        p = p->next;
    }
}

int main(){
    int m, n;

    scanf("%d %d", &n, &m);

    Node *list = getList(n);
    JosephRing(list, m, n);
    return 0;
}