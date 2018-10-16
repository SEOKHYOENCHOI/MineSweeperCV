#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp> 
#include <time.h>
using namespace std;
using namespace cv;

void mouseButtonDown(int event_t, int x, int y, int flags, void* window);
void leftMine(void * window, int to[2]);
void draw(int x, int y, void * window, int to);
void initWindow(Mat *srt, int *state, int* info, int *label, vector<Point>* label_start, int difficulty, int size);

int gameOver;
vector<Point> label_start;
int *state;
int *label;
int *info;
int numMine;

enum ButtonState {
	UNCLICKABLE, CLICKED, UNCLICKED, FLAG
};
enum ButtonInfo {
	AROUND_0 = 0 ,
	AROUND_1, AROUND_2, AROUND_3, AROUND_4,
	AROUND_5, AROUND_6, AROUND_7, AROUND_8,
	MINE, BACKGROUND,UNDEFINED
};

int main()
{
	srand((unsigned)time(NULL));
	int difficulty = 0;
	int size = 0;
	Size blockSize = Size(9, 9);
	Mat window;
	cout << "select difficulty (1~3) : ";
	while (difficulty < 1 || difficulty>3) {
		cin >> difficulty;
		if (difficulty < 1 || difficulty>3)cout << endl << "input 1~3" << endl;
		else break;
	}
	cout << "select window size (1~3) : ";
	while (size < 1 || size>3) {
		cin >> size;
		if (size < 1 || size>3)cout << endl << "input 1~3" << endl;
		else break;
	}
	switch (size) {
	case 1: {window = Mat(350, 250, CV_8UC3); break; }
	case 2: {window = Mat(570, 470, CV_8UC3); break; }
	case 3: {window = Mat(790, 690, CV_8UC3); break; }
	default: {window = Mat(350, 250, CV_8UC3); break; }
	}
	state = (int*)calloc((size * 220 + 130)*(size * 220 + 30), sizeof(int));
	label = (int*)calloc((size * 220 + 130)*(size * 220 + 30), sizeof(int));
	info = (int*)calloc((size * 220 + 130)*(size * 220 + 30), sizeof(int));
	label_start = vector<Point>();
	gameOver = 1;
	initWindow(&window, state, info, label, &label_start, difficulty, size);
	int w = window.cols;
	int h = window.rows;
	while (1) {
		imshow("MineSweeper", window);
		setMouseCallback("MineSweeper", mouseButtonDown, (void*)&window);
		int count = 0;
		for (int i = 0; i < label_start.size(); i++) {
			int x = label_start[i].x + 10;
			int y = label_start[i].y + 10;
			if (state[x + y * w] == FLAG || state[x + y * w] == CLICKED)count++;
		}
		if (count == label_start.size()) {
			cout << "Clear!" << endl;
			if (waitKey(10000) == 27)break;
			break;
		}
		if (gameOver == 0) {
			cout << "Fail!" << endl;
			if (waitKey(10000) == 27)break;
			break;
		}
		if (waitKey(1) == 27)break;
	}
	return 0;
}

