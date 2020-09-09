#include <iostream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <stdlib.h>
#include <windows.h>


using namespace std;

int rMatrixPath(vector<vector<int> >& matrix, int r, int c,int *cnt, vector<vector<int> >& rcheck); //recursion을 이용하여 행렬 경로의 최고점을 구하는 함수 
double dpMatrixPath(vector<vector<int> >& matrix, int r, int c); //dynamic programming을 이용하여 행렬 경로의 최고점을 구하는 함수
int maximum(int x, int y, int z); //세 가지 값중 가장 큰 것을 반환하는 maximum함수
 
void dpShow(vector<vector<int> >& cache, int r, int c); //dynammic programming에서 경로를 출력하는 함수
int rshow(vector<vector<int> >& check, int r, int c); //recursion에서 경로를 출력하는 함수
LARGE_INTEGER StartCounter, EndCounter, liFrequency; // 시간 측정에 사용하기 위한 변수



int main()
{
    cout << " ====  1810929   장세희  알고리즘 입문   행렬 경로 문제 시작합니다. ==== " <<endl;
	cout << "==>>   행렬의 크기를 정하여 랜덤으로 수(-25 와 25 사이의 수)를 삽입 " << endl;
	cout << "==>>   (랜덤으로 발생한 수는 각 행을 다 채운 후 다음 행을 채운다) " << endl;
	cout << "==>>   모든 경로의 점수 중 가장 높은 점수를 구합니다. " << endl;
	cout << "==>>   오른쪽, 아래쪽, 오른쪽 아래 대각선으로만 이동할 수 있습니다. \n";
	cout << "==>>   음수를 행렬에 삽입하지 않을 경우 항상 지나는 원소 수가 많을수록 최고점이 되므로 대각선은 방문하지 않게 됩니다. " << endl;
	cout << "==>>   (오른쪽 아래 대각선을 지나는 경우 지나는 원소 수는 줄어듦.)" << endl;
	cout << "==>>   따라서 행렬 matrix에는 오른족 아래 대각선으로 가는 경로도 포함하기 위해 음수를 포함합니다. " << endl;
	int row = 0; 
	int col = 0;
	cout << "행렬(matrix)의 원하는 행의 수를 입력해주세요. ";
	cin >> row; //row는 입력받은 행의 수
	cout << "행렬(matrix)의 원하는 열의 수를 입력해주세요. ";
	cin >> col; //col은 입력받은 열의 수
	int cnt = 0; //함수의 총 호출 횟수를 저장하기 위한 변수
	vector< vector<int> > matrix(row, vector<int>(col, 0)); //입력받은 행과 열의 수로 행렬 matrix 생성
	vector<vector <int> > rcheck(row, vector<int>(col, 0)); //입력받은 행과 열의 수로 recursion에서 경로를 기록할 행렬 rcheck 생성
	
	cout << endl;
	srand((unsigned)time(NULL));
	for (int i = 0;i < row;i++) {
		for (int j = 0;j < col;j++) {
			int randnum = rand() % 50-25;// -25에서 25 사이의 랜덤 수를 발생시킨다.
			matrix[i][j] = randnum;//발생시킨 랜덤 수를 행렬 matrix에 저장시킨다.
		}
	}
	cout << "( " << row << "X" << col << " )  행렬(matrix)에 입력된 수  \n";
	for (int i = 0;i < row;i++) { //반복문을 이용하여 행렬 matrix에 입력된 수 화면에 출력
		for (int j = 0;j < col;j++) {
			cout << matrix[i][j] << "   ";
			if (j == col - 1) cout << endl;
		}
	}
	cout << endl;
	
	
	
	QueryPerformanceFrequency(&liFrequency); // 주파수(1초당 증가되는 카운트수)를 구한다. 시간측정의 초기화
	QueryPerformanceCounter(&StartCounter); // 코드 수행 전 카운트 저장

	int rAnswer = rMatrixPath(matrix, row-1, col-1, &cnt, rcheck); //rMatrixPath함수의 리턴 값, 즉 최고점을 rAnswer에 저장
	cout << "행렬의 원소 ( 1,1 )에서 ( "<< row << "," << col << " ) 으로 이동하는 모든 경로의 점수 중 가장 높은 점수 : " <<  rAnswer << " (recursion)"<< endl << endl; 
	cout << "recursion 이용 시 함수(rMatrixPath) 호출 횟수 : " << cnt << endl;
	QueryPerformanceCounter(&EndCounter); // 코드 수행 후 카운트 저장
	double rTime = (double)(EndCounter.QuadPart - StartCounter.QuadPart) / (double)liFrequency.QuadPart;
	cout << "recursion 이용 시 수행시간 = " << rTime << " 초" << endl; //측정한 수행시간 출력;
	rshow(rcheck, row - 1, col - 1); //recursion을 이용한 경로 출력 rshow함수 출력
	cout << "!" << endl<<endl<<endl;
	
	
	
	double dpTime = dpMatrixPath(matrix, row, col); //dpMatrixPath함수의 리턴 값인 dpMatrixPath의 수행시간을 dpTime에 저장
	cout << "Dynamic Programming 이용 시 수행시간 = " << dpTime << " 초" << endl << endl <<endl; //측정한 수행시간 출력

	if (rTime > dpTime) { //측정한 두 시간을 비교하여 더 빠른 것의 수행시간 출력
		cout << "Dynamic Programming을 이용하여 최고점을 찾는 것이 recursion을 이용하는 것 보다 " << rTime - dpTime << " 초 더 빠릅니다. " << endl;
	}
	else {
		cout << "recursion을 이용하여 최고점을 찾는 것이 Dynamic Programming을 이용하는 것 보다 " << dpTime - rTime << " 초 더 빠릅니다. " << endl;
	}
	
	

	
}         

