//迷宫问题
#include<iostream>
#include<iomanip>

using namespace std;

#define boundary 1 //地图边界宽度

class node;//先标志一下

int dirx[5] = { 0,1,0,-1,0 };

int diry[5] = { 0,0,1,0,-1 };

//→↓←↑
string s[4] = { "\u2192","\u2193","\u2190","\u2191" };

class node {
public:
	//x, y坐标
	int x;
	int y;
	int state;//表示状态：0表示路径，-1表示边界和障碍，-2表示死胡同，1表示入口，2表示出口
	int dir = 1;//表示方向
	int visited = 0;//表示是否访问，有用
	bool an = false;

	node() {
		x = 0;
		y = 0;
		state = 0;
		dir = 1;//方向默认从东开始
		visited = 0;//默认没有访问
		an = false;
	}

	void set(int i, int j, int s) {
		x = i;
		y = j;
		state = s;
	}

	void change_state(int s) {
		state = s;
	}

	bool shot();

	void next(int& x, int& y);//调整位置写到外面
};

//地图
node* map[1020][1020] = {};
//0表示路径，1表示入口，2表示出口，-1表示边界

int l;
int w;//表示地图的长和宽
int sum = 0;//表示解数

bool node::shot() {//只是先看看当前位置能不能通过
	if (map[this->y][this->x]->state != -1 && map[this->y][this->x]->visited == 0)
		return true;
	return false;
}

void node::next(int& x, int& y) {
	switch (dir) {
	case 1:
		x++;
		break;
	case 2:
		y++;
		break;
	case 3:
		x--;
		break;
	case 4:
		y--;
		break;
	default:
		return;
		break;
	}
}

//使用一个栈来保存，走过的节点序列
class stack {//练习顺序栈
public:
	node* top;
	node* base;
	int size;

	//初始化栈空间
	stack() {
		base = (node*)malloc(1000 * sizeof(node));//假设1000的大小
		if (!base)return;
		top = base;
		size = 1000;
	}

	void destroy() {
		if (!base)return;
		delete base;
		return;
	}

	//清空栈
	void clear() {
		while (!this->isempty()) {
			this->pop();
		}
	}

	//判断空栈，有问题
	bool isempty() {
		return base == top;
	}

	void push(node e) {
		//不做容量判断了
		*top++ = e;
		//需要：改变一下状态
		return;
	}

	//不返回，弹出
	void pop() {
		if (isempty())return;
		--top;
		return;
	}

	void pop(node& e) {//用e返回那个弹出的值
		if (base == top)return;
		e = *--top;
		return;
	}

	void printstack() {
		node* e = this->base;
		while (e != this->top) {
			cout << "(" << e->x << "," << e->y << "," << s[e->dir - 1] << ")" << endl;
			e++;
		}
	}
	//拷贝构造函数
	void operator=(const stack &s) {
		node* e = s.base;
		while (e != s.top) {
			this->push(*e);
			e++;
		}
	}
};

stack arry[100];//建立栈，用来保存序列

stack sq;//临时栈

//初始化地图
static void setmap(int length, int width) {
	//初始化边界
	//这个坐标是按照原来的坐标
	for (int i = 0; i < length + boundary * 2; i++) {
		map[0][i] = new node();
		map[0][i]->set(i, 0, -1);
		map[width + 1][i] = new node();
		map[width + 1][i]->set(i, width + 1, -1);
	}
	for (int i = 0; i < width + boundary * 2; i++) {
		map[i][0] = new node();
		map[i][0]->set(0, i, -1);
		map[i][length + 1] = new node();
		map[i][length + 1]->set(length + 1, i, -1);
	}

	//初始化路径
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < length; j++) {
			map[i + 1][j + 1] = new node();
			map[i + 1][j + 1]->set(j + 1, i + 1, 0);
		}
	}

	map[1][1]->set(1, 1, 1);//设置入口
	map[width][length]->set(length, width, 2);//出口
}

//打印地图
static void printmap(int length, int width) {
	//打印测试
	for (int i = 0; i < width + 2; i++) {
		for (int j = 0; j < length + 2; j++) {
			switch (map[i][j]->state) {
			case -1:
				cout << setw(2) << "#";
				break;
			case 0:
				if (map[i][j]->an) {
					cout << setw(2) << s[map[i][j]->dir - 1];//路径解
				}
				else {
					cout << setw(2) << " ";//路径
				}
				break;
			case 1:
				cout << setw(2) << "*";
				break;
			case 2:
				cout << setw(2) << "@";
				break;
			default:
				break;
			}
		}
		cout << endl;
	}
}

