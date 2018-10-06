#pragma once
#include <iostream>
#include <vector>
#define B 10
#define M 7
#define S 3

class state
{
    public:
        state(){}
        state(int _big, int _mid, int _small)
        {
            b = _big;
            m = _mid; 
            s = _small;
            pre = 0;
        }

        bool operator ==(state &_s)
        {
            return b==_s.b && m==_s.m && s==_s.s;
        }

        void operator = (const state &_s) // const不能漏
        {
            b = _s.b;
            m = _s.m;
            s = _s.s;
            pre = _s.pre;
        }

        int b;
        int m;
        int s;
        int pre; // 父结点编号，方便打印
};

class qOperator
{
    public:
        qOperator(){}
        bool isEmpty()
        {
            return q.empty();
        }
        state deQueue()
        {
            state temp = *(q.begin());
            q.erase(q.begin());
            return temp;
        }
        void enQueue(state cur)
        {
            q.push_back(cur);
        }

        std::vector<state>::iterator getQueueBegin()
        {
            return q.begin();
        }

        std::vector<state>::iterator getQueueEnd()
        {
            return q.end();
        }
    private:
        std::vector<state> q;
};

class SeparateOil
{
    public:
        SeparateOil(){}

        bool run(int s_big, int s_mid, int s_sma, int t_big, int t_mid, int t_sma) 
        {
            //初始化
            state cur(s_big, s_mid, s_sma);
            state _target(t_big, t_mid, t_sma);
            cur_id = 0;
            target = _target;
            
            //BFS
            q.enQueue(cur);
            while(!q.isEmpty())
            {
                cur = q.deQueue();
                if(isTarget(cur))
                {
                    printPath(cur);
                    return true;
                }
                childrenEnQueue(cur);
            }
            return false;
        }

        void childrenEnQueue(state n) // n为当前结点
        {
            if(n.m < M)         enQueue(B-M-n.s,    M,      n.s); // 装满7两桶
            if(n.s < S)         enQueue(B-S-n.m,    n.m,    S); // 装满3两桶
            if(n.m > 0)         enQueue(B-n.s,      0,      n.s); // 倒空7两桶
            if(n.s > 0)         enQueue(B-n.m,      n.m,    0); // 倒空3两桶    
            if(n.m + n.s < S)   enQueue(n.b,        0,      n.s+n.m); // 7两中的油全倒入3两桶
            if(n.m + n.s < M)   enQueue(n.b,        n.s+n.m, 0); // 3两中的油全倒入7两桶
            if(n.m + n.s >= M)  enQueue(n.b,        M,      n.s+n.m-M); // 用3两中的油装满7两
            if(n.m + n.s >= S)  enQueue(n.b,        n.s+n.m-S, S); // 用7两中的油装满3两
            cur_id++;
        }

    private:
        void printPath(state _cur)
        {
            std::vector<int> path; 
            while(cur_id != _cur.pre)
            {
                path.insert(path.begin(), cur_id);
                
                cur_id = visited[cur_id].pre; //向上查找
                //std::cout << "cur_id = " << cur_id << std::endl;
                _cur = visited[cur_id];
            }
            path.insert(path.begin(), cur_id); //根节点

            for(std::vector<int>::iterator _it=path.begin(); _it!=path.end(); ++_it)
            {
                std::cout << "[" << visited[*_it].b << ", " << visited[*_it].m << ", " << visited[*_it].s << "]" << std::endl;
            }
        }

        bool isTarget(state _cur)
        {
            visited.push_back(_cur);
            return _cur == target; 
        }

        void enQueue(int _big, int _mid, int _sma)
        {
            state _child(_big, _mid, _sma);
            if(!isInQueue(_child) && !isInVisited(_child))
            {
                _child.pre = cur_id;
                q.enQueue(_child);
            }
        }

        bool isInQueue(state _cur)
        {
            for(std::vector<state>::iterator _it=q.getQueueBegin(); _it!=q.getQueueEnd(); ++_it)
            {
                if(_cur == *_it) 
                    return true;
            }
            return false;
        }

        bool isInVisited(state _cur)
        {
            for(std::vector<state>::iterator _it=visited.begin(); _it!=visited.end(); ++_it)
            {
                if(_cur == *_it)
                    return true;
            }
            return false;
        }

        void childrenEnQueue2(state _cur) //最初的实现，和舍友讨论后改成了现在的版本
        {
            for(int i=1; i<=8; ++i)
            {
                state child = doRule(_cur, i);
                if(!isInQueue(child) && !isInVisited(child))
                {
                    child.pre = cur_id;
                    q.enQueue(child);
                }
            }
            cur_id++;
        }

        state doRule(state _cur, int _index)
        {
           switch(_index)
           {
            case 1:
                if(_cur.m < M)  // 装满7两桶
                {
                    //_cur.b = B - (M - _cur.m);
                    _cur.m = M;
                    _cur.b = B - M - _cur.s;
                }
                break;
            case 2:
                if(_cur.s < S)  // 装满3两桶
                {
                    _cur.s = S;
                    _cur.b = B - S - _cur.m;
                }
                break;
            case 3:
                if(_cur.m > 0) // 倒空7两桶
                {
                    _cur.m = 0;
                    _cur.b = B - _cur.s; 
                }
                break;
            case 4:
                if(_cur.s > 0) // 倒空3两桶
                {
                    _cur.s = 0;
                    _cur.b = B - _cur.m;
                }
                break;
            case 5:
                if(_cur.m + _cur.s < S)  //7两中的油全倒入3两桶
                {
                    _cur.s = _cur.s + _cur.m;
                    _cur.m = 0;
                }
                break;
            case 6:
                if(_cur.m + _cur.s < M)   // 3两中的油全倒入7两桶
                {
                    _cur.m = _cur.s + _cur.m;
                    _cur.s = 0;
                }
                break;
            case 7:
                if(_cur.m + _cur.s >= M)  // 用3两中的油装满7两
                {
                    _cur.s = _cur.s + _cur.m - M;
                    _cur.m = M;
                }
                break;
            case 8:
                if(_cur.m + _cur.s >= S)  // 用7两中的油装满3两
                {
                    _cur.m = _cur.s + _cur.m - S;
                    _cur.s = S;
                }
                break;
           }
           return _cur;     
        }

    private:
        qOperator q;
        std::vector<state> visited;
        int cur_id;
        state start;
        state target;
};