int maximum(int x, int y, int z) { //세 가지 값중 가장 큰 것을 반환하는 maximum함수
	if (x > y) {
		if (x > z)
			return x;
		else
			return z;
	}
	else {
		if (y > z)
			return y;
		else
			return z;
	}
}

int rMatrixPath(vector<vector<int> >& matrix, int r,int c, int *cnt, vector<vector<int> >& rcheck ) {

	*cnt = *cnt + 1; //recursion을 이용하는 rMatrixPath의 호출 횟수를 나타내기 위해서 cnt 변수 설정, 함수가 호출될때마다 1씩 증가한다.
	
	if (r == 0 && c == 0) {
		return matrix[r][c]; //r 값과 c값이 0일 경우 행렬의 값 리턴 
	}
	else if (r == 0) { //r의 값이 0일 경우 즉, 더이상 위로 이동하지 않을 경우에는 
		rcheck[0][c] = 1; //이 경우 모든 경로는 오른쪽으로 밖에 이동하지 않는다. 그러므로 경로를 저장하는 rcheck배열에 오른쪽으로 이동을 뜻하는 1저장
		return rMatrixPath(matrix, 0, c - 1, cnt, rcheck) + matrix[r][c]; //열의 값만 1씩 감소하면서 rMatrixPath함수 호출
	}
	else if (c == 0) {  //c의 값이 0일 경우, 즉 더이상 왼쪽으로 이동하지 않을 경우에는
		rcheck[r][0] = 2;  //이 경우 모든 경로는 아래쪽으로 밖에 이동하지 않는다. 그러므로 경로를 저장하는 rcheck배열에 아래쪽으로 이동을 뜻하는 2 저장
		return rMatrixPath(matrix, r - 1, 0, cnt, rcheck) + matrix[r][c]; //행의 값만 1씩 감소하면서 rMatrixPath함수 호출
	}
	else {  //행과 열 모두 0이 아닐 경우 오른쪽, 대각선, 아래쪽 모두를 비교해봐야한다. 
		if (maximum(rMatrixPath(matrix, r - 1, c, cnt, rcheck), rMatrixPath(matrix, r, c - 1, cnt, rcheck), rMatrixPath(matrix, r - 1, c - 1, cnt, rcheck)) == rMatrixPath(matrix, r - 1, c, cnt, rcheck)) {
			rcheck[r][c] = 2;  //위쪽까지의 점수의 합이 가장 큰 경우이므로 경로는 아래쪽으로 이동이 된다. 경로를 저장하는 rcheck배열에 2저장
		}
		else if (maximum(rMatrixPath(matrix, r - 1, c, cnt, rcheck), rMatrixPath(matrix, r, c - 1, cnt, rcheck), rMatrixPath(matrix, r - 1, c - 1, cnt, rcheck)) == rMatrixPath(matrix, r , c-1, cnt, rcheck)) {
			rcheck[r][c] = 1;   //왼쪽까지의 점수의 합이 가장 큰 경우이므로 경로는 오른쪽으로 이동이 된다. 경로를 저장하는 rcheck배열에 1저장
		}
		else if (maximum(rMatrixPath(matrix, r - 1, c, cnt,rcheck), rMatrixPath(matrix, r, c - 1, cnt, rcheck), rMatrixPath(matrix, r - 1, c - 1, cnt, rcheck)) == rMatrixPath(matrix, r - 1, c - 1, cnt, rcheck)) {
			rcheck[r][c] = 3;  //왼쪽 위쪽 대각선 방향의 점수의 합이 가장 큰 경우이므로 경로는 오른쪽 아래 대각선으로 이동이 된다. 경로를 저장하는 rcheck배열에 3 저장
		}
		return maximum(rMatrixPath(matrix, r - 1, c, cnt, rcheck), rMatrixPath(matrix, r, c - 1, cnt, rcheck), rMatrixPath(matrix, r - 1, c - 1, cnt, rcheck)) + matrix[r][c]; 
		//오른쪽, 아래쪽, 오른쪽 아래 대각선 세가지를 비교한다. 그러므로 rMatrixPath함수를 왼쪽, 위쪽, 왼쪽 위 대각선으로 호출하며 maximum을 이용하여 비교하여 호출한다. 
	}


	
}

