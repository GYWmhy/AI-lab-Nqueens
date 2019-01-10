#include <cstdio>
#include <cstdlib>
#include <memory.h>
#include <iostream>
using namespace std;

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

void print(FILE *fp,int N){
	int queen[N];
	int i, j;
	int row[N], col[N], maindiagonal[N*2-1], vicediagonal[N*2-1]; 
	for(i = 0;i < N;i++){
		fscanf(fp,"%d",&queen[i]);
	}
	memset(row,0,N * sizeof(int));
	memset(col,0,N * sizeof(int));
	memset(maindiagonal,0,(2 * N - 1) * sizeof(int));
	memset(vicediagonal,0,(2 * N - 1) * sizeof(int));
	for(i = 0;i < N;i++){
		for(j = 0;j < N;j++){
			if(j == queen[i]){
				cout<<"Q ";
				row[i]++;
				col[j]++;
				maindiagonal[N - 1 - i + j]++;
				vicediagonal[i + j]++;
			} 
			else	cout<<". ";
		}
		cout<<endl;
	}
	int h_val = h(row,col,maindiagonal,vicediagonal,N);
	cout<<"queen = "<<N<<", attact = "<<h_val<<endl;
	cout<<endl;
}

int main(){
	FILE *fp;
	int N;
	fp = fopen("input.txt","r");
	fscanf(fp,"%d",&N);
	fclose(fp);
	
	fp = fopen("output_hill_climbing.txt","r");
	cout<<"hill-climbing:"<<endl;
	print(fp,N);
	fclose(fp);
	
	fp = fopen("output_simulate.txt","r");
	cout<<"simulate:"<<endl;
	print(fp,N);
	fclose(fp);
	getchar();
} 
