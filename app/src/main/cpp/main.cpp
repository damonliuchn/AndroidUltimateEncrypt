#include "main.hpp"


typedef int ElemType;
//定义节点
typedef struct Node {
    ElemType data;
    struct Node *next;
} Node;
//定义链表
typedef Node *LinkList;

/* 初始化顺序线性表 */
int initList(LinkList *L) {
    *L = (LinkList) malloc(sizeof(Node));
    if (!(*L)) {
        return 0;
    }
    (*L)->next = NULL;
    return 1;
}
/* 初始条件：顺序线性表L已存在,1≤i≤ListLength(L)， */
/* 操作结果：在L中第i个位置之前插入新的数据元素e，L的长度加1,要找i位置前边的那个节点 */
int ListInsert(LinkList *L, int i, ElemType e) {
    int j;
    LinkList p, s;
    p = *L;
    j = 1;
    while (p && j < i) /* 寻找第i个结点 */
    {
        p = p->next;
        j++;
    }

    if (!p) {
        return 0;    /* 第i个元素不存在 */
    }

    s = (LinkList) malloc(sizeof(Node)); /* 生成新结点(C语言标准函数) */
    s->data = e;
    s->next = p->next; /* 将p的后继结点赋值给s的后继 */
    p->next = s; /* 将s赋值给p的后继 */
    return 1;
}

/* 打印 */
void print(LinkList *L) {
    LinkList p;
    p = *L;
    p = p->next;
    while (p) /* 寻找第i个结点 */
    {
        LOGE("%d\n", p->data);
        p = p->next;
    }
}

//delete
int deleteList(LinkList *L, int i) {
    int j;
    LinkList p, s;
    p = *L;
    j = 1;
    while (p && j < i) /* 寻找第i个结点 */
    {
        p = p->next;
        j++;
    }

    if (!p) {
        return 0;    /* 第i个元素不存在 */
    }
    if (!(p->next)) {
        return 0;    /* 第i个元素不存在 */
    }


    s = p->next;
    p->next = s->next;
    return 1;
}

//算长度
int lengthList(LinkList *L) {
    int j;
    LinkList p;
    p = *L;
    j = -1;
    while (p) /* 寻找第i个结点 */
    {
        p = p->next;
        j++;
    }

    return j;

}

//清空
void clear(LinkList *L) {
    LinkList p, q;
    p = *L;
    while (p->next) /* 寻找第i个结点 */
    {
        q = p->next;
        p->next = q->next;
        free(q);
    }
}

//创建
void create(LinkList *L) {
    int i, j;
    LinkList p, q;
    p = *L;
    srand(time(0));
    for (i = 0; i < 100; i++) {
        q = (LinkList) malloc(sizeof(Node));
        j = rand() % 100 + 1;
        q->data = j;
        q->next = p->next;
        p->next = q;
    }
}

int main() {
    LinkList l;
    int i;
    //初始化
    initList(&l);
    //插入
    for (i = 1; i <= 10; i++) {
        ListInsert(&l, i, i);
    }
    //打印
    print(&l);
    //删除
    deleteList(&l, 3);
    print(&l);
    //算长度
    i = lengthList(&l);
    LOGE("length:%d", i);
    //清空
    clear(&l);
    //算长度
    i = lengthList(&l);
    LOGE("length:%d", i);
    //创建
    create(&l);
    //打印
    print(&l);
    LOGE("\n");
    return 0;
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_masonliu_testndk_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    std::string hello = "Hello from C++";
    char str[] = "abc";
    LOGE("ssssss");
    main();
    //return env->NewStringUTF("unknown");
    return env->NewStringUTF(str);
}