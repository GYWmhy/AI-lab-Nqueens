#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <memory.h>
#include <iostream>
using namespace std;

#define Q 9999 

int N, M;

int sigma(int n){		//求和 
	int sum = 0;
	while(n){
		n--;
		sum += n;
	}
	return sum;
}

int h(int r[],int c[],int md[],int vd[],int N){		//返回相互攻击的皇后对数 
	int i;
	int sum = 0;
	for(i = 0;i < 2 * N - 1;i++){
		if(i < N){
			if(c[i] > 1)	sum += sigma(c[i]);
		}
		if(md[i] > 1)	sum += sigma(md[i]);
		if(vd[i] > 1)	sum += sigma(vd[i]);
	}
	return(sum);
}

void printqueen(int queen[],int N,FILE *fp){
	int i;
	for(i = 0;i < N;i++){
		fprintf(fp,"%d\n",queen[i]);
	}
}

int movequeen(int queen[],int r[],int c[],int md[],int vd[],int N,int a,int b){		//计算该位置的h值 
	int pos = queen[a];
	int col[N], moved[2 * N - 1], viced[2 * N - 1];
	memcpy(col,c,N * sizeof(int));
	memcpy(moved,md,(2 * N - 1) * sizeof(int));
	memcpy(viced,vd,(2 * N - 1) * sizeof(int));
	col[pos]--;
	col[b]++;
	moved[N - 1 - a + pos]--;
	moved[N - 1 - a + b]++;
	viced[a + pos]--;
	viced[a + b]++;
	return h(r,col,moved,viced,N);
}

struct timespec start, stop, pause1, pause2;

bool hill_climbing(int N,int M,FILE *fp){	
	int row[N];		//行
	int col[N];		//列
	int maindiagonal[2 * N - 1];		//主对角线 
	int vicediagonal[2 * N - 1];		//副对角线
	int queen[N];		//按行记录皇后的位置
	int moveh[N][N];		//记录各行皇后移动到对应位置的h值 
	int i, j, k, temp;
	int h_val;
	int min_i, min_j, min = Q;
	int minarray_i[N], minarray_j[N];
	
	FILE *f = NULL;
	
	int idtime = 0;
	
	memset(row,0,N * sizeof(int));
	memset(col,0,N * sizeof(int));
	memset(maindiagonal,0,(2 * N - 1) * sizeof(int));
	memset(vicediagonal,0,(2 * N - 1) * sizeof(int));
	memset(queen,0,N * sizeof(int));
	memset(moveh,0,N * N * sizeof(int));
	memset(minarray_i,Q,N * sizeof(int));
	memset(minarray_j,Q,N * sizeof(int));
	srand(time(NULL));
	clock_gettime(CLOCK_MONOTONIC,&pause1);
	recreate: 
	for(i = 0;i < N;i++){
		queen[i] = temp = rand() % N;
		row[i]++;
		col[temp]++;
		maindiagonal[N - 1 - i + temp]++;
		vicediagonal[i + temp]++;
		//cout<<temp<<" "<<col[temp]<<" "<<maindiagonal[N - 1 - i + temp]<<" "<<vicediagonal[i + temp]<<endl;
	}
	/*for(i = 0;i < N;i++){
		for(j = 0;j < N;j++){
			if(queen[i] == j)	cout<<"Q ";
			else	cout<<". ";
		}
		cout<<endl;
	}
	cout<<endl;*/
	h_val = h(row,col,maindiagonal,vicediagonal,N);
	//cout<<h_val<<endl;
	if(h_val < M)	goto recreate;
	else if(h_val == M){
		f = fopen("data.txt","w+");
		for(i = 0;i < N;i++)	fprintf(f,"%d ",queen[i]);
		fclose(f);
		clock_gettime(CLOCK_MONOTONIC,&pause2);
		printqueen(queen,N,fp);
		return true;
	}
	else{
		f = fopen("data.txt","w+");
		for(i = 0;i < N;i++)	fprintf(f,"%d ",queen[i]);
		fclose(f);
		clock_gettime(CLOCK_MONOTONIC,&pause2);
		while(h_val != M){
			//cout<<h_val<<endl;
			min = Q;
			k = 0;
			idtime++;
			for(i = 0;i < N;i++){
				for(j = 0;j < N;j++){
					if(queen[i] == j)	moveh[i][j] = Q;
					else{
						moveh[i][j] = movequeen(queen,row,col,maindiagonal,vicediagonal,N,i,j);
						if(moveh[i][j] <= min && moveh[i][j] >= M){		//找局部最小值，且不能小于需求攻击的对数 
							if(moveh[i][j] == min){
								minarray_i[k] = i;
								minarray_j[k++] = j;
							}
							else{
								k = 0;
								min = moveh[i][j];
								min_i = i;
								min_j = j;
							}
							
						}
					}
				}
			}
			if(k > 0){
				temp = rand() % k;		//最小值中随机选一个移动 
				min_i = minarray_i[temp];
				min_j = minarray_j[temp];
			}
			temp = queen[min_i];
			col[temp]--;
			col[min_j]++;
			maindiagonal[N - 1 - min_i + temp]--;
			maindiagonal[N - 1 - min_i + min_j]++;
			vicediagonal[min_i + temp]--;
			vicediagonal[min_i + min_j]++;
			queen[min_i] = min_j;
			h_val = min;
	/*for(i = 0;i < N;i++){
		for(j = 0;j < N;j++){
			if(queen[i] == j)	cout<<"Q ";
			else	cout<<". ";
		}
		cout<<endl;
	}
	cout<<endl;*/
			if(idtime > N * N * N)	return false;
		}
		printqueen(queen,N,fp);
		return true;
	}
}


int main(){
	int i, j;
	int off = 0;
	double dt;
	bool hill;
	FILE *fp;
	fp = fopen("input.txt","r");
	if(!fp){
		cout<<"Open file failed!"<<endl;
		exit(0);
	}
	fscanf(fp,"%d",&N);
	fscanf(fp,"%d",&M);
	fclose(fp);
	if(M > sigma(N))	exit(-1); 
	
	fp = fopen("output_hill_climbing.txt","w+");
	if(!fp){
		cout<<"Open file failed!"<<endl;
		exit(0);
	}
	
	do{
		clock_gettime(CLOCK_MONOTONIC,&start);
		hill = hill_climbing(N,M,fp);
		clock_gettime(CLOCK_MONOTONIC,&stop);
	}while(!hill);

	dt = (stop.tv_sec - pause2.tv_sec + pause1.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - pause2.tv_nsec + pause1.tv_nsec - start.tv_nsec) / 1000000000;
	dt *= 1000;
	
	fprintf(fp,"%.8lf\n",dt);
	fclose(fp);
}
