#include<iostream>
#include"crackrar.h"
using namespace std;
const std::vector<muchar> operator+(const std::vector<muchar> a,const std::vector<muchar> b)
{
    std::vector<muchar> c=a;
    c.insert(c.end(),b.begin(),b.end());
    return c;
}

std::vector<muchar> toWideChar(muchar *pass)
{
    timespec tmptime,nowtime;
    clock_gettime(CLOCK_REALTIME,&tmptime);

    int len=strlen((char *)pass);
    std::vector<muchar> res;
    for(int i=0;i<len;i++)
    {
        res.push_back(pass[i]);
        res.push_back(0x00);
    }

    clock_gettime(CLOCK_REALTIME,&nowtime);
    printf("\033[31mtoWideChar\033[m\n");
    if(tmptime.tv_nsec>nowtime.tv_nsec)
    {
        cout<<"0s   "<<1000000000-nowtime.tv_nsec+tmptime.tv_nsec<<" ns"<<endl;
    }
    else
    {
        cout<<"0s   "<<nowtime.tv_nsec-tmptime.tv_nsec<<" ns"<<endl;
    }

    cout<<tmptime.tv_sec<<" "<<tmptime.tv_nsec<<endl;
    cout<<nowtime.tv_sec<<" "<<nowtime.tv_nsec<<endl;

    return res;
}

//get salt,iv,s from file descriptor fd
void getPara(int fd,std::vector<muchar> &salt,std::vector<muchar> &iv,std::vector<muchar> &s)
{
    timespec tmptime,nowtime;
    clock_gettime(CLOCK_REALTIME,&tmptime);

    unsigned char tmp[20];
    unsigned char sa[8],v[16],is[16];
    read(fd,tmp,20);
    read(fd,sa,8);
    read(fd,v,16);
    read(fd,is,16);
    for(int i=0;i<8;i++)
        salt.push_back(sa[i]);
    for(int i=0;i<16;i++)
    {
        iv.push_back(v[i]);
        s.push_back(is[i]);
    }

    clock_gettime(CLOCK_REALTIME,&nowtime);
    printf("\033[31mgetPara\033[m\n");
    if(tmptime.tv_nsec>nowtime.tv_nsec)
    {
        cout<<"0s   "<<1000000000-nowtime.tv_nsec+tmptime.tv_nsec<<" ns"<<endl;
    }
    else
    {
        cout<<"0s   "<<nowtime.tv_nsec-tmptime.tv_nsec<<" ns"<<endl;
    }

    cout<<tmptime.tv_sec<<" "<<tmptime.tv_nsec<<endl;
    cout<<nowtime.tv_sec<<" "<<nowtime.tv_nsec<<endl;
}

void Bsha1(std::vector<muchar> M,muchar *B)
{
    timespec tmptime,nowtime;
    clock_gettime(CLOCK_REALTIME,&tmptime);

    SHA_CTX ctx;
    SHA1_Init(&ctx);
    for(int i=0;i<0x40000;i++)
    {
        muchar tmp[M.size()+3];
        for(unsigned int j=0;j<M.size();j++)
        {
            tmp[j]=M[j];
        }
        tmp[M.size()]=(i&0x0000ff);
        tmp[M.size()+1]=((i&0x00ff00)>>8);
        tmp[M.size()+2]=((i&0x0f0000)>>16);
        SHA1_Update(&ctx,tmp,M.size()+3);
    }
    SHA1_Final(B,&ctx);

    clock_gettime(CLOCK_REALTIME,&nowtime);
    printf("\033[31mBsha1\033[m\n");
    if(tmptime.tv_nsec>nowtime.tv_nsec)
    {
        cout<<"0s   "<<1000000000-nowtime.tv_nsec+tmptime.tv_nsec<<" ns"<<endl;
    }
    else
    {
        cout<<"0s   "<<nowtime.tv_nsec-tmptime.tv_nsec<<" ns"<<endl;
    }

    cout<<tmptime.tv_sec<<" "<<tmptime.tv_nsec<<endl;
    cout<<nowtime.tv_sec<<" "<<nowtime.tv_nsec<<endl;
}

//Calculate C

void CAes(muchar *B,std::vector<muchar> s,muchar *C)
{
    muchar ins[16];
    muchar Bcp[16];//re arrange the sequence of B
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
            Bcp[4*i+j]=B[4*i+3-j];
    }
    AES_KEY keyaes;

    AES_set_decrypt_key(Bcp,128,&keyaes);
    for(int i=0;i<16;i++)
        ins[i]=s[i];
    AES_ecb_encrypt(ins,C,&keyaes,AES_DECRYPT);
}

//test if the key if right
int testKey(std::vector<muchar> salt,std::vector<muchar> s,std::vector<muchar> iv,muchar *key)
{
    std::vector<muchar> veckey=toWideChar(key);
    std::vector<muchar> M=veckey+salt;
    muchar B[20];
    Bsha1(M,B);
    muchar C[16];
    CAes(B,s,C);
    if(((C[12]^iv[12])==0x20)&&(C[13]==iv[13])&&(C[14]==iv[14])&&(C[15]==iv[15]))
        return 1;
    else
        return 0;
}

void prtvector(std::vector<muchar> v)
{
    for(int i=0;i<(int)v.size();i++)
        printf("%02x",v[i]);
    printf("\n");
}
