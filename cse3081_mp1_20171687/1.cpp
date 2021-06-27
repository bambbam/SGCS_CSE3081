#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <time.h>
using namespace std;
FILE* a, *b;//input file, outputfile
int sp, ep;//starting point, endpoint(range)
double beforetime, aftertime;
int sol_n2(int n){
	vector<int> sum;//0 to ith sum = sum[i+1]
	int k = 0;//nothing is summed is sum is zero
	sum.push_back(k);
	for (int i = 0; i < n; i++) {
		int x; fscanf(a,"%d", &x);
		k += x;
		sum.push_back(k);
	}
	beforetime = clock();//after getting input, time start
	int ans = -2147483647;//firstly ans is set inf
	for (int i = 0; i < n+1; i++) {
		for (int j = i + 1; j < n+1; j++) {
			if(ans<sum[j]-sum[i]){
				ans = sum[j]-sum[i];//i ~ j-1 idx
				sp = i;// set starting point
				ep = j-1;//set end point
			}
		}
	}
	sum.clear();//memory free
	return ans;
}
int sol(vector<int>& in, int l, int r) {
	if (l == r) {
		return in[l];//if l==r, the max of sum is in[l]
	}
	int mid = (l + r) / 2;
	int ts1,te1;
	int ml = sol(in,l, mid);
	ts1 = sp;//ts1 : temporary starting point
	te1 = ep;//te1 : temporary end point
	//until here, sp save max sum's starting point bounded (l,mid), ep save max sum's end point bounded(l,mid)
	int mr = sol(in,mid + 1, r);
	//here, sp save max sum's starting point bounded (mid+1,r), ep save max sum's end point bounded (mid+1,r)
	int tmax;// tmax : temporary max value
	if(ml>mr){
		sp = ts1;
		ep = te1;
		tmax = ml;
	}
	else tmax = mr;
	//tmax saves maximum value of ml, mr
	//sp ,ep saves boundary of tmax
	int br = -2147483647, bl = -2147483647;
	//bl and br saves max value of left max sum and right max sum
	int sum = 0;
	//sum is the value to save temporary sum
	for (int i = mid; i >= l; i--) {
		sum += in[i];
		if(bl<sum){
			ts1 = i;
			bl = sum;
		}
	}
	sum = 0;
	for (int j = mid+1; j <= r; j++) {
		sum += in[j];
		if(br<sum){
			te1 = j;
			br = sum;
		}
	}
	if(bl+br>tmax){
		sp = ts1;
		ep = te1;
		tmax = bl+br;
	}
	//sp, ep saves boundary of max value in (l,r)
	return tmax;
}
int sol_nlgn(int n){
	vector<int> in;//input is saved in in vector
	for (int i = 0; i < n; i++) {
		int x; fscanf(a, "%d", &x); 
		in.push_back(x);
	}
	beforetime = clock();//after getting input, clock start
	int ans = sol(in, 0, n - 1);
	in.clear();//free memory
	return ans;
}
int sol_n(int n){
	int min2 = 0;
	int sum2 = 0,x;
	int ans = -2147483647;
	int tsp = 0;
	vector<int> v;// vector v saves input
	for(int i = 0;i<n;i++){
		int x; fscanf(a,"%d",&x);
		v.push_back(x);
	}
	beforetime = clock();//after input, clock start
	//sum2 saves sum of (0,i)
	//min2 saves min of sums and 0
	//max value must be in set of sum2-min2
	//tsp saves when min2's endpoint
	for(int i = 0; i < n; i++) {
		int x = v[i];
		sum2+=x;
		if(ans<sum2-min2){
			ans = sum2-min2;
			ep = i;
			sp = tsp;
		}
		if(min2>sum2){
			min2 = sum2;
			tsp = i+1;
		}
	}
	v.clear();//memory clear
	return ans;
}
int main(int argc, char* argv[] ) {
	a = fopen(argv[1],"r");//input file name is in argv[1]
	int n;
	fscanf(a,"%d",&n);
	int ans;
	if(argv[2][0]=='1') ans = sol_n2(n);
	if(argv[2][0]=='2') ans = sol_nlgn(n);
	if(argv[2][0]=='3') ans = sol_n(n);
	aftertime = clock();//after calculation, clock stop
	char output[30] = "result_";
	strcat(output,argv[1]);//output file
	b = fopen(output,"w");//ouput
	fprintf(b,"%s\n",argv[1]);
	fprintf(b,"%c\n",argv[2][0]);
	fprintf(b,"%d\n",n);
	fprintf(b,"%d\n%d\n",sp,ep);
	fprintf(b,"%d\n",ans);
	fprintf(b,"%lf\n",1000*(aftertime-beforetime)/(CLOCKS_PER_SEC));
	fclose(a);//close of file
	return 0;
}
