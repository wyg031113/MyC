#include<stdio.h>
#include<stdlib.h>
typedef struct tree{
	int data;
	struct tree *lchild,*rchild;
}tree,*Tree;

int count;//ȫ�ֱ��� ������Žڵ�� 
int main(){
	int Btree(Tree *t,tree *node);
	int xianxu(Tree t);
	int i,j,temp;
	
	tree node[100];
	while(scanf("%d",&temp)&&temp){		//�������� ��0���� 
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
	}// �������� ������������� �����a[1]~~a[count]�������� 
	for(j=0;j<count;j++)
		printf("%d ",node[j].data);
		printf("\n");
	Tree t;
	Btree(&t,node);
	
	printf("%d ",t->data);
	xianxu(t);    //���������Ǿ�Ͳ�֪��զ��  ���t->data һ����10 һ����1 
					// �ҵ���������ʱ 1 2 3 4 0 
	return 0;
}
	int j=1;
int Btree(Tree *t,tree *node){
		tree p;
			
		p.lchild=&node[j]; 
		p.rchild=&node[j+1];	
		p.data=node[j].data+node[j+1].data;//ȡС��2���ڵ� ����µĽڵ� 
		
		int i;
		for(i=count;i>j;i--){
			if(p.data<node[i].data)
				node[i+1]=node[i];
			else {
				node[i+1]=p;
				count++;
				break;
			} //���µĽڵ���뵽��ǰ�Ľṹ�������� �����´���ȡ2���� 
		}
		j+=2;
		if(j!=count)
			Btree(t,node);//����������һ���ڵ�Ļ� �ظ�������� 
		else {
			*t=(Tree)malloc(sizeof(tree));
			*t=&p;//����һ��ָ����ڵ��ָ�� 
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


