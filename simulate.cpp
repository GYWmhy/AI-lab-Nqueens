#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
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

double T;		//系统温度 
double r;		//降温快慢 
double T_min;		//温度下限 

struct timespec start, stop, pause1, pause2;

bool simulate(int N,int M,FILE *fp){	
	int row[N];		//行
	int col[N];		//列
	int maindiagonal[2 * N - 1];		//主对角线 
	int vicediagonal[2 * N - 1];		//副对角线
	int queen[N];		//按行记录皇后的位置
	int i, j, k, temp, tempi, tempj;
	int h_val, h_val_1;
	double dE;
	double random;
	
	FILE *f = fopen("data.txt","r");
	
	memset(row,0,N * sizeof(int));
	memset(col,0,N * sizeof(int));
	memset(maindiagonal,0,(2 * N - 1) * sizeof(int));
	memset(vicediagonal,0,(2 * N - 1) * sizeof(int));
	memset(queen,0,N * sizeof(int));
	srand(time(NULL));
	clock_gettime(CLOCK_MONOTONIC,&pause1);
	for(i = 0;i < N;i++){
		fscanf(f,"%d",&temp);
		queen[i] = temp;
		row[i]++;
		col[temp]++;
		maindiagonal[N - 1 - i + temp]++;
		vicediagonal[i + temp]++;
		//cout<<temp<<" "<<col[temp]<<" "<<maindiagonal[N - 1 - i + temp]<<" "<<vicediagonal[i + temp]<<endl;
	}
	fclose(f);
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
	if(h_val == M){
		clock_gettime(CLOCK_MONOTONIC,&pause2);
		printqueen(queen,N,fp);
		return true;
	}
	else{
		clock_gettime(CLOCK_MONOTONIC,&pause2);
		while(T > 0){
			if(h_val == 0)	break;
			
			tempi = rand() % N;		//随机选一行的棋子移动 
			do{
				tempj = rand() % N;		//随机移动到一列 
			}while(tempj == queen[tempi]);
			
			//cout<<tempi<<" "<<tempj<<endl;
			
			h_val_1 = movequeen(queen,row,col,maindiagonal,vicediagonal,N,tempi,tempj);
			//cout<<"h="<<h_val<<" h1="<<h_val_1;
			
			if(h_val_1 < M){
				dE = h_val_1 - h_val;
			}
			else{
				dE = h_val - h_val_1;
			}
			
			//cout<<" dE="<<dE;
			if(dE >= 0){		//无条件接受h值降低的移动 
				h_val = h_val_1;
				temp = queen[tempi];
				col[temp]--;
				col[tempj]++;
				maindiagonal[N - 1 - tempi + temp]--;
				maindiagonal[N - 1 - tempi + tempj]++;
				vicediagonal[tempi + temp]--;
				vicediagonal[tempi + tempj]++;
				queen[tempi] = tempj;
				if(h_val == M)	break;
			}
			else{
				random = rand() % 100;
				if(exp(dE / T) * 100 > random){		//接受这个h值升高的移动 
					h_val = h_val_1;
					temp = queen[tempi];
					col[temp]--;
					col[tempj]++;
					maindiagonal[N - 1 - tempi + temp]--;
					maindiagonal[N - 1 - tempi + tempj]++;
					vicediagonal[tempi + temp]--;
					vicediagonal[tempi + tempj]++;
					queen[tempi] = tempj;
					//cout<<" P="<<exp(dE/T);
				}
			}
			T = r * T;
			//cout<<" T="<<T<<endl;
	/*for(i = 0;i < N;i++){
		for(j = 0;j < N;j++){
			if(queen[i] == j)	cout<<"Q ";
			else	cout<<". ";
		}
		cout<<endl;
	}
	cout<<endl;*/
		}
		if(h_val == M){
			printqueen(queen,N,fp);
			return true;
		}
		else	return false;
	}
}

int main(){
	int i, j;
	int off = 0;
	double dt;
	bool sim;
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
	
	fp = fopen("output_simulate.txt","w+");
	if(!fp){
		cout<<"Open file failed!"<<endl;
		exit(0);
	}
	
	do{
		T = 5;
		r = 0.98;
		T_min = T / 10000;
		clock_gettime(CLOCK_MONOTONIC,&start);
		sim = simulate(N,M,fp);
		clock_gettime(CLOCK_MONOTONIC,&stop);
	}while(!sim);

	dt = (stop.tv_sec - pause2.tv_sec + pause1.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - pause2.tv_nsec + pause1.tv_nsec - start.tv_nsec) / 1000000000;
	dt *= 1000;
	
	fprintf(fp,"%.8lf\n",dt);
	fclose(fp);
}
