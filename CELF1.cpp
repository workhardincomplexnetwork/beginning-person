#include "Utility.h"
#include "Graph.h"
//#include "MemoryUsage.h"
#include<algorithm>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<queue>
#include<functional>

#define S_STATE 0  //未激活节点
#define I_STATE 1  //当前轮激活节点
#define SI_STATE 2 //下一轮激活的节点
#define R_STATE 3  //之前激活的节点
#define REDUND 10

/*

int main(int argc, char** argv)
主函数中的argc代表的是参数的数量，至少为1（argv[0]即.exe文件的路径)。argv为指针表示的参数,argv[0]表示第一个参数，argv[1]表示第二个参数，以此类推。

命令行参数在程序开始运行的时候传递给程序。

命令行参数作用大致有三方面：1.确定信息的来源（比如输入文件的地址）  2.确定信息的终点（比如输出的地址）  3.切换程序的执行

argc will have been set to the count of the number of strings that（argc将被设置为该字符串数的计数 ）
argv will point to, and argv will have been set to point to an array of pointers to the individual strings（argv将指向，而argv将被设置为指向指向单个字符串的指针数组 ）
argv[0] will point to the program name string, what ever that is,（会指向程序名称字符串，不管它是什么， ）
argv[1] will point to the first argument string,
argv[2] will point to the second argument string, and so on, with
argv[argc-1] pointing to the last argument string, and
argv[argc] pointing at a NULL pointer（指向一个空指针）

*/


class Pair {
public:
    int key;
	float value;
	Pair(){}
	Pair(int key, float value) :key(key), value(value) {};
};



using namespace std;

void parseArg(int argn, char ** argv);
void run(Graph *g, string data, int k);

float mc_influence(Graph *g, int *seed_arr, int k);


// 合并两个有序数组为一个有序数组
void merge(Pair arr[], Pair left[], int leftSize, Pair right[], int rightSize) {
    int i = 0, j = 0, k = 0;

    while (i < leftSize && j < rightSize) {
        if (left[i].value >= right[j].value) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < leftSize) {
        arr[k++] = left[i++];
    }

    while (j < rightSize) {
        arr[k++] = right[j++];
    }
}

// 归并排序
void mergeSort(Pair arr[], int size) {
    if (size <= 1) {
        return;
    }

    int mid = size / 2;
    Pair left[mid], right[size - mid];

    for (int i = 0; i < mid; i++) {
        left[i] = arr[i];
    }

    for (int i = mid; i < size; i++) {
        right[i - mid] = arr[i];
    }

    mergeSort(left, mid);
    mergeSort(right, size - mid);
    merge(arr, left, mid, right, size - mid);
}
//Graph::Graph(string data);

/*

void main(int argc,char **argv)

涉及到数组和指针的关系问题了

在C语言里，要想定义一个字符串，有两种办法：

1、定义一个字符数组：char a[] = "hello"  2、定义一个字符指针：char *b = "hello"

a 和 b 中存储的都是首字符的地址。我们可以通过 a[2] 得到 “l” ，而且，也可以通过 p[2] 得到 “l”。

printf(a) 和 printf(b) 是一样的。在打印时，printf()打印指向的这个字符，如果后面仍有字符，就挪动指针继续打印，直到\0。

事实上，在通过a[2]取得元素值时，其内部就是通过指针方法得到的。

而两者的不同之处在于：

a 是一个常量，存储的是第一个字符的地址，是不可变的。比如 a++就是错的。

b 是个指针变量，存储第一个字符的地址，也就是字符串的地址，它是可变的，可以b++，还可通过 *（b+2） 得到 “l”。

*/
int main(int argn, char ** argv)
{

	cout << "Program Start at: " << currentTimestampStr() << endl; //currentTimestampStr()返回当前时间
	cout << "Arguments: ";
//  cout << argv[0];
	for(int i = 0; i < argn; i++){                                 //输出exe文件路径
		cout << argv[i]<<" ";
      //cout << argn ;
	}
	cout << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
    parseArg( argn, argv );
    cout << "--------------------------------------------------------------------------------" << endl;
    cout<<"Program Ends Successfully at: " << currentTimestampStr() << endl;
    return 0;
}