void mouseButtonDown(int event_t, int x, int y, int flags, void * window)
{
	Mat button = *(Mat*)window;
	int w = button.cols;
	int h = button.rows;
	switch(event_t) {
	case CV_EVENT_LBUTTONDOWN: {
		if (state[y*w + x] == UNCLICKED || state[y*w + x] == FLAG) {
			draw(x, y, window, CLICKED);
			if (info[y*w + x] == MINE) {
				gameOver = 0;
				imshow("MineSweeper", button);
				return;
			}
			imshow("MineSweeper", button);
		}break;
	}
	case CV_EVENT_RBUTTONDOWN: {
		if (state[y*w + x] == UNCLICKED && numMine > 0) {
			draw(x, y, window, FLAG);
			int m = --numMine;
			int mineArr[2] = { 0,numMine % 10 };
			m /= 10;
			mineArr[0]=m;
			leftMine(window, mineArr);
			imshow("MineSweeper", button);
		}
		else if (state[y*w + x] == FLAG) {
			int m = ++numMine;
			int mineArr[2] = { 0,numMine % 10 };
			m /= 10;
			mineArr[0] = m;
			leftMine(window, mineArr); draw(x, y, window, UNCLICKED);
			imshow("MineSweeper", button);
		}
		break;
	}
	}

}
void leftMine(void* window, int to[2]) {
	Mat button = *(Mat*)window;
	int w = button.cols;
	int h = button.rows;
	int x = 22;
	int y = 40;
	switch (to[0]) {
	case 0: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 1: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y > 3 && i - y < 18 && j - x >= 9 && j - x <= 11) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 2: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && j - x >= 16 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && j - x >= 3 && j - x <= 5)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 3: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && j - x >= 16 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && j - x >= 16 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 4: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 11 && ((j - x >= 3 && j - x <= 5) || (j - x >= 13 && j - x <= 15))) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 13 && j - x <= 15)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 5: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && j - x >= 3 && j - x <= 5)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && j - x >= 16 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 6: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && j - x >= 3 && j - x <= 5)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 7: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 11 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 18 && j - x >= 16 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 8: {
	for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;

	}
	case 9: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && (j - x >= 16 && j - x <= 18) )
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;

	}
	}
	x += 22;
	switch (to[1]) {
	case 0: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 1: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y > 3 && i - y < 18 && j - x >= 9 && j - x <= 11) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 2: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && j - x >= 16 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && j - x >= 3 && j - x <= 5)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 3: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && j - x >= 16 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && j - x >= 16 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 4: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 11 && ((j - x >= 3 && j - x <= 5) || (j - x >= 13 && j - x <= 15))) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 13 && j - x <= 15)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 5: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && j - x >= 3 && j - x <= 5)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && j - x >= 16 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 6: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && j - x >= 3 && j - x <= 5)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 7: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 11 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 18 && j - x >= 16 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;
	}
	case 8: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;

	}
	case 9: {
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = CLICKED;
				if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 6 && i - y <= 8 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 12 && i - y <= 14 && (j - x >= 16 && j - x <= 18))
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
				{
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else {
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
		break;

	}
	}

}
void draw(int xx, int yy, void* window,int to) {

	Mat button = *(Mat*)window;
	int w = button.cols;
	int h = button.rows;
	switch (to) {
	case FLAG:{
		int x = 0;
		x=label_start[label[w*yy + xx]].x;
		int y = 0;
		y=label_start[label[w*yy + xx]].y;
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				if ((i - y <= 10) && (i - y > 1) && (j-x > 3) && (j - x + i - y) >= 13 && (j - x  <= 12)) {
					state[i*w + j] = FLAG;
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 255;
				}
				else if (i - y > 10 && i - y < 18 && j - x > 8&& j - x < 12) {
					state[i*w + j] = FLAG;
					button.at<Vec3b>(i, j)[0] = 0;
					button.at<Vec3b>(i, j)[1] = 0;
					button.at<Vec3b>(i, j)[2] = 0;
				}
				else {
					state[i*w + j] = FLAG;
					button.at<Vec3b>(i, j)[0] = 200;
					button.at<Vec3b>(i, j)[1] = 200;
					button.at<Vec3b>(i, j)[2] = 200;
				}
			}
		}
		break;
	}
	case UNCLICKED: {
		int x = 0;
		x = label_start[label[w*yy + xx]].x;
		int y = 0;
		y = label_start[label[w*yy + xx]].y;
		for (int i = y + 1; i < y + 21; i++) {
			for (int j = x + 1; j < x + 21; j++) {
				state[i*w + j] = UNCLICKED;
				button.at<Vec3b>(i, j)[0] = 180;
				button.at<Vec3b>(i, j)[1] = 180;
				button.at<Vec3b>(i, j)[2] = 180;
			}
		}
		break;
	}
	case CLICKED: {
		switch (info[label_start[label[w*yy + xx]].x + 10 + w * (label_start[label[w*yy + xx]].y + 10)]) {
		case 0: {
			int x = 0;
			x = label_start[label[w*yy + xx]].x;
			int y = 0;
			y = label_start[label[w*yy + xx]].y;
			for (int i = y + 1; i < y + 21; i++) {
				for (int j = x + 1; j < x + 21; j++) {
					state[i*w + j] = CLICKED;
					button.at<Vec3b>(i, j)[0] = 140;
					button.at<Vec3b>(i, j)[1] = 140;
					button.at<Vec3b>(i, j)[2] = 140;
				}
			}
			if( (info[w*(yy - 22) + xx] == 0|| info[w*(yy - 22) + xx] == 1)&& state[w*(yy - 22) + xx] == UNCLICKED)
				draw(xx, yy - 22, window, CLICKED);

			if ((info[w*yy + xx - 22] == 0|| info[w*yy + xx - 22] == 1 )&& state[w*yy + xx - 22] == UNCLICKED)
				draw(xx - 22, yy, window, CLICKED);

			if (xx + 22 < w&&(info[w*yy + xx + 22] == 0|| info[w*yy + xx + 22] == 1 )&& state[w*yy + xx + 22]==UNCLICKED)
				draw(xx + 22, yy, window, CLICKED);

			if ((info[w*(yy + 22) + xx ] == 0|| info[w*(yy + 22) + xx] == 1)&& state[w*(yy + 22) + xx ]==UNCLICKED)
				draw(xx , yy + 22, window, CLICKED);

			break;
		}
		case 1: {
			int x = 0;
			x = label_start[label[w*yy + xx]].x;
			int y = 0;
			y = label_start[label[w*yy + xx]].y;
			for (int i = y + 1; i < y + 21; i++) {
				for (int j = x + 1; j < x + 21; j++) {
					state[i*w + j] = CLICKED;
					if (i - y > 3 && i - y < 18 && j-x>=9&&j-x<=11) {
						button.at<Vec3b>(i, j)[0] = 255;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 0;
					}
					else {
						button.at<Vec3b>(i, j)[0] = 140;
						button.at<Vec3b>(i, j)[1] = 140;
						button.at<Vec3b>(i, j)[2] = 140;
					}
				}
			}
			break;
		}
		case 2: {
			int x = 0;
			x = label_start[label[w*yy + xx]].x;
			int y = 0;
			y = label_start[label[w*yy + xx]].y;
			for (int i = y + 1; i < y + 21; i++) {
				for (int j = x + 1; j < x + 21; j++) {
					state[i*w + j] = CLICKED;
					if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 0;
					}
					else if (i - y >= 6 && i - y <= 8 && j - x >= 16 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 0;

					}
					else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 0;

					}
					else if (i - y >= 12 && i - y <= 14 && j - x >= 3 && j - x <= 5)
					{
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 0;

					}
					else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 0;
					}
					else {
						button.at<Vec3b>(i, j)[0] = 140;
						button.at<Vec3b>(i, j)[1] = 140;
						button.at<Vec3b>(i, j)[2] = 140;
					}
				}
			}
			break;
		}
		case 3: {
			int x = 0;
			x = label_start[label[w*yy + xx]].x;
			int y = 0;
			y = label_start[label[w*yy + xx]].y;
			for (int i = y + 1; i < y + 21; i++) {
				for (int j = x + 1; j < x + 21; j++) {
					state[i*w + j] = CLICKED;
					if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
						button.at<Vec3b>(i, j)[0] = 255;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 0;
					}
					else if (i - y >= 6 && i - y <= 8 && j - x >= 16 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 255;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 0;

					}
					else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 255;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 0;

					}
					else if (i - y >= 12 && i - y <= 14 && j - x >= 16 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 255;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 0;

					}
					else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 255;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 0;
					}
					else {
						button.at<Vec3b>(i, j)[0] = 140;
						button.at<Vec3b>(i, j)[1] = 140;
						button.at<Vec3b>(i, j)[2] = 140;
					}
				}
			}
			break;
		}
		case 4: {
			int x = 0;
			x = label_start[label[w*yy + xx]].x;
			int y = 0;
			y = label_start[label[w*yy + xx]].y;
			for (int i = y + 1; i < y + 21; i++) {
				for (int j = x + 1; j < x + 21; j++) {
					state[i*w + j] = CLICKED;
					if (i - y >= 3 && i - y <= 11 &&( (j - x >= 3 && j - x <= 5)|| (j - x >= 13 && j - x <= 15))) {
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 255;
					}
					else if (i - y >= 12 && i - y <= 14 && j - x >= 3 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 255;

					}
					else if (i - y >= 15 && i - y <= 17 && j - x >= 13 && j - x <= 15)
					{
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 255;
						button.at<Vec3b>(i, j)[2] = 255;
					}
					else {
						button.at<Vec3b>(i, j)[0] = 140;
						button.at<Vec3b>(i, j)[1] = 140;
						button.at<Vec3b>(i, j)[2] = 140;
					}
				}
			}
			break;
		}
		case 5: {
			int x = 0;
			x = label_start[label[w*yy + xx]].x;
			int y = 0;
			y = label_start[label[w*yy + xx]].y; 
			for (int i = y + 1; i < y + 21; i++) {
				for (int j = x + 1; j < x + 21; j++) {
					state[i*w + j] = CLICKED;
					if(i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
						button.at<Vec3b>(i, j)[0] = 255;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 255;
					}
					else if (i - y >= 6 && i - y <= 8 && j - x >= 3 && j - x <= 5)
					{
						button.at<Vec3b>(i, j)[0] = 255;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 255;
					}
					else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 255;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 255;
					}
					else if (i - y >= 12 && i - y <= 14 && j - x >= 16 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 255;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 255;
					}
					else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 255;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 255;
					}
					else {
						button.at<Vec3b>(i, j)[0] = 140;
						button.at<Vec3b>(i, j)[1] = 140;
						button.at<Vec3b>(i, j)[2] = 140;
					}
				}
			}
			break;
		}
		case 6: {

			int x = 0;
			x = label_start[label[w*yy + xx]].x;
			int y = 0;
			y = label_start[label[w*yy + xx]].y; 
			for (int i = y + 1; i < y + 21; i++) {
				for (int j = x + 1; j < x + 21; j++) {
					state[i*w + j] = CLICKED;
					if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
						button.at<Vec3b>(i, j)[0] = 150;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 50;
					}
					else if (i - y >= 6 && i - y <= 8 && j - x >= 3 && j - x <= 5)
					{
						button.at<Vec3b>(i, j)[0] = 150;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 50;
					}
					else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 150;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 50;
					}
					else if (i - y >= 12 && i - y <= 14 &&(( j - x >= 16 && j - x <= 18)|| (j - x >= 3 && j - x <= 5)))
					{
						button.at<Vec3b>(i, j)[0] = 150;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 50;
					}
					else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 150;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 50;
					}
					else {
						button.at<Vec3b>(i, j)[0] = 140;
						button.at<Vec3b>(i, j)[1] = 140;
						button.at<Vec3b>(i, j)[2] = 140;
					}
				}
			}
			break;
		}
		case 7: {

			int x = 0;
			x = label_start[label[w*yy + xx]].x;
			int y = 0;
			y = label_start[label[w*yy + xx]].y; 
			for (int i = y + 1; i < y + 21; i++) {
				for (int j = x + 1; j < x + 21; j++) {
					state[i*w + j] = CLICKED;
					if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
						button.at<Vec3b>(i, j)[0] = 50;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 150;
					}
					else if (i - y >= 6 && i - y <= 11 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
					{
						button.at<Vec3b>(i, j)[0] = 50;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 150;
					}
					else if (i - y >= 12 && i - y <= 18 && j - x >= 16 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 50;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 150;
					}
					else {
						button.at<Vec3b>(i, j)[0] = 140;
						button.at<Vec3b>(i, j)[1] = 140;
						button.at<Vec3b>(i, j)[2] = 140;
					}
				}
			}
			break;
		}
		case 8: {

			int x = 0;
			x = label_start[label[w*yy + xx]].x;
			int y = 0;
			y = label_start[label[w*yy + xx]].y; 
			for (int i = y + 1; i < y + 21; i++) {
				for (int j = x + 1; j < x + 21; j++) {
					state[i*w + j] = CLICKED;
					if (i - y >= 3 && i - y <= 5 && j - x >= 3 && j - x <= 18) {
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 150;
						button.at<Vec3b>(i, j)[2] = 50;
					}
					else if (i - y >= 6 && i - y <= 8 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
					{
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 150;
						button.at<Vec3b>(i, j)[2] = 50;
					}
					else if (i - y >= 9 && i - y <= 11 && j - x >= 3 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 150;
						button.at<Vec3b>(i, j)[2] = 50;
					}
					else if (i - y >= 12 && i - y <= 14 && ((j - x >= 16 && j - x <= 18) || (j - x >= 3 && j - x <= 5)))
					{
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 150;
						button.at<Vec3b>(i, j)[2] = 50;
					}
					else if (i - y >= 15 && i - y <= 17 && j - x >= 3 && j - x <= 18)
					{
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 150;
						button.at<Vec3b>(i, j)[2] = 50;
					}
					else {
						button.at<Vec3b>(i, j)[0] = 140;
						button.at<Vec3b>(i, j)[1] = 140;
						button.at<Vec3b>(i, j)[2] = 140;
					}
				}
			}
			break;
		}
		case MINE: {

			int x = 0;
			x = label_start[label[w*yy + xx]].x;
			int y = 0;
			y = label_start[label[w*yy + xx]].y; 
			for (int i = y + 1; i < y + 21; i++) {
				for (int j = x + 1; j < x + 21; j++) {
					state[i*w + j] = CLICKED;
					if (i - y + j - x > 11 && i - y + j - x < 32) {
						button.at<Vec3b>(i, j)[0] = 50;
						button.at<Vec3b>(i, j)[1] = 50;
						button.at<Vec3b>(i, j)[2] = 50;
					}
					else {
						button.at<Vec3b>(i, j)[0] = 0;
						button.at<Vec3b>(i, j)[1] = 0;
						button.at<Vec3b>(i, j)[2] = 255;
					}
				}
			}
			break;
		}
		
		}
		break;
	}
	}
}

