#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <map>
#include <set>
#include <stack>
#include <list>
#include <vector>
#include <queue>

using namespace std;

#define PI acos(-1)
#define MP make_pair
#define PB push_back
#define VI vector <int>
#define PII pair <int, int>
#define LL long long
#define SET(v,i) memset(v, i, sizeof(v))
#define FOR(i,a,b) for (int i = (a); i <= (b); i++)
#define FORD(i,a,b) for (int i = (a); i >= (b); i--)
#define FORN(i,a,b) for (int i = (a); i < (b); i++)
#define DOWN(i,a,b) for (int i = (a); i > (b); i--)
#define FIT(it,v) for (typeof(v.begin()) it = v.begin(); it != v.end(); it++)
#define FITD(it,v) for (typeof(v.rbegin()) it = v.rbegin(); it != v.rend(); it++)
#define FREOPEN freopen("a.in", "r", stdin); freopen("a.out", "w", stdout)

#define maxn 411111

struct element {
	int u, v, i;
};

char S[maxn], T1[maxn], T2[maxn];
element a[maxn], b[maxn];
int pos[maxn][20];
int lcp[maxn], cnt[maxn];
int N, N1, N2;

void countingSort() {//use couting sort to reduce the complexity log(n) time
	SET (cnt, 0);
	FOR (i, 1, N) 
		cnt[a[i].v] ++;
		
	FOR (i, 1, maxn - 1) cnt[i] += cnt[i - 1];
	
	FOR (i, 1, N) {
		b[cnt[a[i].v]] = a[i];
		cnt[a[i].v] --;
	}
	
	SET (cnt, 0);
	FOR (i, 1, N) 
		cnt[a[i].u] ++;
		
	FOR (i, 1, maxn - 1) cnt[i] += cnt[i - 1];
	
	FORD (i, N, 1) {
		a[cnt[b[i].u]] = b[i];
		cnt[b[i].u] --;
	}
}

void makeSuffix() {//find the subffix array and lcp informationf of the string S, lenth N.
	N++;
	S[N] = '@';
	
	FOR (i, 1, N) 
		pos[i][0] = S[i] + 1;
		
	FOR (step, 1, 18) {
		FOR (i, 1, N) {
			a[i].i = i;
			a[i].u = pos[i][step - 1];
			
			if (i + (1 << (step - 1)) <= N) 
				a[i].v = pos[i + (1 << (step - 1))][step - 1];
			else a[i].v = 0;
		}
		
		countingSort();
		
		pos[a[1].i][step] = 1;
		
		FOR (i, 2, N)
			pos[a[i].i][step] = pos[a[i - 1].i][step] + 
			( a[i].u != a[i - 1].u || a[i].v != a[i - 1].v);	
	}
}

void makeLcp() {//prepare lcp information
	FOR (i, 1, N - 1) {
		int u = a[i].i;
		int v = a[i + 1].i;
		lcp[i] = 0;
		
		FORD (j, 18, 0) 
			if (pos[u][j] == pos[v][j]) {
				u += 1 << j;
				v += 1 << j;
				lcp[i] += 1 << j;
			}
	}
	
	lcp [N] = N - a[N].i + 1;
}

long long callStr() {//calculate the number of different substring of S;
	long long res = 0;
	
	makeSuffix();
	makeLcp();

	FOR (i, 2, N)
		res += N - a[i].i - lcp[i - 1];
		
	return res;
}

int Left[maxn], Right[maxn];

long long callCommonStr() {//calculate the commong substring
//We will consider the suffix of the string A in the lexicographical order, with each suffix, we find how many prefixes of it are also sub-string of B (this can be done by concatenating two string and obtain the suffix array). However, some of these prefixes may be counted before (in the previous suffix of A), we can find this number based on the longest common prefix of two suffix.

	long long res = 0;
	
	makeSuffix();
	makeLcp();
	
	//Left[i] is the longest common prefix of the suffix a[i] with the nearest suffix to the left a[j] that a[j].i <= N1 (it's belong to the string A).
	
	if (a[1].i <= N1) Left[1] = N1 - a[1].i + 1;
	else Left[1] = 0;
	
	FOR (i, 2, N) 
	if (a[i].i <= N1)
		Left[i] = N1 - a[i].i + 1;
		else Left[i] = min(Left[i - 1], lcp[i - 1]);
	
	
	
	//Right[i] is the longest common prefix of the suffix a[i] with the nearest suffix to the right a[j] that a[j].i <= N1 (it's belong to the string A).
	if (a[N].i <= N1) Right[N] = N1 - a[N].i + 1;
	else Right[N] = 0;
	
	FORD (i, N, 2) 
	if (a[i].i <= N1)
		Right[i] = N1 - a[i].i + 1;
		else Right[i] = min(Right[i + 1], lcp[i]);
		
	FOR (i, 1, N) 
	if (a[i].i > N1) {
		int tmp = N, ok = -1;
		FORD (j, i - 1, 1) {
			tmp = min(tmp, lcp[j]);
			if (a[j].i > N1) {
				ok = 1;
				break;
			}
		}
		
		int u = max(Left[i], Right[i]);
		int v = (ok == -1)? 0: tmp;
		if (v <= u) res += (u - v);
	}
	
	return 2 * res;
}

int main() {
	long long res = 0;
	
	gets(T1);
	N1 = strlen(T1);
	FORD (i, N1, 1)
		T1[i] = T1[i - 1];
	FOR (i, 1, N1) S[i] = T1[i];
	N = N1;
	
	res += callStr();
	cout<<res<<endl;
	
	gets(T2);
	N2 = strlen(T2);
	FORD (i, N2, 1)
		T2[i] = T2[i - 1];
	FOR (i, 1, N2) S[i] = T2[i];
	N = N2;
	int temp = callStr();
	res += temp;
	cout<<temp<<"    n " <<endl;
	
	
	FOR (i, 1, N1) S[i] = T1[i];
	S[N1 + 1] = '@' - 1;
	
	FOR (i, 1, N2) S[N1 + 1 + i] = T2[i];
	N = N1 + N2 + 1;
	temp = (int)callCommonStr();
	res -= temp;
	cout<< temp <<" " << res<<endl;
	
	cout << res << endl;
	
	return 0;
}