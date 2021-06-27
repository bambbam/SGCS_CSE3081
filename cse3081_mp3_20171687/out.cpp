#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>
#include <cstdlib>
using namespace std;
vector<char> in;
typedef struct _node {
	char symbol;
	int freg;
	struct _node* left;
	struct _node* right;
}node;//data structure
struct cmp{
bool operator()(node* t, node* u) {
	return t->freg > u->freg;
	}
};//민힙을 만들기 위함.
FILE* out;
priority_queue<node*,vector<node*>,cmp> pq;
char* outputname;
vector<int> v[300];//0~256까지로 나타낼 수 있는 바이트중 어떤 바이트가 허프만 코드에서 어떤 비트스트링과 대응되는가

int frequency[600];//등장 빈도 수를 나타낸다.

int mul, bits_to_byte;//비트를 연산하여 파일에 넣기 위해 필요한 변수들이다.
void make_tree(node* root) {//트리를 파일에 넣기 위해 만든 함수이다.
	int ret = 0;
	if (root->left == NULL && root->right == NULL) {//리프노드일 때
		ret = mul;//리프노드일때에만 1이기 때문에 이때에만 bits_to_byte에 mul을 더한다.
		bits_to_byte += ret;
		mul /= 2;
		if (mul == 0) {//8번째 비트를 보고 있을 때
			fprintf(out, "%c", bits_to_byte);//파일에 적어준다.
			mul = 128;
			bits_to_byte = 0;
		}
		int t = root->symbol;//리프노드이기 때문에 symbol 또한 넣어준다.
		if (t<0) t += 256;
		int tt = 128;//앞에 비트부터 골라내기 위해 선언해주었다.
 		for (int i = 0; i<8; i++) {
			int k = t / tt;
			bits_to_byte += k * mul;
			mul /= 2;
			t %= tt;
			tt /= 2;
			if (mul == 0) { 
				fprintf(out, "%c", bits_to_byte);
				mul = 128;
				bits_to_byte = 0;
			}
		}
	}
	else mul /= 2;
	if (mul == 0) {
		char c = bits_to_byte;
		fprintf(out, "%c", c);
		mul = 128;
		bits_to_byte = 0;
	}
	if (root->left) make_tree(root->left);//dfs를 돌며 모든 작업을 해준다.
	if (root->right) make_tree(root->right);
}
vector<int> temp;//stack과 같은 역할을 하였다. 모든 바이트들의 비트스트링을 관리하는 v가 vector기 때문에 같이 vector로 적어주었다.
void dfs(node* root, int depth) {
	if (root->left == NULL && root->right == NULL) {//리프노드일때
		int t = root->symbol;
		if(t<0) t+=256;
		v[t] = temp;//벡터는 강한 복사이다. 
	}
	else {
		temp.push_back(0);//왼쪽은 0, 오른쪽은 1
		dfs(root->left, depth + 1);
		temp.pop_back();
		temp.push_back(1);
		dfs(root->right, depth + 1);
		temp.pop_back();
	}
}
void encoding_huffman(const char* argv) {
	FILE* input = fopen(argv, "r");
	if(!input){
		printf("file open error!!");
		return;
	}//만약 파일이 열리지 않는다면 에러를 출력한다.
	int len = strlen(argv);
	outputname = (char*)calloc(sizeof(char), len + 4);
	for (int i = 0; i<len; i++) {
		outputname[i] = argv[i];
	}
	outputname[len] = '.';
	outputname[len + 1] = 'z';
	outputname[len + 2] = 'z';
	outputname[len + 3] = '\0';
	out = fopen(outputname, "w");//output file을 열어준다.
	char c;
	while (fscanf(input,"%c", &c) != EOF) {
		in.push_back(c);
		int t = c;
		if (t<0) t += 256;
		frequency[t]++;
	}//입력받을 데이터를 받아온다.
	if(in.size()==0){
		return;
	}//만약 데이터가 아무것도 없을 경우 아무일도 하지 않는 것이 이득이라 판단하였다.
	for (int i = 0; i < 256; i++) {
		if (frequency[i] != 0) {//십진수로 나타내었을 때 i 번째 바이트의 값을 가지는 바이트가 한번 이상 등장하였다.
			int t = i < 128 ? i : i - 256;
			node* temp = (node*)calloc(sizeof(node), 1);
			temp->left = NULL;
			temp->right = NULL;
			temp->freg = frequency[i];
			temp->symbol = t;
			pq.push(temp);//노드를 만들고 그것을 pq에 넣는다.
		}
	}
	while (pq.size() > 1) {
		node* x = pq.top(); pq.pop();
		node* y = pq.top(); pq.pop();
		node* z = (node*)calloc(sizeof(node), 1);
		z->left = x;
		z->right = y;
		z->freg = x->freg + y->freg;
		z->symbol = 0;
		pq.push(z);
	}
	node* root = pq.top(); pq.pop();
	//huffman tree가 구성되고 root가 root 변수에 들어간다.
	bits_to_byte = 0;
	mul = 128;//초기화
	if(root->right==NULL&&root->left==NULL){//만약 데어타가 하나도 없는 경우 새로운 노드의 왼쪽에 현재 있는 노드를 붙여주면 decoding 하는 과정에서의 에러를 없앨 수 있다.
		node* newroot = (node*)calloc(sizeof(node),1);
		newroot->left = NULL;
		newroot->right = NULL;
		newroot->symbol = root->symbol;
		root->left = newroot;
		root->right = (node*)calloc(sizeof(node),1);
		root->right->symbol = 1;
		root->right->left=  NULL;
		root->right->right = NULL;
	}
	make_tree(root);//데이터에 트리를 적어준다.
	dfs(root, 0);
	for (int i = 0; i<in.size(); i++) {
		char c = in[i];
		int t = c;
		if(t<0) t+=256;
		for (int j : v[t]) {
			bits_to_byte += mul * j;
			mul /= 2;
			if (mul == 0) {
				mul = 128;
				fprintf(out, "%c", bits_to_byte);
				bits_to_byte = 0;
			}
		}
	}//데이터에 파일 정보를 저장한다.
	int remainder;
	int kkk = 1;
	for (remainder = 1; remainder <= 8; remainder++) {
		if (kkk == mul) break;
		kkk *= 2;
	}
	if (mul != 128) {
		fprintf(out, "%c", bits_to_byte);
		fprintf(out, "%c", remainder);
	}
	else fprintf(out,"%c",remainder);

	//마지막 바이트에서 쓸모 있는 비트 수를 적어둔다.
}
//int tempidx;
FILE* input;
int nowc;
int rc1, rc2;