double dpMatrixPath(vector<vector<int> >& matrix, int r, int c) {

	QueryPerformanceFrequency(&liFrequency); // 주파수(1초당 증가되는 카운트수)를 구한다. 시간측정의 초기화
	QueryPerformanceCounter(&StartCounter); // 코드 수행 전 카운트 저장
	vector< vector<int> > cache(r, vector<int>(c, 0)); //dynamic programming에서 각 셀에 이르는 최고점을 저장하기 위한 행렬을 생성한다.
	
	cache[0][0] = matrix[0][0]; // 생성한 cache행렬의 첫번째 값은 무조건 행렬 matrix의 값과 같다.
	cout << "Dynamic Programming ( 1,1 ) 부터 시작 " << endl;
	for (int i = 1;i < r; i++) { //첫번째 열인 경우
		cache[i][0] = matrix[i][0] + cache[i - 1][0]; //이 경우 각 셀에 이르는 최고점은 항상 위쪽 값과 자기 자신을 더한 것이다.  
		cout << "( " << i + 1 << "," << "1 ) 까지 이르는 최고점 = 누적 점수  " << cache[i - 1][0] << "  + 자기 점수  " << matrix[i][0] << " = "<< matrix[i][0] + cache[i - 1][0] << endl;
	}
	for (int j = 1;j < c;j++) { //첫번째 행인 경우
		cache[0][j] = matrix[0][j] + cache[0][j - 1]; //이 경우 각 셀에 이르는 최고점은 항상 왼쪽 값과 자기 자신을 더한 것이다. 
		cout << "( 1,"<< j+1 << " ) 까지 이르는 최고점 = 누적 점수  " << cache[0][j-1] << "  + 자기 점수  " << matrix[0][j] << " = "<< matrix[0][j] + cache[0][j - 1] << endl;
	}
	for (int i = 1;i < r;i++){ //첫번째 행과 열이 아닌 경우 
		for (int j = 1;j < c;j++) {
			cache[i][j] = matrix[i][j] + maximum(cache[i - 1][j], cache[i][j - 1], cache[i - 1][j - 1]);
			//왼쪽, 위쪽, 왼쪽 위 대각선 세가지를 비교하여 가장 큰 값과 자기자신을 더한다. 
			cout << "( " << i+1 << "," << j + 1 << " ) 까지 이르는 최고점 = 누적 점수  " << maximum(cache[i - 1][j], cache[i][j - 1], cache[i - 1][j - 1]) << "  + 자기 점수  " << matrix[i][j] << " = "<< matrix[i][j] + maximum(cache[i - 1][j], cache[i][j - 1], cache[i - 1][j - 1]) << endl;
		}
	}
	QueryPerformanceCounter(&EndCounter); // 코드 수행 후 카운트 저장
	double dpTime = (double)(EndCounter.QuadPart - StartCounter.QuadPart) / (double)liFrequency.QuadPart;

	dpShow(cache, r-1, c-1); //dynamic programming의 경로를 출력하는 dpshow함수 호출
	
	cout << "!" << endl;

	
	cout << "Dynamic Programming (memoization)을 이용한 각 셀에 이르는 최고점을 기록한 행렬(cache)는 " << endl; 
	for (int i = 0;i < r;i++) {
		for (int j = 0;j < c;j++) {
			cout << cache[i][j] << "   ";
			if (j == c - 1) cout << endl;
		}
	} //각 셀에 이르는 최고점을 표현한 행렬 cache 출력
	
	cout << "행렬의 원소 ( 1,1 )에서 ( " << r << "," << c << " ) 으로 이동하는 모든 경로의 점수 중 가장 높은 점수 : " << cache[r - 1][c - 1] << " (DP)" << endl ;
	//cache 행렬의 마지막 셀에 저장되는 값이 결국에는 최고점이므로 마지막 셀의 값으로 최고점 출력
	
	return dpTime; //recursion과 수행시간 비교하기 위해 dpTime 리턴

	
}

