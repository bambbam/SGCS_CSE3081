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
			j--;//tmp�� �� ���� �����Ѵ�.
		}
		arr[j] = tmp;//�װ��� tmp�� �ִ´�.

	}//�̰��� 1~n���� ����Ѵ�. i��°�� �� i-1������ ������ �Ǿ� �ִ�.

}



int partition(vector<int>& arr, int s, int e, int piv) {

	swap(arr[piv], arr[e]);//pivot������ ����� ���� e�� �־�д�.

	int x = s;

	for (int i = s; i < e; i++) {

		if (arr[i] < arr[e]) {

			swap(arr[i], arr[x]);

			x++;

		}

	}//�� ���� �������� pivoting�� ���ش�.

	swap(arr[x], arr[e]);//piovt�� �������� ������ pivot������ �۰�, �������� pivot������ ũ�� �ٲ��.

	return x;

}





void quick_sort(vector<int>& arr, int s, int e) {

	if (e - s > 0) {//end�� s���� Ŭ ����

		int pivot = partition(arr, s, e, e);//pivot ������ arr[pivot]������ �۰�, �������� arr[pivot]���� ũ�� �����ȴ�.

		quick_sort(arr, s, pivot - 1);//����, pivot, ������ ����� ũ����̹Ƿ�, ���� ����� �ٽ� �������ش�.

		quick_sort(arr, pivot + 1, e);//������ ����� �������ش�.

	}

}



void heap_adjust(vector<int>& arr, int root, int e) {//root�Ʒ��� min-heap�� ������ ��, root�� ���� �����ϴ����� �����ϴ� �Լ�

	int child = 2 * root;

	int tmp = arr[root];//�θ��� �� �������� �� ���� ���� ���� �ϴ����� ������ ���̴�.

	while (child <= e) {

		if (child < e&&arr[child] > arr[child + 1]) child++;//�ڽ� �� �� ���� ���� ���ϱ� �����̴�.

		if (tmp < arr[child]) break;//���� tmp<arr[child]��� child�� �θ� �θ��� ���� ���� �ſ� �����ϴ�.

		else {

			arr[child / 2] = arr[child];//���� �׷��� �ʴٸ�, �������� �ʴٴ� �ǹ��̱� ������, ��ĭ�� �ø��� �� ���� �ڽ��� ���ش�.

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

	}//n/2 ���� 1���� �������� heap�� �������ָ�, O(n)���� heap�� ������ �� �ִ�.

	int t = n;

	kk = s;

	for (int i = 1; i <= n; i++) { // heap�� �־��� ���ҵ��� �ϳ��� pop�ϸ�, ������ �Ǿ������� Ȯ���� �� �ִ�.

		arr[kk++] = v[1];

		swap(v[1], v[t]);

		heap_adjust(v, 1, --t);

	}



}





void optimized_quick(vector<int>& arr, int s, int e, int depth_limit) {

	int mid;

	while (e - s > 32) {//�ִ� ũ�Ⱑ 32�� ������ �ȵ� ������� ���� ���̴�. ������ ����� ���ҵ��� ������ �ȵǾ� ������,

		if (depth_limit == 0) {//partion�� ���� ������ ������� ��Ұ��迡 ���� ���� ����� ����, ū ����� ���������� ���� �ȴ�.

			heap_sort(arr, s, e);//���� ���� ������ 0�� �ɶ����� optimized quick�� ���Դٸ�, pivoting�� ����� ���� �ʾ��� ���ɼ��� �����Ƿ�        

			break;//heap sort�� nlogn �ð����⵵�� �������ش�.

		}

		else {

			depth_limit -= 1;

			mid = (s + e) / 2;

			int a = rand()%(e-s+1)+s;

			int b = rand()%(e-s+1)+s;

			int c = rand()%(e-s+1)+s;

			if (arr[a] > arr[b]) swap(arr[a], arr[b]);

			if (arr[a] > arr[c]) swap(arr[a], arr[c]);

			if (arr[b] > arr[c]) swap(arr[b], arr[c]);//3���� rand�� �� median ���� ���� ������ pivot�� ���ɼ��� ���߿� ���� ����.

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

	vv.resize(max_val - min_val + 2);//�ּҰ�~�ִ밪���� ������ �� �ִ� �����̴�.

	before = clock();

	for (int i = 0; i < n; i++) {

		vv[arr[i] - min_val]++;//�ּҰ��� ������ �� �� �ֱ� ������ ó���� ���ش�.

	}

	int s = 0;

	for (int i = 0; i <= max_val - min_val + 1; i++) {

		if (vv[i] != 0) {

			for (int j = 0; j < vv[i]; j++) {

				arr[s++] = i + min_val;//ó���� ���� ���� �״�� ������ ������ �Ǿ��ִ�.

			}

		}

	}

	vv.clear();

}

void mysort(vector<int>& v, int s, int e, int mmax, int mmin) {

	int n = e - s + 1;

	double kk = n*(log2(n));

	if (mmax - mmin < kk&&mmax - mmin <= 10000000) {//�������� nlogn�̳��̸�, �ʹ� ���� �޸𸮸� ������ �ʴ´�.

		counting_sort(v, mmin, mmax, n);

	}

	else {

		optimized_quick(v, 0, n - 1, 2*ceil(log2(n)));

		insertion_sort(v, 0, n - 1);//�ִ� ũ�� 32�� ������ ������ �Ǿ��ֱ� ������, ������ ���������ش�.

	   //������ ������� ũ��� ����̱� ������, ���*n������ �ð��� �Ҹ�ȴ�.

	}

}

int main(int argc, char* argv[]) {

	srand(time(NULL));

	FILE* fp = fopen(argv[1], "r");

	int n; fscanf(fp, "%d", &n);

	int mmax = -2147483648;//counting sort�� �ϱ� ���� �ִ밪

	int mmin = 2147483647;//counting sort�� �ϱ� ���� �ּҰ�

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

