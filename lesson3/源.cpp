//迷宫问题
#include<iostream>
#include<iomanip>

using namespace std;

#define boundary 1 //地图边界宽度

class node;//先标志一下

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

	node* next();//调整位置写到外面
};

//地图
node* map[102][102] = {};
//0表示路径，1表示入口，2表示出口，-1表示边界

bool node::shot() {//只是先看看当前位置能不能通过
	if (map[this->y][this->x]->state != -1 && map[this->y][this->x]->visited == 0)
		return true;
	return false;
}

node* node::next() {
	switch (dir) {
	case 1:
		return map[this->y][this->x + 1];
		break;
	case 2:
		return map[this->y + 1][this->x];
		break;
	case 3:
		return map[this->y][this->x - 1];
		break;
	case 4:
		return map[this->y - 1][this->x];
		break;
	default:
		return nullptr;
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
	void init() {
		base = (node*)malloc(1000 * sizeof(node));//假设100的大小
		if (!base)exit(OVERFLOW);
		top = base;
		size = 1000;
	}
	void destroy() {
		if (!base)return;
		delete base;
		return;
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
};

stack* arry[100];//建立栈，用来保存序列

stack* sq = new stack;//临时栈

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

//核心功能函数
static void fun(node* start, node* end) {
	sq->init();//初始化一个空栈sq
	node* curpos = start;//当前位置
	node* e = new node();//这个用来返回弹出值
	do {
		//如果当前位置可以通过
		if (curpos->shot()) {//当前位置可以走
			curpos->visited = 1;//留下足迹
			//入栈+改一下状态
			curpos->an = true;
			sq->push(*curpos);
			//如果找到
			if (curpos->state == 2) {//说明找到
				return;
			}
			curpos = curpos->next();
		}
		else {//当前位置不能通过
			if (!sq->isempty()) {//栈不空，出栈
				e->an = false;
				sq->pop(*e);

				while (e->dir >= 4 && !sq->isempty()) {
					e->state = -2;

					e->an = false;
					sq->pop(*e);//再看下一个行不行
				}//一直到行的那个
				if (e->dir < 4) {
					e->dir++;
					map[e->y][e->x]->dir++;//保持同步

					e->an = true;
					sq->push(*e);

					curpos = e->next();
				}
			}
		}
	} while (!sq->isempty());//栈不空
}

int main() {
	int l;
	int w;
	cin >> l >> w;
	setmap(l, w);//初始化地图
	printmap(l, w);
	//输入障碍的坐标
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
	fun(map[1][1], map[y][x]);
	sq->printstack();
	printmap(l, w);
	return 0;
}