void parseArg(int argn, char ** argv)
{
    //string data="F:\\vscode_exp\\IM_DC\\networks\\karate-int.txt"; // the path of the dataset
    string data="F:\\vscode_exp\\IM_DC\\networks\\netscience-int.txt";
    //string data="F:\\vscode_exp\\IM_DC\\networks\\email_1133_5451-int.txt";
    //string data="F:\\vscode_exp\\IM_DC\\networks\\blog-int.txt";
    int k;  //the # of seeds to be found
    cout<<"the initial set of seeds to be found: ";
    cin>>k;

    for(int i=0; i<argn; i++)                    //什么作用？
    {
        //cout<<string("-data")<<endl;
        //cout<<data<<endl;
        if(argv[i] == string("-data")){
        	data=string(argv[i+1]);
//          cout<<endl<<"helloworld"<<endl;
        }
        if(argv[i] == string("-k")){
        	k=atoi(argv[i+1]);//atoi将字符串类型转化为整数类型
        	//data=string(argv[i+1]);
        }
        //cout<<argv[0]<<endl;;
    }
    if (data=="")
        ExitMessage("argument data missing");
	if(k == 0)
		ExitMessage("argument k missing");
    Graph *g = new Graph(data);
	cout << "graph " << data << " was built successfully!" << endl;
	run(g, data, k);
}

/// The Greedy algorithm

