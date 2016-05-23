#include<stdio.h>
#include<stdlib.h>
typedef struct tree{
	int data;
	struct tree *lchild,*rchild;
}tree,*Tree;

int count;//全局变量 用来存放节点的 
int main(){
	int Btree(Tree *t,tree *node);
	int xianxu(Tree t);
	int i,j,temp;
	
	tree node[100];
	while(scanf("%d",&temp)&&temp){		//输入数据 以0结束 
		for(i=count-1;i>=0;i--){
			if(temp<node[i].data){
				node[i+1]=node[i];
			}
			else {
			
				break;
			}
		}
		
		node[i+1].data=temp;
		count++;
	}// 输入数据 并将数据排序好 存放在a[1]~~a[count]数组里面 
	for(j=0;j<count;j++)
		printf("%d ",node[j].data);
		printf("\n");
	Tree t;
	Btree(&t,node);
	
	printf("%d ",t->data);
	xianxu(t);    //这句和上面那句就不知道咋了  输出t->data 一个是10 一个是1 
					// 我的输入数据时 1 2 3 4 0 
	return 0;
}
	int j=1;
int Btree(Tree *t,tree *node){
		tree p;
			
		p.lchild=&node[j]; 
		p.rchild=&node[j+1];	
		p.data=node[j].data+node[j+1].data;//取小的2个节点 组成新的节点 
		
		int i;
		for(i=count;i>j;i--){
			if(p.data<node[i].data)
				node[i+1]=node[i];
			else {
				node[i+1]=p;
				count++;
				break;
			} //将新的节点插入到以前的结构体数组中 便于下次再取2个数 
		}
		j+=2;
		if(j!=count)
			Btree(t,node);//如果不是最后一个节点的话 重复上面操作 
		else {
			*t=(Tree)malloc(sizeof(tree));
			*t=&p;//返回一个指向根节点的指针 
		}
		return 0;
}

int xianxu(Tree t){
	if(!t)
		return 0;
	else {
		printf("%d ",t->data);
		xianxu(t->lchild);
		xianxu(t->rchild);
	}	
}


