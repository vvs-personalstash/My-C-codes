#include <bits/stdc++.h>

using namespace std;

#define vi vector<int>
#define vll vector<ll>
#define vvi vector < vi >
#define pb(x) push_back(x)
#define pii pair<int,int>
#define pll pair<long long, long long>
#define all(c) c.begin(),c.end()
#define mp(x,y) make_pair(x,y)
#define mem(a,val) memset(a,val,sizeof(a))
#define eb emplace_back
#define ff first
#define ss second
#define lc(p) (p << 1)
#define rc(p) (p << 1) | 1
#define ps(x, y) fixed << setprecision(y) << x
#define mk(arr, n, type) type *arr = new type[n]
#define range(a, b) substr(a, b - a + 1)
#define trace(x) cerr << #x << ": " << x << endl
#define FIO ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define input(arr,n) FOR(i,0,n) cin>>a[i]
#define FOR(i,k,n) for ( int i=k; i<n; i++ )
#define ROF(i,k,n) for ( int i=k; i>n; i-- )
#define ll long long

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Car{
    int pp;
    queue<string> streets;
};

struct Street{
    vector<int> carsInd;
    int pppc;
    int totalpp;
    int l;
    string name;
    vector<int> openTimes;
};

struct Intersection{
    //int index;
    vector<int> streetsInd;
    int timePassed;
    int lfs;
};

int d,f;
vector<int> pplist;
vector<Intersection> intersections;
vector<Street> streets;
vector<Car> cars;
unordered_map<string,int> strInd;

void print(){
    FOR(i,0,streets.size())
    {
        cout<<"Name: "<<streets[i].name<<" Cars: "<<(streets[i].carsInd.size()==0?"0":"");
        FOR(j,0,streets[i].carsInd.size())
            cout<<cars[streets[i].carsInd[j]].streets.front()<<" to "<<cars[streets[i].carsInd[j]].streets.back();
        cout<<" PPPC: "<<streets[i].pppc<<endl;
    }
}

void finalPrint(){
    FOR(i,0,streets.size())
    {
        for (auto x: streets[i].openTimes)
            cout<<x<<" ";
        cout<<endl;
    }
}

bool cmp(int c1, int c2)
{
    return cars[c1].streets.size()>cars[c2].streets.size();
}

bool cmp2(int s1, int s2)
{
    return streets[s1].pppc>streets[s2].pppc;
}

void CalcPPPC()
{  
    sort(all(pplist),cmp);
    FOR(i,0,pplist.size())
        cars[pplist[i]].pp=i;
    FOR(i,0,streets.size())
    {
        if (streets[i].carsInd.size()==0)
            continue;
        streets[i].totalpp=0;
        FOR(j,0,streets[i].carsInd.size())
            streets[i].totalpp+= cars[streets[i].carsInd[j]].pp;
        streets[i].pppc = streets[i].totalpp*1.0/(streets[i].carsInd.size()*streets[i].l*1.0);
    }
}

void LiterallyTheWholeProgram() {
    bool ok=true;
    while(ok)
    {
        ok=false;
        FOR(i,0,intersections.size())
        {
            if (intersections[i].timePassed>d )
                continue;
            vector<int> streIndexList;
            for(auto x: intersections[i].streetsInd)
                streIndexList.pb(x);
            sort(all(streIndexList),cmp2);
            int totaltime=0;
            for ( auto x: streIndexList )
            {
                if (streets[x].carsInd.size()==0)
                    continue;
                int orig = streets[x].totalpp;
                ok = true;
                int newPP=orig;
                int time=0;
                for ( auto y : streets[x].carsInd )
                {
                    newPP-=cars[y].pp;
                    time++;
                    if (2*newPP<orig)
                        break;
                }
                streets[x].openTimes.pb(time);
                FOR(tt,0,time)
                {
                    int car = streets[x].carsInd.front();
                    streets[x].carsInd.erase(streets[x].carsInd.begin(),streets[x].carsInd.begin()+1);
                    cars[car].streets.pop();
                    if (cars[car].streets.size()>1)
                        streets[strInd[cars[car].streets.front()]].carsInd.pb(car);
                }
                totaltime+=time;
            }
            intersections[i].timePassed+=totaltime;
        }
        CalcPPPC();
    }
}

int main() {
    ifstream MyFile;
    int ni,ns,nc;

    //input starts
    MyFile.open("input5", ios::out);

    MyFile>>d>>ni>>ns>>nc>>f;
    intersections.resize(ni);
    streets.resize(ns);
    cars.resize(nc);
    FOR(i,0,ns)
    {
        int s,e;
        MyFile>>s>>e>>streets[i].name>>streets[i].l;
        intersections[e].streetsInd.pb(i);
        intersections[e].timePassed=0;
        strInd[streets[i].name]=i;
    }

    FOR(i,0,nc)
    {
        int strs;
        MyFile>>strs;
        FOR(j,0,strs)
        {
            string strName;
            MyFile>>strName;
            cars[i].streets.push(strName);
        }
        streets[strInd[cars[i].streets.front()]].carsInd.pb(i);
        pplist.pb(i);
    }
    MyFile.close();
    //input ends

    //Calculate pppc of each street
    CalcPPPC();
  
    //Calculate time per intersection
    LiterallyTheWholeProgram();
    //finalPrint();

    ofstream MyFile2;
    MyFile2.open("output5", ios::in);
    int co=0;
    FOR(i,0,intersections.size())
    {
        bool ok =false;
        intersections[i].lfs=0;
        for ( int x: intersections[i].streetsInd )
        {
            if (accumulate(all(streets[x].openTimes),0)==0)
                continue;
            intersections[i].lfs++;    
        }
        if (intersections[i].lfs!=0)
            co++;
    }
    MyFile2<<co<<'\n';
    FOR(i,0,intersections.size())
    {
        bool ok =false;
        if (intersections[i].lfs==0)
            continue;
        MyFile2<<i<<'\n'<<intersections[i].lfs<<'\n';
        for ( int x: intersections[i].streetsInd )
        {
            if (accumulate(all(streets[x].openTimes),0)==0)
                continue;
            MyFile2<<streets[x].name<<" "<<ceil((int)(  accumulate(all(streets[x].openTimes),0)  /  streets[x].openTimes.size()*1.0  ))<<'\n';
        }
    }
    MyFile2.close();
} 