void dpShow(vector<vector<int> >& cache, int r, int c) {
	//Dynamic Programming에서 이용한 각 셀에 이르는 최고점을 기록해놓은 cache 배열 이용하여 경로 출력
	if (r==0 && c==0) {
		cout << "Dynamic Programming을 이용한 최고점을 찾는 경로는 " ;
	}
	else if (c == 0) { //첫번째 열인 경우 경로는 무조건 아래쪽으로 이동, 열의 값만 1씩 감소하면서 dpShow 함수 호출
		dpShow(cache, r - 1, c);
	}
	else if (r == 0) { //첫번째 행인 경우 경로는 무조건 오른쪽으로 이동, 행의 값만 1씩 감소하면서 dpShow 함수 호출
		dpShow(cache, r, c - 1);
	}
	else { //첫번째 열과 행이 아닌 경우
		if (maximum(cache[r - 1][c], cache[r][c - 1], cache[r - 1][c - 1]) == cache[r - 1][c]) {
			dpShow(cache, r - 1, c); //위쪽의 경우가 가장 클 경우 아래쪽으로 이동하므로 행의 값 1 감소하여 dpShow 함수 호출
		}
		else if (maximum(cache[r - 1][c], cache[r][c - 1], cache[r - 1][c - 1]) == cache[r][c - 1]) {
			dpShow(cache, r, c - 1); //왼쪽의 경우가 가장 클 경우 오른쪽으로 이동하므로 열의 값 1 감소하여 dpShow 함수 호출 
		}
		else if (maximum(cache[r - 1][c], cache[r][c - 1], cache[r - 1][c - 1]) == cache[r - 1][c - 1]) {
			dpShow(cache, r - 1, c - 1); //왼쪽 위 대각선의 경우가 가장 클 경우 오른쪽 아래 대각선으로 이동하므로 행, 열의 값 1 감소하여 dpShow 함수 호출
		}
	}

	cout << "( " << r+1 << "," << c+1 << " ) - ";  //해당 좌표를 출력한다. 

	return ;
}

int rshow(vector<vector<int> >& check, int r, int c) {

	//recursion 시 지나온 경로를 저장하는 배열 check, 1일경우 오른쪽 이동, 2일 경우 아래쪽 이동, 3일 경우 오른쪽 아래 대각선 이동을 뜻한다. 
	
	if (check[r][c]==0) {
		cout << "recursion을 이용한 최고점을 찾는 경로는 ";
	}
	else if (check[r][c] == 1) { //1은 오른쪽 이동 , 열의 값 1 감소하여 rshow함수 호출
		 rshow(check, r, c - 1);
		 
	}
	else if (check[r][c] == 2) { //2는 아래쪽 이동, 행의 값 1 감소하여 rshow 함수 호출
		 rshow(check, r - 1, c);
		 
	}
	else { //3은 오른족 아래 대각선 이동, 행, 열의 값 1 감소하여 rshow 함수 호출 
		rshow(check, r - 1, c - 1);
		
	}
	
	cout << "( " << r + 1 << "," << c + 1 << " ) - "; //해당 좌표 출력
	
	return 0;
}