void run(Graph *g, string data, int k){
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << "Start greedy-CELF algorithm" << endl;
    cout << "data:" << data << " k:" << k << endl;
	time_t time_start = time(NULL);         //计时
	cout << "Finding top " << k << " nodes with greedy-CELF algorithm" << endl;
	cout << "No.\tnode_id\t  influence\ttime(s)" << endl;

//  find the top k nodes

	int *seed_arr = new int[k];             //种子集合
	//priority_queue<Pair,vector<Pair>,less<Pair> > pqueue;
	queue<Pair> pqueue;
	//queue<Pair> pqueue1;
	Pair nodeIM[g->num_nodes];            //    std::queue<int> myQueue;容器
	for(int i = 0; i < g->num_nodes; i++){
//      cout<<g->num_nodes<<endl;34个
		seed_arr[0] = i;
		float inf = 0;
		pqueue.push(Pair(i, inf));          //全部入队，初始化每个节点影响力为0
		nodeIM[i].key = i;
		nodeIM[i].value = inf;
	}                                       //在 queue 的尾部添加一个元素的副本。这是通过调用底层容器的成员函数 push_back()
	/*
	for(int n = 0; n < g->num_nodes;n++){
        cout<<"key = "<<nodeIM[n].key<<"value = "<<nodeIM[n].value<<endl;
	}
	cout<<nodeIM[6].key<<endl;
	*/
	int *updated = new int[g->num_nodes];   //表明节点是否选为种子节点,1表示没选中
	for (int i = 0; i < g->num_nodes; i++)
        updated[i] = 1;                     ///初始未被选中
	float total_inf = 0;                    //,nownode_inf = 0
    for (int i = 0; i < k; i++) {
        //cout<<"第"<<i+1<<"种子节点: "<<endl;
        ///Pair best_pair = nodeIM[i];
        //Pair best_pair = pqueue.front();//front()获取队列的第一个元素
        Pair best_pair = pqueue.front();
        //cout<<"初始最佳id： "<<best_pair.key<<"初始最佳IM值： "<<best_pair.value<<endl;
        ///Pair state_pair = nodeIM[i];
        //Pair state_pair = pqueue.front();
        Pair state_pair = pqueue.front();
        //cout<<"初始候选id： "<<state_pair.key<<"初始候选IM值： "<<state_pair.value<<endl;

        pqueue.pop();//第一个元素保存后，删除 queue 中的第一个元素
        //Pair testf = pqueue.front();
        //cout<<"现在队列第一个节点id： "<<testf.key<<"现在第一个节点IM值： "<<testf.value<<endl;
		if(i==0){
            for(int j = 0; j < g->num_nodes;j++) {     ///N  执行34次出入队
                if(updated[state_pair.key]==1) {      ///未选中
                    seed_arr[i] = state_pair.key;
			    //cout<<state_pair.key<<endl;
                    float m_gain = mc_influence(g, seed_arr, i+1) - total_inf;//增益
                    nodeIM[j].value = m_gain;
                    //cout<<best_pair.value<<"   "<<nodeIM[j].value<<endl;
			    //cout<<nodeIM[j].key<<"  "<<nodeIM[j].value<<"  "<<endl;
			    //float m_gain = mc_influence(g, seed_arr, i+1);
			    //cout<<"轮次: "<<j<<"候选节点id: "<<state_pair.key<<"最大IM值: "<<best_pair.value<<"infs增量: "<<m_gain<<endl;
			    //nodeIM_arr[j] = m_gain;//节点与影响力绑定
			    //cout<<"该节点"<<j<<"的影响力: "<<nodeIM_arr[j]<<endl;
                    if(m_gain>best_pair.value){
                        best_pair.value = m_gain;
                        best_pair.key=state_pair.key;
			        //cout<<"?"<<best_pair.key<<"?"<<endl;
                        pqueue.push(state_pair);      ///在 queue 的尾部添加一个元素的副本
			        //Pair testb = pqueue.back();
			        //cout<<"------"<<endl;
			        //cout<<"现在队尾节点id： "<<testb.key<<"现在队尾节点IM值： "<<testb.value<<endl;
			        //cout<<"------"<<endl;
                    }
                    else{
                        pqueue.push(state_pair);
                    //Pair testb = pqueue.back();
                    //cout<<"----"<<endl;
                    //cout<<"现在队尾节点id： "<<testb.key<<"现在队尾节点IM值： "<<testb.value<<endl;
                    //cout<<"----"<<endl;
                    }
                }                                     ///选中
                else{
                    pqueue.push(state_pair);          //入队
                //Pair testb = pqueue.back();
                //cout<<"--"<<endl;
                //cout<<"现在队尾节点id： "<<testb.key<<"现在队尾节点IM值： "<<testb.value<<endl;
                //cout<<"--"<<endl;
                }
            // cout<<j<<endl;
		    //state_pair = pqueue.front();
                state_pair = pqueue.front();
		    //cout<<"   "<<state_pair.key<<"   "<<state_pair.value<<endl;
                pqueue.pop();
            //Pair testf = pqueue.front();
            //cout<<"现在队列第一个节点id1： "<<testf.key<<"现在第一个节点IM值1： "<<testf.value<<endl;
            }
            mergeSort(nodeIM,g->num_nodes);
            /*for(int n = 0; n < g->num_nodes;n++){
            cout<<"key = "<<nodeIM[n].key<<"value = "<<nodeIM[n].value<<endl;
            }
            cout<<"当前数组第1个元素为:"<<nodeIM[0].key<<"  "<<nodeIM[0].value<<endl;
            cout<<"当前数组第2个元素为:"<<nodeIM[1].key<<"  "<<nodeIM[1].value<<endl;*/
            seed_arr[i] = best_pair.key;
            total_inf += best_pair.value;
            //nownode_inf = best_pair.value;
            //cout<<"?"<<best_pair.key<<"?"<<endl;
            //cout<<"?"<<state_pair.key<<"?"<<endl;
            updated[best_pair.key] = 0;
            //int temp = best_pair.key;
            //cout<<nodeIM[i].key<<"  "<<nodeIM[i].value<<endl;
            //cout<<temp<<endl;
            //cout<<updated[0]<<"  "<<updated[best_pair.key]<<endl;
            //Pair testf = pqueue.back();
            //cout<<testf.key<<testf.value<<endl;还是全0;
            /*
            cout<<best_pair.key<<updated[nodeIM[0].key]<<endl;
            for(int n = 0;n<g->num_nodes;n++){
                cout<<updated[nodeIM[n].key]<<" ";
            }
            cout<<endl;
            */
            cout<<"Greedy  ";
            cout << i + 1 << "\t" << best_pair.key << "\t" <<total_inf << "\t\t" << time(NULL) - time_start << endl;
            /*for(int n = 0;n<g->num_nodes;n++){
                cout<<updated[n]<<" ";
            }
            cout<<endl;*/
        }
        //cout<<best_pair.value<<endl;
        //cout<<endl;
        //cout<<"置顶: "<<nodeIM[0].key<<endl;
        if(i > 0){
            for(int j = i; j < g->num_nodes;j++) {     ///N
                //if(updated[nodeIM[i].key]==1) {      ///如果未选中
//                    if(updated[nodeIM[j].key]==0){
//                        nodeIM[j].value = 0;
//                        continue;
//                    }
                    if(updated[nodeIM[i].key] == 1 ){
                        seed_arr[i] = nodeIM[i].key;
                        float m_gain = mc_influence(g, seed_arr, i+1) - total_inf;//增益
                        nodeIM[i].value = m_gain;
                        //cout<<"轮次: "<<i<<"候选节点id: "<<nodeIM[i].key<<"最大IM值: "<<nodeIM[i].value<<"infs增量: "<<m_gain<<endl;
                        if(nodeIM[i].value >= nodeIM[i+1].value){
                            seed_arr[i] = nodeIM[i].key;
                            total_inf += nodeIM[i].value;
                            updated[nodeIM[i].key] = 0;
                        /*for(int n = 0;n<g->num_nodes;n++){
                            cout<<updated[nodeIM[n].key]<<" ";
                        }
                        cout<<endl;
                        for(int n = 0; n < g->num_nodes;n++){
                            cout<<"key = "<<nodeIM[n].key<<"value = "<<nodeIM[n].value<<endl;
                        }*/
                        //cout<<endl;
                            cout<<"CELF    ";
                        //cout<<endl;
                            cout << i + 1 << "\t" << nodeIM[i].key<< "\t" << total_inf << "\t\t" << time(NULL) - time_start << endl;
                            //break;
                        }
                        else{
                            mergeSort(nodeIM,g->num_nodes);
                            /*for(int j = i; j < g->num_nodes;j++){
                                if(updated[nodeIM[j].key]==0)continue;
                                if(updated[nodeIM[j].key]==1) {
                                    seed_arr[i] = nodeIM[j].key;
                                    float m_gain = mc_influence(g, seed_arr, i+1) - total_inf;
                                    nodeIM[j].value = m_gain;
                                    if(m_gain>best_pair.value){
                                        best_pair.value = m_gain;
                                        best_pair.key=nodeIM[j].key;
                                    //pqueue.push(nodeIM[j]);
                                    }
                                    else{
                                        //continue;//pqueue.push(nodeIM[j]);移动元素
                                    }
                                }
                                else{
                                    //continue;//pqueue.push(nodeIM[j]);
                                }
                                //state_pair = pqueue.front();保持队列元素个数
                                //pqueue.pop();
                            }*/
                            /*seed_arr[i] = best_pair.key;
                            total_inf += best_pair.value;
                            updated[best_pair.key] = 0;
                            mergeSort(nodeIM,g->num_nodes);
                            cout<<"Greedy  ";*/
                            //cout<<endl;
                            //cout << i + 1 << "\t" << best_pair.key<< "\t" << total_inf << "\t\t" << time(NULL) - time_start << endl;//不能使用nodeIM[j],超出j的作用域了。
                        }///不能动
                        //cout<<"123 "<<endl;(之前选中的节点所计算的IM值可能对后面有影响)
                    }                                     ///选中
                //state_pair = pqueue.front();
                //pqueue1.pop();
            //}
            //mergeSort(nodeIM,g->num_nodes);
            //seed_arr[i] = best_pair.key;
            //total_inf += best_pair.value;
            //updated[best_pair.key] = 0;
            //cout << i + 1 << "\t" << best_pair.key << "\t" << total_inf << "\t\t" << time(NULL) - time_start << endl;
        }///不能动
//        seed_arr[i] = nodeIM[i].key;
//        total_inf += nodeIM[i].value;
//        updated[nodeIM[i].key] = 0;
//        cout<<"CELF1    ";
//        cout << i + 1 << "\t" << nodeIM[i].key<< "\t" << total_inf << "\t\t" << time(NULL) - time_start << endl;
    }///不能动
	//disp_mem_usage("");
    }
	cout << "Time used: " << time(NULL) - time_start << " s" << endl;
}