void initWindow(Mat * srt, int *state, int* info,int *label,vector<Point>* label_start, int difficulty, int size)
{
	int h = size * 220 + 130;
	int w = size * 220 + 30;
	int label_t = 0;
	
	for (int i = 0; i < srt->rows; i++) {
		for (int j = 0; j < srt->cols; j++) {
			srt->at<Vec3b>(i, j)[0] = 0;
			srt->at<Vec3b>(i, j)[1] = 0;
			srt->at<Vec3b>(i, j)[2] = 255;
		}
	}
	for (int i = 0; i < srt->rows; i++) {
		bool blockArea = false;
		for (int j = 0; j < srt->cols; j++) {
			if (i >= 110 && i < h - 23 && j >= 22 && j <= w - 23) {
				label_start->push_back(Point(j, i));
				for (int k = i; k < i + 22; k++) {
					for (int l = j; l < j + 22; l++) {
						if (l % 22 > 0 && l % 22 < 21 && k % 22 > 0 && k % 11 < 21) {
							info[k*srt->cols + l] = UNDEFINED;
							state[k*srt->cols + l] = UNCLICKED;
							label[k*srt->cols + l] = label_t;
							srt->at<Vec3b>(k, l)[0] = 180;
							srt->at<Vec3b>(k, l)[1] = 180;
							srt->at<Vec3b>(k, l)[2] = 180;
						}
						else {
							srt->at<Vec3b>(k, l)[0] = 100;
							srt->at<Vec3b>(k, l)[1] = 100;
							srt->at<Vec3b>(k, l)[2] = 100;
							info[k*srt->cols + l] = BACKGROUND;
							state[k*srt->cols + l] = UNCLICKABLE;
							label[k*srt->cols + l] = -1;
						}
					}
				}
				label_t++;
				j += 21;
				blockArea = true;
			}
			else if (i >= 110 && i < h - 23 && j == 0) {
				for (int k = i; k < i + 22; k++) {
					for (int l = j; l < j + 22; l++) {
						srt->at<Vec3b>(k, l)[0] = 200;
						srt->at<Vec3b>(k, l)[1] = 200;
						srt->at<Vec3b>(k, l)[2] = 200;
						info[k*srt->cols + l] = BACKGROUND;
						state[k*srt->cols + l] = UNCLICKABLE;
						label[k*srt->cols + l] = -1;

					}
				}
				j += 21;
			}
			else if (i >= 110 && i < h - 23 && j == w - 8) {
				for (int k = i; k < i + 22; k++) {
					for (int l = j; l < j + 8; l++) {
						srt->at<Vec3b>(k, l)[0] = 200;
						srt->at<Vec3b>(k, l)[1] = 200;
						srt->at<Vec3b>(k, l)[2] = 200;
						info[k*srt->cols + l] =  BACKGROUND;
						state[k*srt->cols + l] = UNCLICKABLE;
						label[k*srt->cols + l] = -1;

					}
				}
				j = srt->cols - 1;
			}
			else {
				state[i*srt->cols + j] = UNCLICKABLE;
				info[i*srt->cols + j] = BACKGROUND;
				label[i*srt->cols + j] = -1;
				srt->at<Vec3b>(i, j)[0] = 200;
				srt->at<Vec3b>(i, j)[1] = 200;
				srt->at<Vec3b>(i, j)[2] = 200;
			}
		}
		if (blockArea)
			i += 21;
	}
	numMine = difficulty * size * 10;
	int *mineArr = (int*)calloc(numMine, sizeof(int));
	int *flag = (int*)calloc(label_t, sizeof(int));
	for (int i = 0; i < numMine; i++) {
		int locate = rand() % label_t;
		if (flag[locate] == 0) {
			mineArr[i] = locate;
			flag[locate]++;
		}
		else i--;
	}
	std::free(flag);
	for (int i = numMine-1; i >= 0; i--) {
		for (int j = 0; i < i; j++) {
			if (mineArr[i] <= mineArr[j]) {
				int temp = mineArr[i];
				mineArr[i] = mineArr[j];
				mineArr[j] = mineArr[i];
			}
		}
	}
	for (int i = 0; i < numMine; i++) {
		int start_y = label_start->at(mineArr[i]).y;
		int start_x = label_start->at(mineArr[i]).x;
		for (int y = start_y + 1; y < start_y + 21; y++) {
			for (int x = start_x+1; x < start_x + 21; x++) {
				info[y*srt->cols + x] = MINE;
			}
		}
	}
	for (int i = 0; i < label_t; i++) {
		int start_y = label_start->at(i).y + 11;
		int start_x = label_start->at(i).x + 11;
		if (info[start_y*srt->cols + start_x] != MINE) {
			int numAroundMine = 0;
			for (int y = start_y - 22; y < start_y + 23; y += 22) {
				for (int x = start_x - 22; x < start_x + 23; x += 22) {
					if (info[y*srt->cols + x] == MINE)numAroundMine++;
				}
			}
			start_y = label_start->at(i).y;
			start_x = label_start->at(i).x;
			for (int y = start_y + 1; y < start_y + 21; y++) {
				for (int x = start_x + 1; x < start_x + 21; x++) {
					info[y*srt->cols + x] = numAroundMine;
				}
			}
		}
	}
	for (int i = 40; i <= 62; i++) {
		for (int j = 22; j < 66; j++) {
			if (i == 40 || i == 62||j==22||j==43) {
				srt->at<Vec3b>(i, j)[0] = 200;
				srt->at<Vec3b>(i, j)[1] = 200;
				srt->at<Vec3b>(i, j)[2] = 200;
			}
			else {
				srt->at<Vec3b>(i, j)[0] = 0;
				srt->at<Vec3b>(i, j)[1] = 0;
				srt->at<Vec3b>(i, j)[2] = 0;

			}
		}
	}
	int m = numMine;
	int mineArray[2] = { 0,numMine % 10 };
	m /= 10;
	mineArray[0] = m;
	leftMine((void*)srt, mineArray);
	std::free(mineArr);
}