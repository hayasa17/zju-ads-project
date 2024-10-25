#include<bits/stdc++.h>
using namespace std;

bool backtracking(vector<vector<int>>& graph, map<pair<int, int>, vector<int>>& results, int n, int m, int checked_num);
bool judge_direction(vector<vector<int>> graph, int n, int m, int index_i, int index_j, int direction);

int scan_downward(vector<vector<int>>& graph, map<pair<int, int>, vector<int>>& results, int index_i, int index_j, int n);
int scan_rightward(vector<vector<int>>& graph, map<pair<int, int>, vector<int>>& results, int index_i, int index_j, int m);

void undo_down(vector<vector<int>>& graph, map<pair<int, int>, vector<int>>& results, int index_i, int index_j, int new_i);
void undo_right(vector<vector<int>>& graph, map<pair<int, int>, vector<int>>& results, int index_i, int index_j, int new_j);

int main(void){
    int n, m;
    cin >> n >> m;
    int i, j;

    vector<vector<int>> graph(n, vector<int>(m));
    map<pair<int, int>, vector<int>> results;

    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            cin >> graph[i][j];
        }
    }

    cout << endl;
    if(backtracking(graph, results, n, m, 0)==false){
        cout << "No Solution" << endl;
    }else{
        for(const auto& ptr:results){
            pair<int, int> key = ptr.first;
            const vector<int> &vec = ptr.second;

            if(vec[0]==0 && vec[1]==0 && vec[2]==0 && vec[3]==0){
                continue;
            }

            cout << key.first+1 << ' ' << key.second+1 << ' ';

            for(int val:vec){
                cout << val << ' ';
            }

            cout << endl;
        }
    }

    return 0;
}

bool backtracking(vector<vector<int>>& graph, map<pair<int, int>, vector<int>>& results, int n, int m, int checked_num){
    bool found = false;
    int i, j;

    if(checked_num == n*m){
        for(i=0;i<n;i++){
            for(j=0;j<m;j++){
                if(graph[i][j]!=0){
                    return false;
                }
            }
        }

        return true;
    }

    int index_i, index_j;
    index_i = checked_num / m;
    index_j = checked_num % m;

    int degree = graph[index_i][index_j];
    int new_i_1, new_j_1;

    switch(degree){
        case 0:
            found = backtracking(graph, results, n, m, checked_num + 1);

        case 1:            
            if(judge_direction(graph, n, m, index_i, index_j, 1)==true){
                new_i_1 = scan_downward(graph, results, index_i, index_j, n);
                found = backtracking(graph, results, n, m, checked_num + 1);
                if(found==false){
                    undo_down(graph, results, index_i, index_j, new_i_1);
                }
            }
                     
            if(judge_direction(graph, n, m, index_i, index_j, 3)==true){
                new_j_1 = scan_rightward(graph, results, index_i, index_j, m);
                found = backtracking(graph, results, n, m, checked_num + 1);
                if(found==false){
                    undo_right(graph, results, index_i, index_j, new_j_1);
                }
            }

            break;

        case 2:          
            if(judge_direction(graph, n, m, index_i, index_j, 1)==true && judge_direction(graph, n, m, index_i, index_j, 3)==true){
                new_i_1 = scan_downward(graph, results, index_i, index_j, n);
                new_j_1 = scan_rightward(graph, results, index_i, index_j, m);
                found = backtracking(graph, results, n, m, checked_num + 1);
                if(found==false){
                    undo_down(graph, results, index_i, index_j, new_i_1);
                    undo_right(graph, results, index_i, index_j, new_j_1);
                }
            }

            break;
    }

    return found;
}

int scan_downward(vector<vector<int>>& graph, map<pair<int, int>, vector<int>>& results, int index_i, int index_j, int n){
    int i, temp;
    for(i=index_i+1;i<n;i++){
        temp = graph[i][index_j];
        if(temp!=0){
            pair<int, int> p;
            p.first=i;
            p.second = index_j;
            if(results.count(p)==0){
                vector<int> answer(4, 0);
                answer[0] = 1;
                results[p] = answer;
            }else{
                results[p][0] = 1;
            }
            graph[i][index_j]--;

            p.first = index_i;
            p.second = index_j;
            if(results.count(p)==0){
                vector<int> answer(4, 0);
                answer[1] = 1;
                results[p] = answer;
            }else{
                results[p][1] = 1;
            }
            graph[index_i][index_j]--;

            return i;
        }
    }
}

int scan_rightward(vector<vector<int>>& graph, map<pair<int, int>, vector<int>>& results, int index_i, int index_j, int m){
    int j, temp;
    for(j=index_j+1;j<m;j++){
        temp = graph[index_i][j];
        if(temp!=0){
            pair<int, int> p;
            p.first=index_i;
            p.second = j;
            if(results.count(p)==0){
                vector<int> answer(4, 0);
                answer[2] = 1;
                results[p] = answer;
            }else{
                results[p][2] = 1;
            }
            graph[index_i][j]--;

            p.first = index_i;
            p.second = index_j;
            if(results.count(p)==0){
                vector<int> answer(4, 0);
                answer[3] = 1;
                results[p] = answer;
            }else{
                results[p][3] = 1;
            }
            graph[index_i][index_j]--;

            return j;
        }
    }
}

bool judge_direction(vector<vector<int>> graph, int n, int m, int index_i, int index_j, int direction){
    int i, j;
    switch(direction){
        case 1:
            for(i=index_i+1;i<n;i++){
                if(graph[i][index_j]!=0){
                    return true;
                }
            }
            return false;
        
        case 3:
            for(j=index_j+1;j<m;j++){
                if(graph[index_i][j]!=0){
                    return true;
                }
            }
            return false;
    }
}

void undo_down(vector<vector<int>>& graph, map<pair<int, int>, vector<int>>& results, int index_i, int index_j, int new_i){
    graph[index_i][index_j]++;
    graph[new_i][index_j]++;
    results[make_pair(index_i, index_j)][1] = 0;
    results[make_pair(new_i, index_j)][0] = 0;
}

void undo_right(vector<vector<int>>& graph, map<pair<int, int>, vector<int>>& results, int index_i, int index_j, int new_j){
    graph[index_i][index_j]++;
    graph[index_i][new_j]++;
    results[make_pair(index_i, index_j)][3] = 0;
    results[make_pair(index_i, new_j)][2] = 0;
}