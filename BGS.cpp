#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <string>
#include <cstring>
#include <math.h>
#include <windows.h>
#include <direct.h>
#define fi first
#define se second
#define pb push_back
#define pob pop_back
#define sz(Object) Object.size()
#define all(Object) Object.begin(), Object.end()
#define Fu(tmp,L,R,Jump)	for(int tmp=L; tmp<R; tmp+=Jump)
#define Fd(tmp,R,L,Jump)	for(int tmp=R; tmp>=L; tmp-=Jump)
#define space(Amount)   string(Amount, ' ') 
#define charOut(Tmp, Amount)    string(Amount, Tmp)
using namespace std;
typedef long long ll;
typedef pair <int, int> pii;
const int oo = 23092001;
void SetColor(int highlight_color, int text_color){
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    int color_code = highlight_color * 16 + text_color;
    SetConsoleTextAttribute(hStdout, color_code);
}
void SetFontSize(int FontSize){
    CONSOLE_FONT_INFOEX info = {0};
    info.cbSize       = sizeof(info);
    info.dwFontSize.Y = FontSize;
    info.FontWeight   = FW_NORMAL;
    wcscpy(info.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}
void cutline(char x, int color){
    SetColor(0, color); 
    cout << string(120, x);
    SetColor(0, 7);
    cout << endl;
}
void cutline(){
    SetColor(0, 11); 
    cout << string(120, '_');
    SetColor(0, 7);
    cout << endl;
}
int realLength(const string &str) {
	int length = 0;
	for (const auto &c : str)
		if ((c & 0xC0) != 0x80)
			++length;
	return length;
}
struct Node{
    int val;
    char leaf;
    Node* left;
    Node* right;
    Node(int v, Node* x, Node* y){
        val = v;
        left = x;
        right = y;
        leaf = NULL;
    }
    Node(int v, char x){
        val = v;
        left = right = NULL;
        leaf = x;
    }
};
class cmpNode{
    public:
        bool operator() (Node* a, Node* b){
            if(a->val == b->val){
                if(a->leaf != NULL || b->leaf != NULL)
                    return a->leaf > b->leaf;
            }
            return a->val > b->val;
        }
};
class Activate{
    public:
        vector <int> hz;
        vector <string> res;
        string cptdata;
        string decode;
        string CurPath;
        vector <string> in4;
        Activate(){
            hz.assign(256, 0);
            res.assign(256, "");
            cptdata.clear();
            decode.clear();
            in4.pb("cpu get name");
            in4.pb("os get Caption");
            in4.pb("diskdrive get model");
            in4.pb("path win32_VideoController get name");
            in4.pb("path win32_physicalmedia get SerialNumber");
            char tmp[256];
            CurPath = string(_getcwd(tmp, 256));
        }
        void Read_CMD_File(char Cmd[]){
            system(Cmd);
            string tmp = "cmd /c type ";
            string Cvt = tmp + '"' + CurPath + "\\Temp.txt\"" + " > " + CurPath + "\\Temp2.txt\"";
            char* ch = (char*)Cvt.c_str();
            system(ch);
            ifstream fin("Temp2.txt");
                while(getline(fin, tmp))
                    cptdata = cptdata + tmp + '\n';
            fin.close();
        }
        void Read_Data(){
            string CMDPath;
            char* Cmd;
            for(const auto &i:in4){
                CMDPath = "wmic " + i + " > " + CurPath + "\\Temp.txt";
                Cmd = (char*)CMDPath.c_str();
                Read_CMD_File(Cmd);
            }
            remove("Temp.txt");
            remove("Temp2.txt");
        }
        void Create_Huffman_Code(Node* root, string tmp){
            if(root -> leaf != NULL){
                int x = root -> leaf;
                res[x] = tmp;
            }
            else{
                Create_Huffman_Code(root -> left, tmp + '0');
                Create_Huffman_Code(root -> right, tmp + '1');
            }
        }
        void Create_Key(){
            for(const auto &i:cptdata)
                ++hz[int(i)];        
            priority_queue < Node*, vector < Node* > , cmpNode > pq;
            Fu(i, 32, 256, 1)
                if(hz[i] != 0)  
                    pq.push(new Node(hz[i], char(i)));
            while(sz(pq) > 1){
                Node* x = pq.top();   pq.pop();
                Node* y = pq.top();   pq.pop();
                Node* root = new Node(x->val + y->val, x, y);
                pq.push(root);
            }
            Create_Huffman_Code(pq.top(), "");
                for(const auto &i:cptdata)
                    if(i != '\n')
                        decode = decode + res[int(i)];
                    else
                        decode = decode + '\n';
        }
        bool Check(){
            // Read_Data();
            // Create_Key();
            // string key = "";
            // string read;
            // ifstream fin("KEY.txt");
            //     while(getline(fin, read))
            //         key = key + read + '\n';
            // fin.close();
            // if(decode == key)  return true;
            // return false;
            return true;
        }
};

int choice;
bool undir;
class DisjointSet{
    public:
        vector <int> parent;
        DisjointSet(int numOfV){
            parent.assign(numOfV + 1, 0);
            Fu(i, 0, numOfV+1, 1)    parent[i] = i;
        }
        int Root(int x){
            if(parent[x] == x) return x;
            parent[x] = Root(parent[x]);
            return parent[x];
        }
        bool Union(int x, int y){
            x = Root(x);
            y = Root(y);
            if(x == y)  return false;
            if(x < y)   parent[y] = x;
            else        parent[x] = y;
            return true;
        }
};
bool increasingLength(pair <pii, int> a, pair <pii, int> b){
    if(a.se == b.se){
        if(a.fi.fi == b.fi.fi)  return a.fi.se < b.fi.se;
        return a.fi.fi < b.fi.fi;
    }
    return a.se < b.se;
}
class Graph{
    public:
        int V;  //số điểm.
        int E;  //số cạnh.
        vector < vector<int> > edges; //danh sách cạnh.
        vector < vector<bool> > matrix; //ma trận kề.
        vector <int> ideg;
        vector <int> odeg; //bậc.
        //Contructors:
        Graph(int numberOfVertices){
            V = numberOfVertices;
            edges.assign(V+1, vector <int> ());
            matrix.assign(V+1, vector <bool> (V+1, 0));
            ideg.assign(V+1, 0);
            odeg.assign(V+1, 0);
        }
        Graph(int numberOfVertices, int numberOfEdges){
            V = numberOfVertices;
            E = numberOfEdges;
            edges.assign(V+1, vector <int> ());
            matrix.assign(V+1, vector <bool> (V+1, 0));
            ideg.assign(V+1, 0);
            odeg.assign(V+1, 0);
        }
        //Functions:
        void addEdge(int x, int y){
            edges[x].pb(y);
            matrix[x][y] = true;
            ++odeg[x];
            ++ideg[y];
        }
        void CreateGraph(int method){
            int x, y;
            char z;
            /*Nhập ma trận kề:*/
            if(method == 1){
                cout << "Nhập ma trận kề:\n";
                Fu(i, 1, V+1, 1)
                    Fu(j, 1, V+1, 1){
                        cin >> z;
                        if(z != '0')
                            addEdge(i, j);
                    }
            }
            /*Nhập danh sách cạnh:*/
            else if(method == 2){
                cout << "Nhập số cạnh: ";
                cin >> E;
                cout << "Nhập danh sách cạnh:\n";
                Fu(i, 0, E, 1){
                    cin >> x >> y;
                    addEdge(x, y);
                    if(undir)
                        addEdge(y, x);
                }
                Fu(i, 1, V+1, 1)
                    sort(all(edges[i]));
            }
        }
        void DSKtoDSC(int method){
            switch (method){
                case 1:{
                    int y;
                    string s;
                    ofstream fout("OUTPUT.txt");
                    cin.ignore();
                    Fu(x, 1, V+1, 1){
                        getline(cin , s);
                        stringstream ss(s);
                        while(ss >> y)
                            if(!undir || (undir && x < y)) 
                                fout << x << ' ' << y << endl;
                    }
                    fout.close();
                    break;
                }
                case 2:{
                    SetConsoleOutputCP(437);
                    ifstream fin("INPUT.txt");
                    ofstream fout("TEMP.txt");
                    bool isVer = true;
                    char _C;
                    while(fin >> _C)
                        if(isdigit(_C)){
                            string tmp = "";
                            tmp = tmp + _C;
                            while(fin >> _C){
                                if(isdigit(_C))  tmp += _C;
                                else    break;
                            }
                            if(isVer){
                                isVer = false;
                                fout << endl;
                            }
                            else
                                fout << tmp << ' ';
                            if(!isVer && _C == '}')    isVer = true;
                        }
                    fin.close();
                    fout.close();
                    fin.open("TEMP.txt");
                    fout.open("OUTPUT.txt");
                    int y;
                    string s;
                    fin.ignore();
                    Fu(x, 1, V+1, 1){
                        getline(fin , s);
                        stringstream ss(s);
                        while(ss >> y)
                            if(!undir || (undir && x < y)) 
                                fout << x << ' ' << y << endl;
                    }
                    fin.close();
                    fout.close();
                    remove("TEMP.txt");
                    SetConsoleOutputCP(65001);
                    break;
                }
            }      
        }
        void MTKtoDSC(){
            ofstream fout("OUTPUT.txt");
            char tmp;
            Fu(i, 1, V+1, 1)
                Fu(j, 1, V+1, 1){
                    cin >> tmp;
                    if(tmp != '0' && (!undir || (undir && i < j)))
                        fout << i << ' ' << j << endl;
                }
            fout.close();
        }
        void MTLTtoDSC(){
            ofstream fout("OUTPUT.txt");
            vector < vector <int> > a(V+1, vector <int> (E+1, 0));
            Fu(i, 1, V+1, 1)
                Fu(j, 1, E+1, 1)
                    cin >> a[i][j];
            Fu(j, 1, E+1, 1){
                int x = 0, y = 0;
                Fu(i, 1, V+1, 1)
                    if(a[i][j]){
                        if(x)   y = a[i][j] * i;
                        else    x = a[i][j] * i;
                    }
                if(x > 0)
                    fout << x << ' ' << abs(y) << endl;
                else 
                    fout << y << ' ' << -x << endl;
            }
            fout.close();
        }
        void DSCtoDSK(){
            ofstream fout("OUTPUT.txt");
            Fu(i, 1, V+1, 1){
                for(const auto &point:edges[i])
                    fout << point << ' ';
                fout << endl;
            }
            fout.close();
        }
        void DSCtoMTK(){
            ofstream fout("OUTPUT.txt");
            vector < vector <bool> > a(V+1, vector <bool> (V+1, 0));
            Fu(i, 1, V+1, 1)
                for(const auto &point:edges[i])
                    a[i][point] = 1;
            Fu(i, 1, V+1, 1){
                Fu(j, 1, V+1, 1)
                    fout << a[i][j] << ' ';
                fout << endl;
            }
            fout.close();
        }
        void DSCtoMTLT(){
            ofstream fout("OUTPUT.txt");
            vector < vector <int> > a(V+1, vector <int> (E+1, 0));
            int cnt = 1;
            Fu(i, 1, V+1, 1)
                for(const auto &j:edges[i])
                    if(!undir || (undir && i < j)){
                        a[i][cnt] = 1;
                        a[j][cnt] = (undir?1:-1);
                        ++cnt;
                    }
            Fu(i, 1, V+1, 1){
                Fu(j, 1, E+1, 1)
                    fout << a[i][j] << ' ';
                fout << endl;
            }
            fout.close();
        }
        void DegDisplay(){
            ofstream fout("OUTPUT.txt");
            if(undir)
                Fu(i, 1, V+1, 1)
                    fout << "Deg" << '(' << i << ") = " << ideg[i] << endl;
            else
                Fu(i, 1, V+1, 1)
                    fout << "DegIn(" << i << ") = " << ideg[i] << "   DegOut(" << i << ") = "   << odeg[i] << endl;
            fout.close();
        }
        void FindPathByDFS(int u, int v){
            ofstream fout("OUTPUT.txt");
            fout << "DFS = ";
            stack <int> s;
            vector <int> parent(V+1, -1);
            s.push(u);
            parent[u] = 0;
            fout << u << "(0), ";
            while(!s.empty()){
                int top = s.top();
                s.pop();
                for(const auto &w:edges[top])
                    if(parent[w] == -1){
                        parent[w] = top;
                        s.push(top);
                        s.push(w);
                        fout << w << '(' << top << "), ";
                        break;
                    }
            }
            fout << endl << "Đường đi từ U tới V: " << v;
            while(parent[v]){
                fout << " <- " << parent[v];
                v = parent[v];
            }
            fout.close();
        }
        void FindPathByBFS(int u, int v){
            ofstream fout("OUTPUT.txt");
            fout << "BFS = ";
            queue <int> q;
            vector <int> parent(V+1, -1);
            q.push(u);
            parent[u] = 0;
            fout << u << "(0), ";
            while(!q.empty()){
                int front = q.front();
                q.pop();
                for(const auto &w:edges[front])
                    if(parent[w] == -1){
                        parent[w] = front;
                        q.push(w);
                        fout << w << '(' << front << "), ";
                    }
            }
            fout << endl << "Đường đi từ U tới V: " << v;
            while(parent[v]){
                fout << " <- " << parent[v];
                v = parent[v];
            }
            fout.close();
        }
        void FindTPLTByDFS(int u, vector <int> &parent, ofstream &fout){
            fout << "DFS(" << u << ") = ";
            stack <int> s;
            s.push(u);
            parent[u] = 0;
            fout << u << "(0), ";
            while(!s.empty()){
                int top = s.top();
                s.pop();
                for(const auto &w:edges[top])
                    if(parent[w] == -1){
                        parent[w] = top;
                        s.push(top);
                        s.push(w);
                        fout << w << '(' << top << "), ";
                        break;
                    }
            }
            fout << endl;
        }
        void FindTPLTByBFS(int u, vector <int> &parent, ofstream &fout){
            fout << "BFS(" << u << ") = ";
            queue <int> q;
            q.push(u);
            parent[u] = 0;
            fout << u << "(0), ";
            while(!q.empty()){
                int front = q.front();
                q.pop();
                for(const auto &w:edges[front])
                    if(parent[w] == -1){
                        parent[w] = front;
                        q.push(w);
                        fout << w << '(' << front << "), ";
                    }
            }
            fout << endl;
        }
        void DFSCutVertex(int del, int start, vector <bool> &seen, ofstream &fout){
            stack <int> s;
            fout << "DFS(" << start << ") = ";
            s.push(start);
            seen[start] = true;
            fout << start << "(0), ";
            while(!s.empty()){
                int top = s.top();
                s.pop();
                for(const auto &w:edges[top])
                    if(!seen[w] && w != del){
                        seen[w] = true;
                        s.push(top);
                        s.push(w);
                        fout << w << '(' << top << "), ";
                        break;
                    }
            }
            fout << endl;
        }
        void BFSCutVertex(int del, int start, vector <bool> &seen, ofstream &fout){
            queue <int> q;
            fout << "BFS(" << start << ") = ";
            q.push(start);
            seen[start] = true;
            fout << start << "(0), ";
            while(!q.empty()){
                int front = q.front();
                q.pop();
                for(const auto &w:edges[front])
                    if(!seen[w] && w != del){
                        seen[w] = true;
                        q.push(w);
                        fout << w << '(' << front << "), ";
                    }
            }
            fout << endl;
        }
        void DFSBridge(int xdel, int ydel, int start, vector <bool> &seen, ofstream &fout){
            stack <int> s;
            fout << "DFS(" << start << ") = ";
            s.push(start);
            seen[start] = true;
            fout << start << "(0), ";
            while(!s.empty()){
                int top = s.top();
                s.pop();
                for(const auto &w:edges[top])
                    if(!seen[w]){
                        if(top == xdel && w == ydel)    continue;
                        if(undir && top == ydel && w == xdel)   continue;
                        seen[w] = true;
                        s.push(top);
                        s.push(w);
                        fout << w << '(' << top << "), ";
                        break;
                    }
            }
            fout << endl;
        }
        void BFSBridge(int xdel, int ydel, int start, vector <bool> &seen, ofstream &fout){
            queue <int> q;
            fout << "BFS(" << start << ") = ";
            q.push(start);
            seen[start] = true;
            fout << start << "(0), ";
            while(!q.empty()){
                int front = q.front();
                q.pop();
                for(const auto &w:edges[front])
                    if(!seen[w]){
                        if(front == xdel && w == ydel)    continue;
                        if(undir && front == ydel && w == xdel)   continue;
                        seen[w] = true;
                        q.push(w);
                        fout << w << '(' << front << "), ";
                    }
            }
            fout << endl;
        }
        void Euler(int start){
            ofstream fout("OUTPUT.txt");
            if(undir){
                int cnt = 0;
                Fu(i, 1, V+1, 1)    if(ideg[i]&1)    ++cnt;
                if(cnt > 2){
                    fout << "Không phải đồ thị Euler hoặc nửa Euler!\n";
                    return;
                }
            }
            else{
                int cnt = 0;
                Fu(i, 1, V+1, 1)
                    cnt += abs(ideg[i] - odeg[i]);
                if(cnt > 2){
                    fout << "Không phải đồ thị Euler hoặc nửa Euler!\n";
                    return;
                }
            }
            vector < vector <bool> > check(all(matrix));
            // vector < vector<int> > check(all(edges));
            vector <int> res;
            vector <int> s;
            vector <int> temp;
            string CE;
            s.pb(start);
            while(!s.empty()){
                int cur = s[sz(s) - 1];
                bool haveEdge = false;
                Fu(i, 1, V+1, 1)
                    if(check[cur][i]){
                        if(!temp.empty()){
                            for(const auto &j:s)  fout << j << ' ';
                            for(const auto &j:temp) CE = CE + to_string(j) + ' ';
                            fout << "\nCE: " << CE;
                            temp.clear();
                            fout << "\n-----------------------\n";
                        }
                        haveEdge = true;
                        s.pb(i);
                        check[cur][i] = false;
                        if(undir)   check[i][cur] = false;
                        break;
                    }
                if(!haveEdge){
                    s.pob();
                    temp.pb(cur);
                    res.pb(cur);
                }
                /*Check bằng danh sách kề:
                    // if(!check[cur].empty()){
                    //     int tmp = check[cur][0];
                    //     s.push(check[cur][0]);
                    //     check[cur].erase(check[cur].begin());
                    //     if(undir)   check[tmp].erase(lower_bound(all(check[tmp]), cur));
                    // }
                    // else{
                    //     s.pop();
                    //     res.push(cur);
                    // }
                */
                if(sz(res) == E+1)  break;
            }
            // while(!res.empty()){
            //     fout << res.top() << ' ';
            //     res.pop();
            // }
            reverse(all(res));
            for(const auto &i:res)  fout << i << ' ';
            fout.close();
        }
        void Hamilton(vector <bool> &seen, vector <int> &res, ofstream &fout){
            if(sz(res) == V){
                for(const auto &i:res)  fout << i << ' ';
                if(binary_search(all(edges[res[V-1]]), res[0]))  fout << res[0];
                fout << "\n------------------------------------\n";
                return;
            }
            int cur = res[sz(res)-1];
            for(const auto &i:edges[cur])
                if(!seen[i]){
                    seen[i] = true;
                    res.pb(i);
                    Hamilton(seen, res, fout);
                    seen[i] = false;
                    res.pob();
                }
        }
        void DFS_SpanningTree(int start, vector <string> &res, ofstream &fout){
            stack <int> s;
            vector <bool> seen(V+1, 0);
            fout << "DFS: " << start << "(0), ";
            s.push(start);
            seen[start] = true;
            while(!s.empty()){
                int cur = s.top();
                s.pop();
                for(const auto &i:edges[cur])
                    if(!seen[i]){
                        seen[i] = true;
                        s.push(cur);
                        s.push(i);
                        res.pb("(" + to_string(min(i, cur)) + ", " + to_string(max(i, cur)) + "), ");
                        fout << i << '(' << cur << "), ";
                        break;
                    }
            }
            fout << endl;
        }
        void BFS_SpanningTree(int start, vector <string> &res, ofstream &fout){
            queue <int> q;
            vector <bool> seen(V+1, 0);
            fout << "BFS: " << start << "(0), ";
            q.push(start);
            seen[start] = true;
            while(!q.empty()){
                int cur = q.front();
                q.pop();
                for(const auto &i:edges[cur])
                    if(!seen[i]){
                        seen[i] = true;
                        q.push(i);
                        res.pb("(" + to_string(min(i, cur)) + ", " + to_string(max(i, cur)) + "), ");
                        fout << i << '(' << cur << "), ";
                    }
            }
            fout << endl;
        }
};
//Weighted Graph (Đồ thị trọng số):
class WEG{
    public:
        int V;
        int E;
        vector < vector <int> > matrix;
        WEG(int numberOfVertices){
            V = numberOfVertices;
            matrix.assign(V+1, vector <int> (V+1, oo));
            Fu(i, 1, V+1, 1)    matrix[i][i] = 0;
        }
        WEG(int numberOfVertices, int numberOfEdges){
            V = numberOfVertices;
            E = numberOfEdges;
            matrix.assign(V+1, vector <int> (V+1, oo));
            Fu(i, 1, V+1, 1)    matrix[i][i] = 0;
        }
        void addEdge(int x, int y, int val){
            matrix[x][y] = val;
        }
        void CreateGraph(int method){
            if(method == 1){
                cout << "Ký tự vô cùng được quy ước trong ma trận là: ";
                string infinite;
                cin >> infinite;
                cout << "Nhập ma trận trọng số:\n";
                string tmp;
                Fu(i, 1, V+1, 1)
                    Fu(j, 1, V+1, 1){
                        cin >> tmp;
                        if(tmp.substr(0, sz(infinite)) != infinite)
                            addEdge(i, j, stoi(tmp));
                    }
            }
            /*Nhập danh sách cạnh:*/
            else if(method == 2){
                cout << "Nhập số cạnh của đồ thị: ";
                cin >> E;
                int x, y, z;
                cout << "Nhập danh sách cạnh:\n";
                Fu(i, 0, E, 1){
                    cin >> x >> y >> z;
                    addEdge(x, y, z);
                    if(undir)
                        addEdge(y, x, z);
                }
            }
            else if(method == 23092001){
                cout << "Nhập ma trận trọng số:\n";
                int tmp;
                Fu(i, 1, V+1, 1)
                    Fu(j, 1, V+1, 1){
                        cin >> tmp;
                        addEdge(i, j, tmp);
                    }
            }
        }
        string Display(int dist, int parent, int Lg10, int status){
            if(status == 1)    return space(7 + Lg10) + '-' + space(7 + Lg10) + "||";
            if(dist >= oo) return space(6 + Lg10) + "+oo" + space(6 + + Lg10) + "||";
            string left = to_string(dist);
            string right = to_string(parent);
            if(status == 2) return space(7 + Lg10 - sz(left)) + left + '|' + right + '*' + space(6 + Lg10 - sz(right)) + "||";
            return space(7 + Lg10 - sz(left)) + left + '|' + right + space(7 + Lg10 - sz(right)) + "||";
        }
        string Line(char x){
            string res;
            res = charOut(x, 8);
            Fu(i, 1, V+1, 1)
                res += charOut(x, 17 + 2*(int)log10(i));
            return res;
        }
        string PathFromUtoV(int u, int v, vector <int> parent){
            vector <string> tmp;
            while(parent[v] != parent[u]){
                tmp.pb(to_string(v));
                tmp.pb(" -> ");
                v = parent[v];
            }
            tmp.pb(to_string(u));
            reverse(all(tmp));
            string res = "";
            for(const auto &i:tmp)
                res += i;
            return res;
        }
        void Dijktra(int u){
            ofstream fout("OUTPUT.txt");
            fout << Line('=') << endl;
            fout << "||Bước||";
            Fu(i, 1, V+1, 1)
                fout << space(3) << "d[" << i << "]|e[" << i <<"]" << space (3) << "||";
            fout << endl << Line('=') << endl;
            //Init:
            vector <int> status(V+1, 0);
            /*Status:
                - 0: Chưa thăm.
                - 1: Đã thăm.
                - 2: Đã thăm và đang là đỉnh duyệt.
            */
            vector <int> dist(V+1, oo);
            vector <int> parent(V+1, -1);
            dist[0] = 0;
            dist[u] = 0;
            parent[u] = 0;
            Fu(step, 0, V+1, 1){
                //Find Vertex:
                int CurVertex = 0;
                Fu(i, 1, V+1, 1)
                    if(!status[i] && (CurVertex == 0 || (dist[i] < dist[CurVertex])))
                        CurVertex = i;
                if(!CurVertex)  goto DONE;
                status[CurVertex] = 2;
                //Display:
                fout << "|| " << step << space(3-sz(to_string(step))) << "||";
                Fu(i, 1, V+1, 1)
                    fout << Display(dist[i], parent[i], (int)log10(i), status[i]);
                fout << endl << Line('-') << endl;
                status[CurVertex] = 1;
                //Calculate:
                Fu(i, 1, V+1, 1)
                    if(!status[i]){
                        int dist_tmp = dist[CurVertex] + matrix[CurVertex][i];
                        if(dist_tmp < dist[i]){
                            dist[i] = dist_tmp;
                            parent[i] = CurVertex;
                        }
                    }
            }
            DONE:
            Fu(i, 1, V+1, 1)
                if(i != u){
                    if(dist[i] >= oo)
                        fout << "\nKhông có đường đi từ " << u << " tới " << i << endl;
                    else fout << "\nĐường đi ngắng nhất từ " << u << " tới " << i << " là: " << PathFromUtoV(u, i, parent) << "(cost: " << dist[i] <<  ")\n";
                    fout << charOut('-', 30);
                }
            fout.close();
        }
        void Bellman_Ford(int u){
            ofstream fout("OUTPUT.txt");
            fout << Line('=') << endl;
            fout << "||Bước||";
            Fu(i, 1, V+1, 1)
                fout << space(3) << "d[" << i << "]|e[" << i <<"]" << space (3) << "||";
            fout << endl << Line('=') << endl;
            //Init:
            vector <int> dist(V+1, oo);
            vector <int> parent(V+1, u);
            dist[0] = 0;
            parent[0] = 0;
            dist[u] = 0;
            parent[u] = 0;
            Fu(i, 1, V+1, 1){
                if(i == u)  continue;
                dist[i] = matrix[u][i];
            }
            bool change = true;
            for(int step = 1; change; ++step){
                change = false;
                //Display:
                fout << "|| " << step << space(3-sz(to_string(step))) << "||";
                Fu(i, 1, V+1, 1)
                    fout << Display(dist[i], parent[i], (int)log10(i), 0);
                fout << endl << Line('-') << endl;
                //Calculate:
                Fu(i, 1, V+1, 1)
                    Fu(j, 1, V+1, 1){
                        if(i == j || dist[j] == oo)  continue;
                        int dist_tmp = dist[j] + matrix[j][i];
                        if(dist[i] > dist_tmp){
                            dist[i] = dist_tmp;
                            parent[i] = j;
                            change = true;
                        }
                    }
            }
            Fu(i, 1, V+1, 1)
                if(i != u){
                    if(dist[i] >= 23090000){
                        fout << "\nKhông có đường đi từ " << u << " tới " << i << endl;
                        continue;
                    }
                    fout << "\nĐường đi ngắn nhất từ " << u << " tới " << i << " là: " << PathFromUtoV(u, i, parent) << "(cost: " << dist[i] <<  ")\n";
                    fout << charOut('-', 30);
                }
            fout.close();
        }
        string Display(int dist, int parent){
            string left = to_string(dist);
            string right = to_string(parent);
            return space(3 - sz(left)) + left + '|' + right + space(3 -sz(right)) + "||";
        }
        void Floyd(){
            ofstream fout("OUTPUT.txt");
            vector < vector <int> > next(V+1, vector <int> (V+1, -1));
            Fu(i, 1, V+1, 1)
                Fu(j, 1, V+1, 1)
                    if(matrix[i][j] != oo)
                        next[i][j] = j;
            fout << "Bước 0:\n";
                Fu(i, 1, V+1, 1){
                    Fu(j, 1, V+1, 1)
                        if(matrix[i][j] >= 23090000)  fout << "  +oo  ||";
                        else
                            fout << Display(matrix[i][j], next[i][j]);
                    fout << endl;
                }
            fout << Line('-') << endl;
            Fu(k, 1, V+1, 1){
                fout << "Bước " << k << ":\n";
                Fu(i, 1, V+1, 1)
                    Fu(j, 1, V+1, 1)
                        if(matrix[i][j] > matrix[i][k] + matrix[k][j]){
                            matrix[i][j] = matrix[i][k] + matrix[k][j];
                            next[i][j] = next[i][k];
                        }
                Fu(i, 1, V+1, 1){
                    Fu(j, 1, V+1, 1)
                        if(matrix[i][j] >= 23090000)  fout << "  +oo  ||";
                        else
                            fout << Display(matrix[i][j], next[i][j]);
                    fout << endl;
                }
                fout << Line('-') << endl;
            }
            Fu(i, 1, V+1, 1){
                fout << endl;
                Fu(j, 1, V+1, 1){
                    if(i != j){
                        fout << "\nĐường đi ngắn nhất từ " << i << " tới " << j << " là: ";
                        int k = i;
                        fout << i;
                        while(k != j){
                            k = next[k][j];
                            fout << " -> " << k;
                        }
                    fout << "(cost: " << matrix[i][j] <<  ")\n";
                    }
                }
                fout << Line('-');
            }
            fout.close();
        }
        void Prim(int u){
            ofstream fout("OUTPUT.txt");
            vector <pii> res;
            fout << Line('=') << charOut('=', 19) << endl;
            fout << "||Bước||";
            Fu(i, 1, V+1, 1)
                fout << space(3) << "d[" << i << "]|e[" << i <<"]" << space (3) << "||";
            fout << "   T   ||   WT   ||";
            fout << endl << Line('=') << charOut('=', 19) << endl;
            //Init:
            vector <int> status(V+1, 0);
            /*Status:
                - 0: Chưa thăm.
                - 1: Đã thăm.
                - 2: Đã thăm và đang là đỉnh duyệt.
            */
            vector <int> dist(V+1, oo);
            vector <int> parent(V+1, -1);
            dist[0] = 0;
            dist[u] = 0;
            parent[u] = 0;
            int WT = 0;
            Fu(step, 0, V+1, 1){
                //Find Vertex:
                int CurVertex = 0;
                Fu(i, 1, V+1, 1)
                    if(!status[i] && (CurVertex == 0 || (dist[i] < dist[CurVertex])))
                        CurVertex = i;
                if(!CurVertex && step < V){
                    fout << "Đồ thị không có cây khung.";
                    fout.close();
                    return;
                }
                int x = CurVertex;
                int y = parent[CurVertex];
                if(undir && x > y)  swap(x, y);
                if(step && step < V)   res.pb({x, y});
                WT += dist[CurVertex];
                status[CurVertex] = 2;
                //Display:
                fout << "|| " << step << space(3-sz(to_string(step))) << "||";
                Fu(i, 1, V+1, 1)
                    fout << Display(dist[i], parent[i], (int)log10(i), status[i]);
                fout << '(' << x << ", " << y << ')' << space(3 - sz(to_string(x)) - sz(to_string(y))) << "||" << space(3) << WT << space(5 - sz(to_string(WT))) << "||";
                fout << endl << Line('-') << charOut('-', 19) << endl;
                status[CurVertex] = 1;
                //Update:
                Fu(i, 1, V+1, 1)
                    if(!status[i]){
                        int dist_tmp = matrix[CurVertex][i];
                        if(dist_tmp < dist[i]){
                            dist[i] = dist_tmp;
                            parent[i] = CurVertex;
                        }
                        else if(dist_tmp == dist[i] && parent[i] > CurVertex)
                            parent[i] = CurVertex;
                    }
            }
            fout << "\ndH = " << WT << ".\nCây khung nhỏ nhất: ";
            for(const auto i:res)
                fout << '(' << i.fi << ", " << i.se << "), ";
            fout.close();
        }
        void Kruskal(){
            ofstream fout("OUTPUT.txt");
            DisjointSet DS(V+1);
            vector < pair <pii, int> > edges;
            vector < pii > res;
            int WT = 0;
            Fu(i, 1, V+1, 1)
                Fu(j, 1, V+1, 1)
                    if(matrix[i][j] != oo && (!undir || (undir && i < j)))
                        edges.pb({{i, j}, matrix[i][j]});
            sort(all(edges), increasingLength);
            vector <bool> seen(V+1, 0);
            for(const auto &cur:edges){
                int v1 = cur.fi.fi;
                int v2 = cur.fi.se;
                if(seen[v1] && seen[v2]){
                    if(DS.Union(v1, v2)){
                        res.pb(cur.fi);
                        WT += matrix[v1][v2];
                    }
                }
                else if(!seen[v1] && !seen[v2]){
                    seen[v1] = true;
                    seen[v2] = true;
                    if(DS.Union(v1, v2)){
                        res.pb(cur.fi);
                        WT += matrix[v1][v2];
                    }
                }
                else{
                    if(seen[v1]){
                        DS.parent[v2] = DS.parent[v1];
                        seen[v2] = true;
                    }
                    else{
                        DS.parent[v1] = DS.parent[v2];
                        seen[v1] = true;
                    }
                    res.pb(cur.fi);
                    WT += matrix[v1][v2];
                }
                if(res.size() == V-1)    break;
            }
            fout << "dH = " << WT << ".\nCây khung nhỏ nhất: ";
            for(const auto &cur:res)
                fout << '(' << cur.fi << ", " << cur.se << "), ";
            fout.close();
        }
        bool MaxflowNetwork(int &res){
            vector <int> parent(V+1, 0);
            queue <int> q;
            int path_flow = INT_MAX;
            parent[1] = 1;
            q.push(1);
            while(!q.empty()){
                int u = q.front();  q.pop();
                Fu(v, 2, V+1, 1)
                    if(!parent[v] && matrix[u][v]){
                        path_flow = min(path_flow, matrix[u][v]);
                        parent[v] = u;
                        if(v == V){
                            while(v != parent[v]){
                                matrix[v][parent[v]] += path_flow;
                                matrix[parent[v]][v] -= path_flow;
                                v = parent[v];
                            }
                            res += path_flow;
                            return true;
                        }
                        q.push(v);
                    }
            }
            return false;
        }
};
class WVG{
    public:
        int V;
        int E;
        vector <int> value;
        vector < vector<int> > edges;
        vector < vector<bool> > matrix;
        WVG(int numOfVertex){
            V = numOfVertex;
            value.assign(V+1, 0);
            edges.assign(V+1, vector<int>());
            matrix.assign(V+1, vector<bool>(V+1, 0));
        }
};
/*Function*/

void mout(const string a){
    int len = realLength(a);
    cout << space(20) << "|| " << a << space(75 - len) << "||\n";
}
void moutline(){
    cout << space(20) << "||";
    SetColor(0, 8);
    cout  << charOut('-', 76); 
    SetColor(0, 7);
    cout << "||\n";
}
void ShowMenu(){
    cout << endl;
    cout << space(20) << charOut('=', 80) << endl;
    cout << space(20) << "||";
    SetColor(15, 4);
    cout << space(36) << "MENU" << space(36); 
    SetColor(0, 7);
    cout << "||\n";
    cout << space(20) << charOut('=', 80) << endl;
    mout("1: Chuyển đổi dạng biểu diễn đồ thị.");
    mout("2. Tính bậc của các đỉnh.");
    moutline();
    mout("3: Tìm đường đi từ đỉnh U tới đỉnh V.");
    mout("4: Tìm thành phần liên thông của đồ thị.");
    mout("5: Kiểm tra và tìm đỉnh trụ của đồ thị.");
    mout("6: Kiểm tra và tìm cạnh cầu của đồ thị.");
    mout("7. Kiểm tra tính liên thông của đồ thị.");
    moutline();
    mout("8: Tìm đường đi và chu trình Euler.");
    mout("9: Tìm đường đi và chu trình Halminton.");
    moutline();
    mout("10: Tìm cây khung của đồ thị với gốc U.");
    mout("11: Tìm cây khung nhỏ nhất bằng thuật toán Prim.");
    mout("12. Tìm cây khung nhỏ nhất bằng thuật toán Kruskal.");
    moutline();
    mout("13. Tìm đường đi ngắn nhất bằng thuật toán Dijkstra.");
    mout("14. Tìm đường đi ngắn nhất bằng thuật toán Bellman-Ford.");
    mout("15. Tìm đường đi ngắn nhất bằng thuật toán Floyd.");
    moutline();
    mout("16. Tính giá trị luồng cực đại.");
    cout << space(20) << charOut('=', 80) << endl;
}

bool SolveMethod(int V);
bool Convert(int V);

void ConvertDSKtoDSC(int V);
void ConvertMTKtoDSC(int V);
void ConvertMTLTtoDSC(int V);
void ConvertDSCtoDSK(int V);
void ConvertDSCtoMTK(int V);
void ConvertDSCtoMTLT(int V);
void DegCal(int V);
void DFSFindPath(int V);
void BFSFindPath(int V);
void DFSFindTPLT(int V);
void BFSFindTPLT(int V);
void DFSListCutVertex(int V);
void BFSListCutVertex(int V);
void DFSListBridge(int V);
void BFSListBridge(int V);
void DFSConnectivityCheck(int V);
void BFSConnectivityCheck(int V);
void FindEuler(int V);
void FindHalminton(int V);
void FindSpanningTreeByDFS(int V);
void FindSpanningTreeByBFS(int V);
void FindShortestPathByDijkstra(int V);
void FindShortestPathByBellmanFord(int V);
void FindShortestPathByFloyd(int V);
void FindSmallestSpanningTreeByPrim(int V);
void FindSmallestSpanningTreeByKruskal(int V);
void CalMaxflowNetwork(int V);

void Discrete_Math_2(int V){
    SHOW_MENU:
    ShowMenu();
    cout << "Lựa chọn: ";
    do{
        cin >> choice;
        cutline();
        if(choice < 1 || choice > 16){
            Invalid_Menu_Choice:
            SetColor(0, 12);
            cout << "KHÔNG TỒN TẠI LỰA CHỌN, XEM LẠI MENU:\n";
            SetColor(0, 7);
            ShowMenu();
        }
    }   while(choice < 1 || choice > 16);
    switch (choice){
        case 1:     if(!Convert(V))     goto SHOW_MENU;     break;
        case 2:     DegCal(V);                              break;
        case 3:     case 4: case 5: case 6: case 10:          
        case 7:     if(!SolveMethod(V)) goto SHOW_MENU;     break;
        case 8:     FindEuler(V);                           break;
        case 9:     FindHalminton(V);                       break;
        case 11:    FindSmallestSpanningTreeByPrim(V);      break;
        case 12:    FindSmallestSpanningTreeByKruskal(V);   break;
        case 13:    FindShortestPathByDijkstra(V);          break;
        case 14:    FindShortestPathByBellmanFord(V);       break;
        case 15:    FindShortestPathByFloyd(V);             break;
        case 16:    CalMaxflowNetwork(V);                   break;
        default:    goto Invalid_Menu_Choice;               break;
    }
    SetColor(0, 10);
    cout << space(38) << "Kết quả đã được in ra trong file OUTPUT.txt.\n";     
    SetColor(0, 7); 
}
string in4[5] = {"cpu get name", "os get Caption", "diskdrive get model", "path win32_VideoController get name", "path win32_physicalmedia get SerialNumber"};


/*Main program*/
int main(){
    DeleteMenu(GetSystemMenu(GetConsoleWindow(), false), SC_MAXIMIZE, MF_BYCOMMAND);
    SetConsoleOutputCP(65001);
    // ShowScrollBar(hWnd, SB_HORZ, false);
    SetConsoleTitle("Basic Graph Supporter 2021");
    SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_SIZEBOX);
    SetFontSize(18);
    cout << endl;
    SetColor(0, 10);
    cout << "Basic Graph Supporter 2021\n";
    cout << '\n' << "Created by HlighT - PTIT - B19DCCN333 - fb.com/hlight.fb - github.com/hlight-git\n";
    cout << endl;
    SetColor(0, 9);
    Fu(i, 0, 120, 1)    cout << "░";
    // Activate HlighT;
    // if(!HlighT.Check()){
    //     cout << "\n\n";
    //     SetColor(0, 12);
    //     cout << space(47) << "KEY CỦA BẠN KHÔNG HỢP LỆ!\n";
    //     SetColor(0, 7);
    //     cout << endl << endl;
    //     system("pause");
    //     return 0;
    // }
    bool firstRun = true;
    while(true){
        if(firstRun)    firstRun = false;
        else    cin.ignore();
        cout << "Chọn dạng đồ thị:\n";
        cout << "   0: Đồ thị vô hướng.\n   1: Đồ thị có hướng.\n";
        
        string tmp;
        do{
            getline(cin, tmp);
            if(tmp !=  "1" && tmp != "0")
                cout << "Lựa chọn không hợp lệ, nhập lại: ";
        }while(tmp != "1" && tmp != "0");
        if(tmp == "0")  undir = true;
        else undir = false;
        cutline();
        int V;
        cout << "Nhập số đỉnh của đồ thị: ";
        cin >> V;
        cutline();
        Discrete_Math_2(V);
        cutline('_', 4);
    }
    return 0;
}

