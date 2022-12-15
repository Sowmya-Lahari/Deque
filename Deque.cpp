#include<iostream>
using namespace std;

template <typename T>
class deque{
    private:
        T** data;
        int** trace;
        int block_size=1000;
        int blocks=50;
        //int max_size=5000;
        int start;
        int end;
    public:
        int capacity;
        int max_size=5000;
        int size_d;
        deque()
        {
            capacity=0;
            size_d=0;
            data=new T*[blocks];
            trace=new int*[blocks];
            start=(blocks/2)-5;
            end=start;
        }
        deque(int n,T x)
        {
            size_d=n;
            start=(blocks/2)-5;
            end=start+n/block_size;
            //cout<<start<<" "<<end<<endl;
            if(n<=block_size)
            {
                start=(blocks/2)-5;
                end=start;
                data=new T*[blocks];
                trace=new int*[blocks];
                data[start]=new T[block_size];
                capacity=block_size;
                trace[start]=new int[2];
                trace[start][0]=0;
                trace[start][1]=n-1;
                for(int i=0;i<n;i++)
                    data[start][i]=x;
                return;
            }
            else
            {
                int rem=n;
                if(n>max_size)
                {
                    blocks=(n/block_size)+4;
                    if(n%block_size>0)
                        blocks++;
                    capacity=blocks-4;
                    max_size=blocks*block_size;
                    start=2;
                    end=blocks-3;
                }
                data=new T*[blocks];
                trace=new int*[blocks];
                int temp=0;
                for(int i=start;i<=end && rem>0;i++)
                {
                    //cout<<"rem "<<rem<<endl;
                    data[i]=new T[block_size];
                    trace[i]=new int[2];
                    trace[i][0]=0;
                    if(rem<block_size)
                    {
                        trace[i][1]=rem-1;
                        temp=rem;
                    }
                    else
                    {
                        trace[i][1]=block_size-1;
                        temp=block_size;
                    }
                    for(int j=0;j<temp;j++)
                        data[i][j]=x;
                    rem-=block_size;
                }
            }
        }
        void insert_first_elem(T x,int pos)
        {
            data[start]=new T[blocks];
            trace[start]=new int[2];
            data[start][pos]=x;
            //cout<<x<<"  "<<pos<<endl;
            trace[start][0]=pos;
            trace[start][1]=pos;
            capacity=block_size;
            size_d++;
            return;
        }
        void resize_util(int new_size)
        {
            blocks*=new_size;
            T **temp_data=new T*[blocks];
            int **new_trace=new int*[blocks];
            int new_start=(blocks/2)-5;
            int ind=new_start;
            for(int i=start;i<=end;i++)
            {
                temp_data[ind]=data[i];
                new_trace[ind]=trace[i];
                ind++;
            }
            capacity=ind*block_size;
            max_size=block_size*blocks;
            data=temp_data;
            trace=new_trace;
            start=new_start;
            end=ind-1;
        }
        void push_back(T x)
        {
            //cout<<"here\n"<<end<<endl;
            if(size_d==0)
            {
                insert_first_elem(x,0);
                return;
            }
            size_d++;
            int last=trace[end][1];
            //cout<<last<<endl;
            if(last<block_size-1)
            {
                last++;
                data[end][last]=x;
                //cout<<x<<"  "<<last<<endl;
                trace[end][1]=last;
            }
            else
            {
                if(end==blocks-1)
                {
                    resize_util(2);
                }
                end++;
                data[end]=new T[block_size];
                trace[end]=new int[2];
                data[end][0]=x;
                //cout<<x<<"  "<<0<<endl;
                trace[end][0]=0;
                trace[end][1]=0;
            }
        }
        void pop_back()
        {
            if(size_d==0)
            {
                //cout<<"Underflow";
                return;
            }
            if(trace[end][0]==trace[end][1])
            {
                //free(data[end]);
                data[end]=NULL;
                //free(trace[end]);
                trace[end]=NULL;
                if(start==end)
                {
                    size_d=0;
                    capacity=0;
                    start=(blocks/2)-5;
                    end=start;
                }
                else
                {
                    size_d--;
                    capacity-=block_size;
                }
            }
            else
            {
                trace[end][1]--;
                size_d--;
            }
        }
        void push_front(T x)
        {
            if(size_d==0)
            {
                insert_first_elem(x,block_size/2);
                return;
            }
            size_d++;
            int first=trace[start][0];
            if(first>0)
            {
                first--;
                data[start][first]=x;
                //cout<<x<<"  "<<first<<endl;
                trace[start][0]=first;
                return;
            }
            if(first==0 && start==0)
            {
                resize_util(2);
            }
            start--;
            data[start]=new T[block_size];
            trace[start]=new int[2];
            data[start][block_size-1]=x;
            //cout<<x<<"  "<<block_size-1<<endl;
            trace[start][0]=block_size-1;
            trace[start][1]=block_size-1;
        }
        void pop_front()
        {
            if(size_d==0)
            {
                //cout<<"Underflow";
                return;
            }
            if(trace[start][0]==trace[start][1])
            {
                //free(data[start]);
                data[start]=NULL;
                //free(trace[start]);
                trace[start]=NULL;
                if(start==end)
                {
                    size_d=0;
                    capacity=0;
                    start=(blocks/2)-5;
                    end=start;
                }
                else
                {
                    size_d--;
                    capacity-=block_size;
                }
            }
            else
            {
                trace[start][0]++;
                size_d--;
            }
        }
        T front()
        {
            //cout<<start<<"\t"<<end<<endl;
            static T res;
            if(size_d==0)
                return res;
            return data[start][trace[start][0]];
        }
        T back()
        {
            //cout<<start<<"\t"<<end<<"\t"<<trace[end][1]<<endl;
            static T res;
            if(size_d==0)
                return res;
            return data[end][trace[end][1]];
        }
        bool empty()
        {
            if(size_d==0)
                return true;
            return false;
        }
        int size()
        {
            //cout<<"sizefun"<<endl;
            return size_d;
        }
        void resize(int x,T d)
        {
            if(x==0)
            {
                clear();
                return;
            }
            if(x==size_d)
                return;
            if(x<size_d)
            {
                int del=size_d-x;
                size_d-=del;
                for(int i=end;i>=start && del>0;i--)
                {
                    int cur_sz=trace[i][1]-trace[i][0]+1;
                    if(cur_sz==del)
                    {
                        capacity-=block_size;
                        //free(data[i]);
                        data[i]=NULL;
                        //free(trace[i]);
                        trace[i]=NULL;
                        end--;
                        return;
                    }
                    if(cur_sz>del)
                    {
                        trace[i][1]=trace[i][1]-del;
                        return;
                    }
                    del-=cur_sz;
                }
                return;
            }
            if(x>max_size)
            {
                int new_size=(x/max_size);
                new_size+=1;
                resize_util(new_size);
            }
            if(x>size_d)
            {
                int inc=x-size_d;
                //cout<<"inc  "<<inc<<endl;
                size_d=x;
                while(inc>0)
                {
                    int rem=block_size-trace[end][1]-1;
                    //cout<<"rem  "<<rem<<endl;
                    if(rem>0)
                    {
                        int ind=trace[end][1]+1;
                        for(int i=0;i<rem;i++)
                            data[end][ind++]=d;
                        trace[end][1]=ind-1;
                        inc-=rem;
                    }
                    else
                    {
                        end++;
                        data[end]=new T[block_size];
                        trace[end]=new int[2];
                        trace[end][0]=0;
                        int temp=(inc<block_size)?inc:block_size;
                        for(int i=0;i<temp;i++)
                            data[end][i]=d;
                        trace[end][1]=temp-1;
                        inc-=temp;
                    }
                }
            }
            //cout<<start<<" "<<end<<endl;
        }
        void clear()
        {
            //cout<<start<<" "<<end<<endl;
            /*for(int i=start;i<=end;i++)
            {
                //free(data[i]);
                data[i]=NULL;
                //free(trace[i]);
                trace[i]=NULL;
            }*/
            size_d=0;
            capacity=0;
            start=(blocks/2)-5;
            end=start;
        }
        T operator [] (int n)
        {
            static T res;
            if(size_d==0)
                res;
            int num_elem,rem=n+1;
            for(int i=start;i<=end;i++)
            {
                num_elem=trace[i][1]-trace[i][0]+1;
                if(rem<=num_elem)
                {
                    int ind=trace[i][0]+rem-1;
                    res=data[i][ind];
                    break;
                }
                rem-=num_elem;
            }
            return res;
        }
};
int main()
{
    typedef string type;
    int ch=1;
    deque<type> *d1;
    int n;
    type x,d;
    while (ch<13)
    {
        cout<<endl<<"Enter"<<endl;
        cin>>ch;
        if(ch==0)
        {
            d1=new deque<type>();
        }
        else if(ch==1)
        {
            cin>>n>>x;
            d1=new deque<type>(n,x);
        }
        else if(ch==2)
        {
            cin>>x;
            d1->push_back(x);
        }
        else if(ch==3)
        {
            d1->pop_back();
        }
        else if(ch==4)
        {
            cin>>x;
            d1->push_front(x);
        }
        else if(ch==5)
        {
            d1->pop_front();
        }
        else if(ch==6)
        {
            cout<<d1->front()<<endl;
        }
        else if(ch==7)
        {
            cout<<d1->back()<<endl;
        }
        else if(ch==8)
        {
            if(d1->empty())
                cout<<"Not empty"<<endl;
            else
                cout<<"Empty"<<endl;
        }
        else if(ch==9)
        {
            cout<<d1->size()<<endl;
        }
        else if(ch==10)
        {
            cin>>n>>d;
            d1->resize(n,d);
        }
        else if(ch==11)
        {
            d1->clear();
        }
        else if(ch==12)
        {
            deque<type> temp;
            temp=*d1;
            cin>>n;
            cout<<temp[n]<<endl;
        }
    }
    
    return 0;
}