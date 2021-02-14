#include<stdio.h>
typedef long long ll;
ll quickadd(ll a,ll b ,ll mod)//加法模拟乘法
{
    ll res=0;
    while(b>0)
    {
        if(b&1) res=(res+a)%mod;
        a=(a+a)%mod;
        b>>=1;
    }
    return res;
}
ll quickpow(ll a , ll b, ll c)//快速幂
{
    ll res = 1;
    while(b>0)
    {
        if(b&1) res=quickadd( res,  a , c);
        b>>=1;
        a=quickadd(a,a,c);
    }    
    return res%c;
}
int main()
{
    long a,b,c;
    scanf("%ld %ld %ld",&a,&b,&c);
    printf("%ld^%ld mod %ld=%ld",a,b,c,quickpow(a,b,c));
    return 0;
}