///Compute the influence spread using Mento-Carlo simulation
/*统计模拟方法，是指使用随机数（或者更常见的伪随机数）来解决很多计算问题的方法。他的工作原理就是两件事：不断抽样、逐渐逼近
#define S_STATE 0  //未激活节点
#define I_STATE 1  //当前轮激活节点
#define SI_STATE 2 //下一轮激活的节点
#define R_STATE 3  //之前激活的节点
#define REDUND 10
*/
float mc_influence(Graph *g, int *seed_arr, int k){
//  初始化需要一个 unsigned int 类型的参数。在实际开发中，我们可以用时间作为参数，只要每次播种的时间不同，那么生成的种子就不同，最终的随机数也就不同
//	cout<<i<<endl;
	srand((unsigned)time(NULL));
	double inf = 0;
	int *i_arr = new int[g->num_nodes];             //the array of current active nodes     目前激活节点集合
	int i_size = 0;                                 // the # of newly active nodes          新激活节点
	int *r_arr = new int[g->num_nodes];             // the array of previous active nodes   之前激活节点集合

	int r_size = 0;                                 // the # of previously active nodes
	int *si_arr = new int[g->num_nodes];            // the array of nodes to be active in t+1
	int si_size = 0;                                // the # of nodes to be active in t+1
	int *state_arr = new int[g->num_nodes];         /// the state of nodes
//	cout<<r_size<<endl;0
	memset(state_arr, S_STATE, g->num_nodes * sizeof(int)); // initialize the state array
//  cout<<r_size<<endl;0
	int *rand_arr = new int[g->num_nodes];          ///the 0 ~ n-1 numbers sorted by random order
//  cout<<r_size<<endl;0
	for(int r = 0; r < 1000; r++){             ///1000次模拟？蒙特卡洛模拟
		double active_size = 0;
		//reset the state of all nodes
		for(int i = 0; i < r_size; i++){               //初始第一次不执行
			state_arr[r_arr[i]] = S_STATE;
			//cout<<"123"<<endl;
		}
		r_size = 0;
		// initialize the seed nodes
		for(int i = 0; i < k; i++){
			i_arr[i_size++] = seed_arr[i];
			state_arr[i_arr[i]] = I_STATE;
			//cout<<i_arr[i_size++]<<"   "<<state_arr[i_arr[i]]<<endl;
		}
		while(i_size > 0){              //权重级联模型
			active_size += i_size;
			si_size = 0;
			randomOrder(rand_arr, i_size);
			for(int i = 0; i < i_size; i++){
				int i_node = i_arr[i];
				int k_out = g->node_array[i_node].k_out;
				for(int j = 0; j < k_out; j++){
					int neigh = g->node_array[i_node].id_array[j];
					if (state_arr[neigh] == S_STATE) {
						int k_in = g->node_array[neigh].k_in;
						///double pp = 1.0 / k_in;  //激活概率为入度的倒数(边权重/入度的倒数)
						double pp = 0.01;      //独立级联模型
						double rand_float = ((double)rand()) / RAND_MAX;//生成0-1之间的随机数
						if(rand_float < pp) {
							state_arr[neigh] = SI_STATE;
							si_arr[si_size++] = neigh;
						}
					}
				}
			}
			for(int i = 0; i < i_size; i++){    //i即当前轮节点激活过程结束，变成过去激活节点
				state_arr[i_arr[i]] = R_STATE;
				r_arr[r_size++] = i_arr[i];     //r_size++
			}
			i_size = 0;
			for(int i = 0; i < si_size; i++){  //i+1即刚激活的节点准备去激活其他节点
				state_arr[si_arr[i]] = I_STATE;
				i_arr[i_size++] = si_arr[i];
			}
		}
		inf += active_size;
	}
	delete[] i_arr;
	delete[] r_arr;
	delete[] si_arr;
	delete[] state_arr;
	delete[] rand_arr;
//	cout<<inf / NUM_SIMUS<<endl;
	return inf / 1000;
}