bool SolveMethod(int V){
    cout << "Chọn phương pháp duyệt:\n";
    cout << "   1: DFS.\n";
    cout << "   2: BFS.\n";
    cout << "   0: Quay lại menu chính.\n";
    int method;
    do{
        cin >> method;
        Invalid_Input_in_SolveMethod:
        if(method < 0 || method > 2)
            cout << "Lựa chọn không hợp lệ, nhập lại: ";
    }while(method < 0 || method > 2);
    cutline();
    if(method == 1){
        switch(choice){
            case 3:     DFSFindPath(V);             break;
            case 4:     DFSFindTPLT(V);             break;
            case 5:     DFSListCutVertex(V);        break;
            case 6:     DFSListBridge(V);           break;
            case 7:     DFSConnectivityCheck(V);    break;
            case 10:    FindSpanningTreeByDFS(V);   break;
        }
    }
    else if(method == 2){
        switch(choice){
            case 3:     BFSFindPath(V);             break;
            case 4:     BFSFindTPLT(V);             break;
            case 5:     BFSListCutVertex(V);        break;
            case 6:     BFSListBridge(V);           break;
            case 7:     BFSConnectivityCheck(V);    break;
            case 10:    FindSpanningTreeByBFS(V);   break;
        }
    }
    else return false;
    cutline();
    return true;
}
bool Convert(int V){
    cout << "Chọn kiểu chuyển đổi:\n";
    cout << "   1: Chuyển danh sách kề sang danh sách cạnh.\n";
    cout << "   2: Chuyển ma trận kề sang danh sách cạnh.\n";
    cout << "   3. Chuyển ma trận liên thuộc sang danh sách cạnh.\n";
    cout << "   4. Chuyển danh sách cạnh sang danh sách kề.\n";
    cout << "   5. Chuyển danh sách cạnh sang ma trận kề.\n";
    cout << "   6. Chuyển danh sách cạnh sang ma trận liên thuộc.\n";
    cout << "   0. Quay lại menu chính.\n";
    do{
        cin >> choice;
        Invalid_Input_in_Converter:
        if(choice < 0 || choice > 6)
            cout << "Lựa chọn không hợp lệ, nhập lại: ";
    }while(choice < 0 || choice > 6);
    cutline();
    switch(choice){
        case 0: return false;
        case 1: ConvertDSKtoDSC(V);     break;
        case 2: ConvertMTKtoDSC(V);     break;
        case 3: ConvertMTLTtoDSC(V);    break;
        case 4: ConvertDSCtoDSK(V);     break;
        case 5: ConvertDSCtoMTK(V);     break;
        case 6: ConvertDSCtoMTLT(V);    break;
        default: goto Invalid_Input_in_Converter;
            break;
    }
    cutline();
    return true;
}
void ConvertDSKtoDSC(int V){
    Graph g(V);
    int method;
    cout << "Nhập danh sách kề:\n";
    cout << "   1. Nhập bằng tay trên console.\n";
    cout << "   2. Nhập qua file INPUT.txt (Cú pháp: Ke(x) = {y, z, ...})\n";
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    g.DSKtoDSC(method);
}
void ConvertMTKtoDSC(int V){
    Graph g(V);
    cout << "Nhập ma trận kề:\n";
    g.MTKtoDSC();
}
void ConvertMTLTtoDSC(int V){
    cout << "Nhập số cạnh của đồ thị:";
    int E;
    cin >> E;
    cutline();
    Graph g(V);
    cout << "Nhập ma trận liên thuộc:\n";
    g.MTLTtoDSC();
}
void ConvertDSCtoDSK(int V){
    Graph g(V);
    g.CreateGraph(2);
    g.DSCtoDSK();
}
void ConvertDSCtoMTK(int V){
    Graph g(V);
    g.CreateGraph(2);
    g.DSCtoMTK();
}
void ConvertDSCtoMTLT(int V){
    Graph g(V);
    g.CreateGraph(2);
    g.DSCtoMTLT();
}
void DegCal(int V){
    Graph g(V);
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    cutline();
    g.CreateGraph(method);
    g.DegDisplay();
}
void DFSFindPath(int V){
    Graph g(V);
    cout << "Nhập lần lượt 2 đỉnh U và V: ";
    int u, v;
    cin >> u >> v;
    cutline();
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    cutline();
    g.CreateGraph(method);
    g.FindPathByDFS(u, v);
}
void BFSFindPath(int V){
    Graph g(V);
    cout << "Nhập lần lượt 2 đỉnh U và V: ";
    int u, v;
    cin >> u >> v;
    cutline();
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    cutline();
    g.CreateGraph(method);
    g.FindPathByBFS(u, v);
}
void DFSFindTPLT(int V){
    Graph g(V);
    int start;
    cout << "Nhập đỉnh xuất phát: ";
    cin >> start;
    cutline();
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    cutline();
    g.CreateGraph(method);
    ofstream fout("OUTPUT.txt");
    vector <int> parent(V+1, -1);
    g.FindTPLTByDFS(start, parent, fout);
    Fu(i, 1, V+1, 1)
        if(parent[i] == -1)
            g.FindTPLTByDFS(i, parent, fout);
    fout.close();
}
void BFSFindTPLT(int V){
    Graph g(V);
    int start;
    cout << "Nhập đỉnh xuất phát: ";
    cin >> start;
    cutline();
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    cutline();
    g.CreateGraph(method);
    ofstream fout("OUTPUT.txt");
    vector <int> parent(V+1, -1);
    g.FindTPLTByBFS(start, parent, fout);
    Fu(i, 1, V+1, 1)
        if(parent[i] == -1)
            g.FindTPLTByBFS(i, parent, fout);
    fout.close();
}
void DFSListCutVertex(int V){
    Graph g(V);
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    cutline();
    g.CreateGraph(method);
    ofstream fout("OUTPUT.txt");
    vector <int> parent(V+1, -1);
    vector <int> res;
    int tplt = 0;
    fout << "Thành phần liên thông:\n\n";
    Fu(i, 1, V+1, 1)
        if(parent[i] == -1){
            ++tplt;
            g.FindTPLTByDFS(i, parent, fout);
        }
    fout << "\nKiểm tra đỉnh trụ:\n\n";
    Fu(i, 1, V+1, 1){
        vector <bool> seen(V+1, 0);
        int cnt = 0;
        Fu(j, 1, V+1, 1)
            if(i != j && !seen[j]){
                ++cnt;
                g.DFSCutVertex(i, j, seen, fout);
            }
        if(cnt > tplt)  res.pb(i);
        fout << charOut('-', 100) << endl;
    }
    if(res.size()){
        fout << "CÁC ĐỈNH TRỤ CỦA ĐỒ THỊ LÀ: ";
        for(const auto &i:res)  fout << i << ' ';
    }
    else    fout << "ĐỒ THỊ KHÔNG CÓ ĐỈNH TRỤ!";
    fout.close();
}
void BFSListCutVertex(int V){
    Graph g(V);
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    cutline();
    g.CreateGraph(method);
    ofstream fout("OUTPUT.txt");
    vector <int> parent(V+1, -1);
    vector <int> res;
    int tplt = 0;
    fout << "Thành phần liên thông:\n\n";
    Fu(i, 1, V+1, 1)
        if(parent[i] == -1){
            ++tplt;
            g.FindTPLTByBFS(i, parent, fout);
        }
    fout << "\nKiểm tra đỉnh trụ:\n\n";
    Fu(i, 1, V+1, 1){
        vector <bool> seen(V+1, 0);
        int cnt = 0;
        Fu(j, 1, V+1, 1)
            if(i != j && !seen[j]){
                ++cnt;
                g.BFSCutVertex(i, j, seen, fout);
            }
        if(cnt > tplt)  res.pb(i);
        fout << charOut('-', 100) << endl;
    }
    if(res.size()){
        fout << "CÁC ĐỈNH TRỤ CỦA ĐỒ THỊ LÀ: ";
        for(const auto &i:res)  fout << i << ' ';
    }
    else    fout << "ĐỒ THỊ KHÔNG CÓ ĐỈNH TRỤ!";
    fout.close();
}
void DFSListBridge(int V){
    Graph g(V);
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    cin >> method;
    cutline();
    g.CreateGraph(method);
    ofstream fout("OUTPUT.txt");
    vector <int> parent(V+1, -1);
    vector <pii> res;
    int tplt = 0;
    fout << "Thành phần liên thông:\n\n";
    Fu(i, 1, V+1, 1)
        if(parent[i] == -1){
            ++tplt;
            g.FindTPLTByDFS(i, parent, fout);
        }
    fout << "\nKiểm tra cạnh cầu:\n\n";
    Fu(i, 1, V+1, 1){
        for(const auto &j:g.edges[i]){
            if(undir && i > j)   continue;
            fout << " - CẠNH (" << i << ", " << j << "):\n";
            vector <bool> seen(V+1, 0);
            int cnt = 0;
            Fu(k, 1, V+1, 1)
                if(!seen[k]){
                    ++cnt;
                    g.DFSBridge(i, j, k, seen, fout);
                }
            if(cnt > tplt)  res.pb({i, j});
            fout << charOut('-', 100) << endl;
        }
    }
    if(res.size()){
        fout << "CÁC CẠNH CẦU CỦA ĐỒ THỊ LÀ: ";
        for(const auto &i:res)  fout << '(' << i.fi << ", " << i.se << "), ";
    }
    else    fout << "ĐỒ THỊ KHÔNG CÓ CẠNH CẦU!";
    fout.close();
}
void BFSListBridge(int V){
    Graph g(V);
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    cutline();
    g.CreateGraph(method);
    ofstream fout("OUTPUT.txt");
    vector <int> parent(V+1, -1);
    vector <pii> res;
    int tplt = 0;
    fout << "Thành phần liên thông:\n\n";
    Fu(i, 1, V+1, 1)
        if(parent[i] == -1){
            ++tplt;
            g.FindTPLTByBFS(i, parent, fout);
        }
    fout << "\nKiểm tra cạnh cầu:\n\n";
    Fu(i, 1, V+1, 1){
        for(const auto &j:g.edges[i]){
            if(undir && i > j)   continue;
            vector <bool> seen(V+1, 0);
            int cnt = 0;
            fout << " - CẠNH (" << i << ", " << j << "):\n";
            Fu(k, 1, V+1, 1)
                if(!seen[k]){
                    ++cnt;
                    g.BFSBridge(i, j, k, seen, fout);
                }
            if(cnt > tplt)  res.pb({i, j});
            fout << charOut('-', 100) << endl;
        }
    }
    if(res.size()){
        fout << "CÁC CẠNH CẦU CỦA ĐỒ THỊ LÀ: ";
        for(const auto &i:res)  fout << '(' << i.fi << ", " << i.se << "), ";
    }
    else    fout << "ĐỒ THỊ KHÔNG CÓ CẠNH CẦU!";
    fout.close();
}
void DFSConnectivityCheck(int V){
    Graph g(V);
    Graph g2(V);
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    cutline();
    if(method == 2){
        cout << "Nhập số cạnh của đồ thị:";
        cin >> g.E;
        g2.E = g.E;
        cout << "Nhập danh sách cạnh:\n";
        int x, y;
        Fu(i, 0, g.E, 1){
            cin >> x >> y;
            g.addEdge(x, y);
            g2.addEdge(x, y);
            bool alreadyHave = false;
            for(const auto &j:g2.edges[y])
                if(j == x)  alreadyHave = true;
            if(!alreadyHave)    g2.addEdge(y, x);
        }
        Fu(i, 1, V+1, 1){
            sort(all(g.edges[i]));
            sort(all(g2.edges[i]));
        }
    }
    else if(method == 1){
        cout << "Nhập ma trận kề:\n";
        char z;
        Fu(i, 1, V+1, 1)
            Fu(j, 1, V+1, 1){
                cin >> z;
                if(z != '0'){
                    g.addEdge(i, j);
                    g2.addEdge(i, j);
                    bool alreadyHave = false;
                    for(const auto &k:g2.edges[j])
                        if(k == i)  alreadyHave = true;
                    if(!alreadyHave)    g2.addEdge(j, i);
                }
            }
        Fu(i, 1, V+1, 1){
            sort(all(g.edges[i]));
            sort(all(g2.edges[i]));
        }
    }
    ofstream fout("OUTPUT.txt");
    fout << "Các thành phần liên thông của đồ thị vô hướng nền:\n";
    vector <int> parent(V+1, -1);
    Fu(j, 1, V+1, 1)
        if(parent[j] == -1)
            g2.FindTPLTByDFS(j, parent, fout);
    fout << charOut('_', 100) << endl;
    fout << "Kiểm tra tính liên thông:\n";
    Fu(i, 1, V+1, 1){
        fout << " + DFS TỪ ĐỈNH " << i << ":\n";
        parent.assign(V+1, -1);
        g.FindTPLTByDFS(i, parent, fout);
        Fu(j, 1, V+1, 1)
            if(parent[j] == -1)
                g.FindTPLTByDFS(j, parent, fout);
        fout << charOut('-', 100) << endl;
    }
    fout.close();
}
void BFSConnectivityCheck(int V){
    Graph g(V);
    Graph g2(V);
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    cutline();
    if(method == 2){
        cout << "Nhập số cạnh của đồ thị:";
        cin >> g.E;
        g2.E = g.E;
        cout << "Nhập danh sách cạnh:\n";
        int x, y;
        Fu(i, 0, g.E, 1){
            cin >> x >> y;
            g.addEdge(x, y);
            g2.addEdge(x, y);
            bool alreadyHave = false;
            for(const auto &j:g2.edges[y])
                if(j == x)  alreadyHave = true;
            if(!alreadyHave)    g2.addEdge(y, x);
        }
        Fu(i, 1, V+1, 1){
            sort(all(g.edges[i]));
            sort(all(g2.edges[i]));
        }
    }
    else if(method == 1){
        cout << "Nhập ma trận kề:\n";
        char z;
        Fu(i, 1, V+1, 1)
            Fu(j, 1, V+1, 1){
                cin >> z;
                if(z != '0'){
                    g.addEdge(i, j);
                    g2.addEdge(i, j);
                    bool alreadyHave = false;
                    for(const auto &k:g2.edges[j])
                        if(k == i)  alreadyHave = true;
                    if(!alreadyHave)    g2.addEdge(j, i);
                }
            }
        Fu(i, 1, V+1, 1){
            sort(all(g.edges[i]));
            sort(all(g2.edges[i]));
        }
    }
    ofstream fout("OUTPUT.txt");
    fout << "Các thành phần liên thông của đồ thị vô hướng nền:\n";
    vector <int> parent(V+1, -1);
    Fu(j, 1, V+1, 1)
        if(parent[j] == -1)
            g2.FindTPLTByBFS(j, parent, fout);
    fout << charOut('_', 100) << endl;
    fout << "Kiểm tra tính liên thông:\n";
    Fu(i, 1, V+1, 1){
        fout << " + BFS TỪ ĐỈNH " << i << ":\n";
        parent.assign(V+1, -1);
        g.FindTPLTByBFS(i, parent, fout);
        Fu(j, 1, V+1, 1)
            if(parent[j] == -1)
                g.FindTPLTByBFS(j, parent, fout);
        fout << charOut('-', 100) << endl;
        
    }
    fout.close();
}
void FindEuler(int V){
    Graph g(V);
    cout << "Nhập đỉnh xuất phát:\n";
    int start;
    cin >> start;
    cutline();
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    g.CreateGraph(method);
    g.Euler(start);
}
void FindHalminton(int V){
    Graph g(V);
    cout << "Nhập đỉnh xuất phát:\n";
    int start;
    cin >> start;
    cutline();
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    g.CreateGraph(method);    
    ofstream fout("OUTPUT.txt");
    vector <bool> seen (V+1, 0);
    vector <int> res(1, start);
    seen[start] = true;
    g.Hamilton(seen, res, fout);
    fout.close();
}
void FindSpanningTreeByDFS(int V){
    Graph g(V);
    cout << "Nhập gốc của cây:\n";
    int start;
    cin >> start;
    cutline();
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    g.CreateGraph(method);
    
    ofstream fout("OUTPUT.txt");
    vector <string> res;
    g.DFS_SpanningTree(start, res, fout);
    fout << "\nCây khung tìm thấy:\n";
    for(const auto &i:res)
        fout << i;
    fout.close();
}
void FindSpanningTreeByBFS(int V){
    Graph g(V);
    cout << "Nhập gốc của cây:\n";
    int start;
    cin >> start;
    cutline();
    cout << "Phương pháp nhập input:\n    1: Ma trận kề.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    g.CreateGraph(method);
    ofstream fout("OUTPUT.txt");
    vector <string> res;
    g.BFS_SpanningTree(start, res, fout);
    fout << "\nCây khung tìm thấy:\n";
    for(const auto &i:res)
        fout << i;
    fout.close();
}
void FindShortestPathByDijkstra(int V){
    WEG g(V);
    int u;
    cout << "Nhập điểm xuất phát:\n";
    cin >> u;
    cutline();
    cout << "Phương pháp nhập input:\n    1: Ma trận trọng số.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    g.CreateGraph(method);    
    g.Dijktra(u);
}
void FindShortestPathByBellmanFord(int V){
    WEG g(V);
    int u;
    cout << "Nhập điểm xuất phát:\n";
    cin >> u;
    cutline();
    cout << "Phương pháp nhập input:\n    1: Ma trận trọng số.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    g.CreateGraph(method);
    g.Bellman_Ford(u);
}
void FindShortestPathByFloyd(int V){
    WEG g(V);
    cout << "Phương pháp nhập input:\n    1: Ma trận trọng số.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    cutline();
    g.CreateGraph(method);
    g.Floyd();
}
void FindSmallestSpanningTreeByPrim(int V){
    WEG g(V);
    int u;
    cout << "Nhập gốc của cây:\n";
    cin >> u;
    cutline();
    cout << "Phương pháp nhập input:\n    1: Ma trận trọng số.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    g.CreateGraph(method);
    g.Prim(u);
}
void FindSmallestSpanningTreeByKruskal(int V){
    WEG g(V);
    cout << "Phương pháp nhập input:\n    1: Ma trận trọng số.\n    2: Danh sách cạnh.\n";
    int method;
    do{
        cin >> method;
        if(method != 1 && method != 2)
            cout << "Không hợp lệ, nhập lại:";
    }while(method != 1 && method != 2);
    g.CreateGraph(method);
    g.Kruskal();
}
void CalMaxflowNetwork(int V){
    WEG g(V);
    g.CreateGraph(23092001);
    int res = 0;
    while(g.MaxflowNetwork(res));
    ofstream fout("OUTPUT.txt");
    fout << "Luồng cực đại của đồ thị: Maxflow = " << res;
    fout.close();
}