char get_char() {//트리를 재구성하는 과정에서 현재 비트값이 1일때 다음 8개의 바이트를 받아 그것의 값을 구한다.
	char ret = 0;
	int tt = 128;
	for (int i = 0; i < 8; i++) {
		int t = nowc / mul;
		ret += tt * t;
		nowc %= mul;
		mul /= 2; tt /= 2;
		if (mul == 0) {
			char c; fscanf(input, "%c", &c);
			int t = c;
			nowc = t < 0 ? t + 256 : t;
			mul = 128;
		}
	}
	return ret;
}

node* remake_tree() {//트리를 재구성한다.
	node* ret = (node*)calloc(1,sizeof(node));
	int t = nowc / mul;
	nowc %= mul;
	mul /= 2;
	if (mul == 0) {
		char c; fscanf(input, "%c", &c);
		int temp = c;
		nowc = temp < 0 ? temp + 256 : temp;
		mul = 128;
	}
	if (t == 0) {//만약 현재 비트가 0이라면 노드의 왼쪽에는 첫번째 재귀, 오른쪽에는 두번째 재귀의 ret값을 적는다.
		ret->left = remake_tree();
		ret->right = remake_tree();
	}
	else {
		char c = get_char();
		ret->symbol = c;
		ret->right = NULL;
		ret->left = NULL;//c를 값으로 하는 노드를 하나 만들어서 return해준다.
	}
	return ret;
}
bool isend = false;
void decode(node* root) {//트리를 가지고 값들을 재구성하는 단계이다.
	if (root->left == NULL && root->right == NULL) {
		fprintf(out, "%c", root->symbol);
		return;
	}//만약 리프라면 symbol을 출력해준다.
	int t = nowc / mul;
	nowc %= mul;
	mul /= 2;
	if (mul == 0) {
		char c;
		int iseof = fscanf(input,"%c",&c);
		int temp = c;
		if(temp<0) temp+=256;
		nowc = rc1;
		rc1 = rc2;
		rc2 = temp;		
		if (iseof == EOF) {
			isend = true;//이것은 뒤에서 두번째 바이트를 따로 관리해주기 위함이다.
		}
		mul = 128;
	}
	if (t == 0) {//0이면 왼쪽 1이면 오른쪽
		decode(root->left);
	}
	else decode(root->right);
}

void decoding_huffman(const char* argv) {
	input = fopen(argv, "r");
	if(!input){
		printf("file open error!!");
		return;
	}
	//만약 파일이 열리지 않는다면 에러메세지를 띄운다.
	int len = strlen(argv);
	char* outputname = (char*)calloc(sizeof(char), len + 4);
	for (int i = 0; i < len; i++) outputname[i] = argv[i];
	outputname[len] = '.';
	outputname[len + 1] = 'y';
	outputname[len + 2] = 'y';
	outputname[len + 3] = '\0';
	out = fopen(outputname, "w");
	//output file open
	mul = 128;
	char c;
	int iseof = fscanf(input, "%c", &c);
	if(iseof==EOF) return;
	nowc = c;
	if (nowc < 0) nowc += 256;
	node* root = remake_tree();
	/*tempidx = -1;
	while (fscanf(input, "%c" , &c) != EOF) {
		todecode.push_back(c);
	}*/
	fscanf(input,"%c",&c);
	rc1 = c;
	if(rc1<0) rc1 += 256;
	iseof = fscanf(input,"%c",&c);
	rc2 = c;
	if(rc2<0) rc2 += 256;
	if(iseof==EOF) isend = true;
	//if(todecode.size()==1) isend = true;
	while (1) {
		if(!isend) decode(root);
		if (isend == true) {
			int remainder = rc1;
			int kkk = 1 << ((remainder+7) % 8);
			if(kkk==128){
				decode(root);
			}
			while(mul!=kkk){//만약 마지막에서 두번째 노드라면 비트가 쓸모 없어질 때까지 읽는다.
				decode(root);
			}
			break;
		}
	}
}

int main(int argc, char* argv[]) {
	if(argc!=3){
		printf("input error!!");
		return 0;
	}
	if(strcmp(argv[1],"-c")==0) encoding_huffman(argv[2]);
	else if(strcmp(argv[1],"-d")==0) decoding_huffman(argv[2]);
	else{
		printf("input error!!");
		return 0;
	}
	return 0;
}