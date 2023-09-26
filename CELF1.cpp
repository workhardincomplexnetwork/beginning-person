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

#define S_STATE 0  //δ����ڵ�
#define I_STATE 1  //��ǰ�ּ���ڵ�
#define SI_STATE 2 //��һ�ּ���Ľڵ�
#define R_STATE 3  //֮ǰ����Ľڵ�
#define REDUND 10

/*

int main(int argc, char** argv)
�������е�argc������ǲ���������������Ϊ1��argv[0]��.exe�ļ���·��)��argvΪָ���ʾ�Ĳ���,argv[0]��ʾ��һ��������argv[1]��ʾ�ڶ����������Դ����ơ�

�����в����ڳ���ʼ���е�ʱ�򴫵ݸ�����

�����в������ô����������棺1.ȷ����Ϣ����Դ�����������ļ��ĵ�ַ��  2.ȷ����Ϣ���յ㣨��������ĵ�ַ��  3.�л������ִ��

argc will have been set to the count of the number of strings that��argc��������Ϊ���ַ������ļ��� ��
argv will point to, and argv will have been set to point to an array of pointers to the individual strings��argv��ָ�򣬶�argv��������Ϊָ��ָ�򵥸��ַ�����ָ������ ��
argv[0] will point to the program name string, what ever that is,����ָ����������ַ�������������ʲô�� ��
argv[1] will point to the first argument string,
argv[2] will point to the second argument string, and so on, with
argv[argc-1] pointing to the last argument string, and
argv[argc] pointing at a NULL pointer��ָ��һ����ָ�룩

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


// �ϲ�������������Ϊһ����������
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

// �鲢����
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

�漰�������ָ��Ĺ�ϵ������

��C�����Ҫ�붨��һ���ַ����������ְ취��

1������һ���ַ����飺char a[] = "hello"  2������һ���ַ�ָ�룺char *b = "hello"

a �� b �д洢�Ķ������ַ��ĵ�ַ�����ǿ���ͨ�� a[2] �õ� ��l�� �����ң�Ҳ����ͨ�� p[2] �õ� ��l����

printf(a) �� printf(b) ��һ���ġ��ڴ�ӡʱ��printf()��ӡָ�������ַ���������������ַ�����Ų��ָ�������ӡ��ֱ��\0��

��ʵ�ϣ���ͨ��a[2]ȡ��Ԫ��ֵʱ�����ڲ�����ͨ��ָ�뷽���õ��ġ�

�����ߵĲ�֮ͬ�����ڣ�

a ��һ���������洢���ǵ�һ���ַ��ĵ�ַ���ǲ��ɱ�ġ����� a++���Ǵ�ġ�

b �Ǹ�ָ��������洢��һ���ַ��ĵ�ַ��Ҳ�����ַ����ĵ�ַ�����ǿɱ�ģ�����b++������ͨ�� *��b+2�� �õ� ��l����

*/
int main(int argn, char ** argv)
{

	cout << "Program Start at: " << currentTimestampStr() << endl; //currentTimestampStr()���ص�ǰʱ��
	cout << "Arguments: ";
//  cout << argv[0];
	for(int i = 0; i < argn; i++){                                 //���exe�ļ�·��
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

    for(int i=0; i<argn; i++)                    //ʲô���ã�
    {
        //cout<<string("-data")<<endl;
        //cout<<data<<endl;
        if(argv[i] == string("-data")){
        	data=string(argv[i+1]);
//          cout<<endl<<"helloworld"<<endl;
        }
        if(argv[i] == string("-k")){
        	k=atoi(argv[i+1]);//atoi���ַ�������ת��Ϊ��������
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
	time_t time_start = time(NULL);         //��ʱ
	cout << "Finding top " << k << " nodes with greedy-CELF algorithm" << endl;
	cout << "No.\tnode_id\t  influence\ttime(s)" << endl;

//  find the top k nodes

	int *seed_arr = new int[k];             //���Ӽ���
	//priority_queue<Pair,vector<Pair>,less<Pair> > pqueue;
	queue<Pair> pqueue;
	//queue<Pair> pqueue1;
	Pair nodeIM[g->num_nodes];            //    std::queue<int> myQueue;����
	for(int i = 0; i < g->num_nodes; i++){
//      cout<<g->num_nodes<<endl;34��
		seed_arr[0] = i;
		float inf = 0;
		pqueue.push(Pair(i, inf));          //ȫ����ӣ���ʼ��ÿ���ڵ�Ӱ����Ϊ0
		nodeIM[i].key = i;
		nodeIM[i].value = inf;
	}                                       //�� queue ��β�����һ��Ԫ�صĸ���������ͨ�����õײ������ĳ�Ա���� push_back()
	/*
	for(int n = 0; n < g->num_nodes;n++){
        cout<<"key = "<<nodeIM[n].key<<"value = "<<nodeIM[n].value<<endl;
	}
	cout<<nodeIM[6].key<<endl;
	*/
	int *updated = new int[g->num_nodes];   //�����ڵ��Ƿ�ѡΪ���ӽڵ�,1��ʾûѡ��
	for (int i = 0; i < g->num_nodes; i++)
        updated[i] = 1;                     ///��ʼδ��ѡ��
	float total_inf = 0;                    //,nownode_inf = 0
    for (int i = 0; i < k; i++) {
        //cout<<"��"<<i+1<<"���ӽڵ�: "<<endl;
        ///Pair best_pair = nodeIM[i];
        //Pair best_pair = pqueue.front();//front()��ȡ���еĵ�һ��Ԫ��
        Pair best_pair = pqueue.front();
        //cout<<"��ʼ���id�� "<<best_pair.key<<"��ʼ���IMֵ�� "<<best_pair.value<<endl;
        ///Pair state_pair = nodeIM[i];
        //Pair state_pair = pqueue.front();
        Pair state_pair = pqueue.front();
        //cout<<"��ʼ��ѡid�� "<<state_pair.key<<"��ʼ��ѡIMֵ�� "<<state_pair.value<<endl;

        pqueue.pop();//��һ��Ԫ�ر����ɾ�� queue �еĵ�һ��Ԫ��
        //Pair testf = pqueue.front();
        //cout<<"���ڶ��е�һ���ڵ�id�� "<<testf.key<<"���ڵ�һ���ڵ�IMֵ�� "<<testf.value<<endl;
		if(i==0){
            for(int j = 0; j < g->num_nodes;j++) {     ///N  ִ��34�γ����
                if(updated[state_pair.key]==1) {      ///δѡ��
                    seed_arr[i] = state_pair.key;
			    //cout<<state_pair.key<<endl;
                    float m_gain = mc_influence(g, seed_arr, i+1) - total_inf;//����
                    nodeIM[j].value = m_gain;
                    //cout<<best_pair.value<<"   "<<nodeIM[j].value<<endl;
			    //cout<<nodeIM[j].key<<"  "<<nodeIM[j].value<<"  "<<endl;
			    //float m_gain = mc_influence(g, seed_arr, i+1);
			    //cout<<"�ִ�: "<<j<<"��ѡ�ڵ�id: "<<state_pair.key<<"���IMֵ: "<<best_pair.value<<"infs����: "<<m_gain<<endl;
			    //nodeIM_arr[j] = m_gain;//�ڵ���Ӱ������
			    //cout<<"�ýڵ�"<<j<<"��Ӱ����: "<<nodeIM_arr[j]<<endl;
                    if(m_gain>best_pair.value){
                        best_pair.value = m_gain;
                        best_pair.key=state_pair.key;
			        //cout<<"?"<<best_pair.key<<"?"<<endl;
                        pqueue.push(state_pair);      ///�� queue ��β�����һ��Ԫ�صĸ���
			        //Pair testb = pqueue.back();
			        //cout<<"------"<<endl;
			        //cout<<"���ڶ�β�ڵ�id�� "<<testb.key<<"���ڶ�β�ڵ�IMֵ�� "<<testb.value<<endl;
			        //cout<<"------"<<endl;
                    }
                    else{
                        pqueue.push(state_pair);
                    //Pair testb = pqueue.back();
                    //cout<<"----"<<endl;
                    //cout<<"���ڶ�β�ڵ�id�� "<<testb.key<<"���ڶ�β�ڵ�IMֵ�� "<<testb.value<<endl;
                    //cout<<"----"<<endl;
                    }
                }                                     ///ѡ��
                else{
                    pqueue.push(state_pair);          //���
                //Pair testb = pqueue.back();
                //cout<<"--"<<endl;
                //cout<<"���ڶ�β�ڵ�id�� "<<testb.key<<"���ڶ�β�ڵ�IMֵ�� "<<testb.value<<endl;
                //cout<<"--"<<endl;
                }
            // cout<<j<<endl;
		    //state_pair = pqueue.front();
                state_pair = pqueue.front();
		    //cout<<"   "<<state_pair.key<<"   "<<state_pair.value<<endl;
                pqueue.pop();
            //Pair testf = pqueue.front();
            //cout<<"���ڶ��е�һ���ڵ�id1�� "<<testf.key<<"���ڵ�һ���ڵ�IMֵ1�� "<<testf.value<<endl;
            }
            mergeSort(nodeIM,g->num_nodes);
            /*for(int n = 0; n < g->num_nodes;n++){
            cout<<"key = "<<nodeIM[n].key<<"value = "<<nodeIM[n].value<<endl;
            }
            cout<<"��ǰ�����1��Ԫ��Ϊ:"<<nodeIM[0].key<<"  "<<nodeIM[0].value<<endl;
            cout<<"��ǰ�����2��Ԫ��Ϊ:"<<nodeIM[1].key<<"  "<<nodeIM[1].value<<endl;*/
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
            //cout<<testf.key<<testf.value<<endl;����ȫ0;
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
        //cout<<"�ö�: "<<nodeIM[0].key<<endl;
        if(i > 0){
            for(int j = i; j < g->num_nodes;j++) {     ///N
                //if(updated[nodeIM[i].key]==1) {      ///���δѡ��
//                    if(updated[nodeIM[j].key]==0){
//                        nodeIM[j].value = 0;
//                        continue;
//                    }
                    if(updated[nodeIM[i].key] == 1 ){
                        seed_arr[i] = nodeIM[i].key;
                        float m_gain = mc_influence(g, seed_arr, i+1) - total_inf;//����
                        nodeIM[i].value = m_gain;
                        //cout<<"�ִ�: "<<i<<"��ѡ�ڵ�id: "<<nodeIM[i].key<<"���IMֵ: "<<nodeIM[i].value<<"infs����: "<<m_gain<<endl;
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
                                        //continue;//pqueue.push(nodeIM[j]);�ƶ�Ԫ��
                                    }
                                }
                                else{
                                    //continue;//pqueue.push(nodeIM[j]);
                                }
                                //state_pair = pqueue.front();���ֶ���Ԫ�ظ���
                                //pqueue.pop();
                            }*/
                            /*seed_arr[i] = best_pair.key;
                            total_inf += best_pair.value;
                            updated[best_pair.key] = 0;
                            mergeSort(nodeIM,g->num_nodes);
                            cout<<"Greedy  ";*/
                            //cout<<endl;
                            //cout << i + 1 << "\t" << best_pair.key<< "\t" << total_inf << "\t\t" << time(NULL) - time_start << endl;//����ʹ��nodeIM[j],����j���������ˡ�
                        }///���ܶ�
                        //cout<<"123 "<<endl;(֮ǰѡ�еĽڵ��������IMֵ���ܶԺ�����Ӱ��)
                    }                                     ///ѡ��
                //state_pair = pqueue.front();
                //pqueue1.pop();
            //}
            //mergeSort(nodeIM,g->num_nodes);
            //seed_arr[i] = best_pair.key;
            //total_inf += best_pair.value;
            //updated[best_pair.key] = 0;
            //cout << i + 1 << "\t" << best_pair.key << "\t" << total_inf << "\t\t" << time(NULL) - time_start << endl;
        }///���ܶ�
//        seed_arr[i] = nodeIM[i].key;
//        total_inf += nodeIM[i].value;
//        updated[nodeIM[i].key] = 0;
//        cout<<"CELF1    ";
//        cout << i + 1 << "\t" << nodeIM[i].key<< "\t" << total_inf << "\t\t" << time(NULL) - time_start << endl;
    }///���ܶ�
	//disp_mem_usage("");
    }
	cout << "Time used: " << time(NULL) - time_start << " s" << endl;
}

