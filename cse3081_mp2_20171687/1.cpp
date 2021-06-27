#include <stdio.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <time.h>
#include <cstring>
using namespace std;
double before, after;
void insertion_sort(vector<int>& arr, int s, int e) {
	int n = e - s + 1;
	int tmp, j;
	for (int i = s + 1; i <= e; i++) {
		tmp = arr[i];
		j = i;
		while (j > s && tmp < arr[j - 1]) {
			arr[j] = arr[j - 1];
			j--;//tmp가 들어갈 곳을 결정한다.
		}
		arr[j] = tmp;//그곳에 tmp를 넣는다.

	}//이것을 1~n까지 계속한다. i번째일 떄 i-1까지는 정렬이 되어 있다.

}



int partition(vector<int>& arr, int s, int e, int piv) {

	swap(arr[piv], arr[e]);//pivot값으로 사용할 것을 e에 넣어둔다.

	int x = s;

	for (int i = s; i < e; i++) {

		if (arr[i] < arr[e]) {

			swap(arr[i], arr[x]);

			x++;

		}

	}//그 값을 기준으로 pivoting을 해준다.

	swap(arr[x], arr[e]);//piovt을 기준으로 왼쪽은 pivot값보다 작게, 오른쪽은 pivot값보다 크게 바뀐다.

	return x;

}





void quick_sort(vector<int>& arr, int s, int e) {

	if (e - s > 0) {//end가 s보다 클 동안

		int pivot = partition(arr, s, e, e);//pivot 왼쪽은 arr[pivot]값보다 작게, 오른쪽은 arr[pivot]보다 크게 형성된다.

		quick_sort(arr, s, pivot - 1);//왼쪽, pivot, 오른쪽 덩어리는 크기순이므로, 왼쪽 덩어리를 다시 정렬해준다.

		quick_sort(arr, pivot + 1, e);//오른쪽 덩어리를 정렬해준다.

	}

}



void heap_adjust(vector<int>& arr, int root, int e) {//root아래는 min-heap을 만족할 때, root가 어디로 들어가야하는지를 결정하는 함수

	int child = 2 * root;

	int tmp = arr[root];//부모의 값 이제부터 이 값이 어디로 들어가야 하는지를 결정할 것이다.

	while (child <= e) {

		if (child < e&&arr[child] > arr[child + 1]) child++;//자식 중 더 작은 값과 비교하기 위함이다.

		if (tmp < arr[child]) break;//만약 tmp<arr[child]라면 child의 부모에 부모의 값이 들어가기 매우 적합하다.

		else {

			arr[child / 2] = arr[child];//만약 그렇지 않다면, 적합하지 않다는 의미이기 때문에, 한칸씩 올리고 그 밑의 자식을 봐준다.

			child *= 2;

		}

	}

	arr[child / 2] = tmp;

}



void heap_sort(vector<int>& arr, int s, int e) {

	int n = e - s + 1;

	vector<int> v(n+1);

	int kk = s;

	for (int i = 1; i <= n; i++) {

		v[i] = arr[kk++];

	}

	int k = n / 2;

	for (int i = k; i >= 1; i--) {

		heap_adjust(v, i, n);

	}//n/2 부터 1까지 역순으로 heap을 구성해주면, O(n)으로 heap을 구성할 수 있다.

	int t = n;

	kk = s;

	for (int i = 1; i <= n; i++) { // heap에 넣었던 원소들을 하나씩 pop하면, 정렬이 되어있음을 확인할 수 있다.

		arr[kk++] = v[1];

		swap(v[1], v[t]);

		heap_adjust(v, 1, --t);

	}



}