//打印解！
static void printanswer(int length, int width,stack& sq) {
	node* temp ;
	bool flag=false;//做一下标记
	for (int i = 0; i < width + 2; i++) {
		for (int j = 0; j < length + 2; j++) {
			switch (map[i][j]->state) {
			case -1:
				cout << setw(2) << "#";
				break;
			case 0:
				temp = sq.base;//初始条件，，就和for循环一样
				//检索栈！
				while (temp != sq.top) {
					if (temp->x == j && temp->y == i) {
						cout << setw(2) <<"\033[1;31m" << s[temp->dir - 1]<<"\033[0m";
						flag = true;
						temp = sq.base;//从头开始检索
						break;
					}
					temp++;
				}
				if (!flag)cout << setw(2) << " ";
				flag = false;
				break;
			case 1:
				cout << setw(2) << "*";
				break;
			case 2:
				cout << setw(2) << "@";
				break;
			default:
				break;
			}
		}
		cout << endl;
	}
}

static void walk(int x, int y) {
	//结束条件
	if (x == l && y == w) {
		for (int i = 0; i < 100; i++) {
			if (arry[i].isempty()) {
				arry[i] = sq;
				break;
			}
		}//保存到栈里面
		sum++;
		return;
	}

	//判断四个方向
	for (int i = 1; i <= 4; i++) {
		if (map[y + diry[i]][x + dirx[i]]->visited == 0 && map[y + diry[i]][x + dirx[i]]->state != -1) {//改成要去的位置
			map[y][x]->visited = 1;

			//入栈
			map[y][x]->dir = i;//不要直接改变dir的值
			sq.push(*map[y][x]);//入的是当前位置

			walk(x + dirx[i], y + diry[i]);//不能直接改x,y的值

 			map[y][x]->visited = 0;//还原位置，很重要，尝试出栈
			sq.pop();//不返回直接弹出
		}
	}
}

////核心功能函数
//static void fun(node* start, node* end) {
//	sq->init();//初始化一个空栈sq
//	node* curpos = start;//当前位置
//	node* e = new node();//这个用来返回弹出值
//	do {
//		//如果当前位置可以通过
//		if (curpos->shot()) {//当前位置可以走
//			curpos->visited = 1;//留下足迹
//			//入栈+改一下状态
//			curpos->an = true;
//			sq->push(*curpos);
//			//如果找到
//			if (curpos->state == 2) {//说明找到
//				return;
//			}
//			curpos = curpos->next();
//		}
//		else {//当前位置不能通过
//			if (!sq->isempty()) {//栈不空，出栈
//				e->an = false;
//				sq->pop(*e);
//
//				while (e->dir >= 4 && !sq->isempty()) {
//					e->state = -2;
//
//					e->an = false;
//					sq->pop(*e);//再看下一个行不行
//				}//一直到行的那个
//				if (e->dir < 4) {
//					e->dir++;
//					map[e->y][e->x]->dir++;//保持同步
//
//					e->an = true;
//					sq->push(*e);
//
//					curpos = e->next();
//				}
//			}
//		}
//	} while (!sq->isempty());//栈不空
//}

int main() {
	cout << "输入地图长宽" << endl;
	cin >> l >> w;
	setmap(l, w);//初始化地图
	printmap(l, w);
	//输入障碍的坐标
	cout << "输入障碍坐标，按q结束" << endl;
	int x;
	int y;
	while (true) {
		cin >> x >> y;
		//按非整形退出
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
			break;
		}
		map[y][x]->change_state(-1);
		printmap(l, w);
	}
	/*fun(map[1][1], map[y][x]);*/
	walk(1, 1);//从起点开始walk

	for (int i = 0; i < 100; i++) {
		if (!arry[i].isempty()) {//不空
			cout << "第" << i + 1 << "种：" << endl;
			arry[i].printstack();
			cout << endl;
			printanswer(l, w, arry[i]);
			cout << endl;
		}
	}
	cout << "总解："<<sum << endl;
	return 0;
}