///Compute the influence spread using Mento-Carlo simulation
/*ͳ��ģ�ⷽ������ָʹ������������߸�������α�������������ܶ��������ķ��������Ĺ���ԭ����������£����ϳ������𽥱ƽ�
#define S_STATE 0  //δ����ڵ�
#define I_STATE 1  //��ǰ�ּ���ڵ�
#define SI_STATE 2 //��һ�ּ���Ľڵ�
#define R_STATE 3  //֮ǰ����Ľڵ�
#define REDUND 10
*/
float mc_influence(Graph *g, int *seed_arr, int k){
//  ��ʼ����Ҫһ�� unsigned int ���͵Ĳ�������ʵ�ʿ����У����ǿ�����ʱ����Ϊ������ֻҪÿ�β��ֵ�ʱ�䲻ͬ����ô���ɵ����ӾͲ�ͬ�����յ������Ҳ�Ͳ�ͬ
//	cout<<i<<endl;
	srand((unsigned)time(NULL));
	double inf = 0;
	int *i_arr = new int[g->num_nodes];             //the array of current active nodes     Ŀǰ����ڵ㼯��
	int i_size = 0;                                 // the # of newly active nodes          �¼���ڵ�
	int *r_arr = new int[g->num_nodes];             // the array of previous active nodes   ֮ǰ����ڵ㼯��

	int r_size = 0;                                 // the # of previously active nodes
	int *si_arr = new int[g->num_nodes];            // the array of nodes to be active in t+1
	int si_size = 0;                                // the # of nodes to be active in t+1
	int *state_arr = new int[g->num_nodes];         /// the state of nodes
//	cout<<r_size<<endl;0
	memset(state_arr, S_STATE, g->num_nodes * sizeof(int)); // initialize the state array
//  cout<<r_size<<endl;0
	int *rand_arr = new int[g->num_nodes];          ///the 0 ~ n-1 numbers sorted by random order
//  cout<<r_size<<endl;0
	for(int r = 0; r < 1000; r++){             ///1000��ģ�⣿���ؿ���ģ��
		double active_size = 0;
		//reset the state of all nodes
		for(int i = 0; i < r_size; i++){               //��ʼ��һ�β�ִ��
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
		while(i_size > 0){              //Ȩ�ؼ���ģ��
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
						///double pp = 1.0 / k_in;  //�������Ϊ��ȵĵ���(��Ȩ��/��ȵĵ���)
						double pp = 0.01;      //��������ģ��
						double rand_float = ((double)rand()) / RAND_MAX;//����0-1֮��������
						if(rand_float < pp) {
							state_arr[neigh] = SI_STATE;
							si_arr[si_size++] = neigh;
						}
					}
				}
			}
			for(int i = 0; i < i_size; i++){    //i����ǰ�ֽڵ㼤����̽�������ɹ�ȥ����ڵ�
				state_arr[i_arr[i]] = R_STATE;
				r_arr[r_size++] = i_arr[i];     //r_size++
			}
			i_size = 0;
			for(int i = 0; i < si_size; i++){  //i+1���ռ���Ľڵ�׼��ȥ���������ڵ�
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