void optimized_quick(vector<int>& arr, int s, int e, int depth_limit) {

	int mid;

	while (e - s > 32) {//최대 크기가 32인 정렬이 안된 덩어리들을 만들 것이다. 각각의 덩어리의 원소들은 정렬이 안되어 있으나,

		if (depth_limit == 0) {//partion에 의해 각각의 덩어리들은 대소관계에 따라 작은 덩어리가 왼쪽, 큰 덩어리가 오른쪽으로 오게 된다.

			heap_sort(arr, s, e);//만약 깊이 제한이 0이 될때까지 optimized quick에 들어왔다면, pivoting이 제대로 되지 않았을 가능성이 높으므로        

			break;//heap sort로 nlogn 시간복잡도를 보장해준다.

		}

		else {

			depth_limit -= 1;

			mid = (s + e) / 2;

			int a = rand()%(e-s+1)+s;

			int b = rand()%(e-s+1)+s;

			int c = rand()%(e-s+1)+s;

			if (arr[a] > arr[b]) swap(arr[a], arr[b]);

			if (arr[a] > arr[c]) swap(arr[a], arr[c]);

			if (arr[b] > arr[c]) swap(arr[b], arr[c]);//3개의 rand값 중 median 값이 가장 최적의 pivot일 가능성이 셋중엔 제일 높다.

			mid = partition(arr, s, e, b);

			if (mid - s > e - mid) {

				optimized_quick(arr, s, mid - 1, depth_limit);//tail recursion

				s = mid + 1;

			}

			else {

				optimized_quick(arr, mid + 1, e, depth_limit);//tail recursion

				e = mid - 1;

			}

		}

	}

}



void counting_sort(vector<int>& arr, int min_val, int max_val, int n) {

	vector<int> vv;

	vv.resize(max_val - min_val + 2);//최소값~최대값까지 저장할 수 있는 벡터이다.

	before = clock();

	for (int i = 0; i < n; i++) {

		vv[arr[i] - min_val]++;//최소값이 음수가 될 수 있기 때문에 처리를 해준다.

	}

	int s = 0;

	for (int i = 0; i <= max_val - min_val + 1; i++) {

		if (vv[i] != 0) {

			for (int j = 0; j < vv[i]; j++) {

				arr[s++] = i + min_val;//처리를 해준 값을 그대로 뽑으면 정렬이 되어있다.

			}

		}

	}

	vv.clear();

}

void mysort(vector<int>& v, int s, int e, int mmax, int mmin) {

	int n = e - s + 1;

	double kk = n*(log2(n));

	if (mmax - mmin < kk&&mmax - mmin <= 10000000) {//이정도면 nlogn이내이며, 너무 많은 메모리를 쓰지도 않는다.

		counting_sort(v, mmin, mmax, n);

	}

	else {

		optimized_quick(v, 0, n - 1, 2*ceil(log2(n)));

		insertion_sort(v, 0, n - 1);//최대 크기 32인 덩어리들로 소팅이 되어있기 때문에, 정렬을 마무리해준다.

	   //각각의 덩어리들의 크기는 상수이기 때문에, 상수*n정도의 시간이 소모된다.

	}

}

int main(int argc, char* argv[]) {

	srand(time(NULL));

	FILE* fp = fopen(argv[1], "r");

	int n; fscanf(fp, "%d", &n);

	int mmax = -2147483648;//counting sort를 하기 위한 최대값

	int mmin = 2147483647;//counting sort를 하기 위한 최소값

	vector<int> v(n);

	for (int i = 0; i < n; i++) {

		fscanf(fp, "%d", &v[i]);

		mmax = max(v[i], mmax);

		mmin = min(v[i], mmin);

	}

	before = clock();

	if (argv[2][0]=='1') insertion_sort(v, 0, n-1);

	if (argv[2][0]=='2') quick_sort(v, 0, n-1);

	if (argv[2][0]=='3') heap_sort(v, 0, n-1);

	if (argv[2][0]=='4') mysort(v, 0, n-1, mmax, mmin);

	after = clock();

	char outputfilename[100] = "result_";

	if (argv[2][0]=='1') outputfilename[7] = '1';

	if (argv[2][0]=='2') outputfilename[7] = '2';

	if (argv[2][0]=='3') outputfilename[7] = '3';

	if (argv[2][0]=='4') outputfilename[7] = '4';

	outputfilename[8] = '_';

	outputfilename[9] = '\0';

	strcat(outputfilename, argv[1]);

	FILE* output = fopen(outputfilename, "w");

	fprintf(output, "%s\n%c\n%d\n", argv[1], argv[2][0], n);

	fprintf(output, "%lf\n", (after-before)/CLOCKS_PER_SEC);

	for (int i = 0; i < n; i++) {

		fprintf(output, "%d ", v[i]);

	}

	return 0;

}

