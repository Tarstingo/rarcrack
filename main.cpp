#include<fstream>
#include"crackrar.h"

using namespace std;

//salt 8byte(21-28)
static std::vector<muchar> salt;
//iv   16byte(29-44)
static std::vector<muchar> iv;
//s    16byte(45-60)
static std::vector<muchar> s;

int main()
{
    int fd=open("rar.rar",O_RDONLY);
    if(fd<=0)
    {
        perror("can not open file");
        return 1;
    }
    getPara(fd,salt,iv,s);
    close(fd);
    printf("salt\033[31m\n");
    prtvector(salt);
    printf("\033[miv\033[31m\n");
    prtvector(iv);
    printf("\033[ms\033[31m\n");
    prtvector(s);
    printf("\033[m");


    printf("Now We start cracking...\n");

    muchar key[20];

    struct timespec startime,nowtime,tmptime;
    clock_gettime(CLOCK_REALTIME,&startime);
    nowtime.tv_nsec=startime.tv_nsec;
    nowtime.tv_sec=startime.tv_sec;
    cout<<"time "<<startime.tv_sec<<":"<<startime.tv_nsec<<endl;
    unsigned int long numkeys=0;
    ifstream fkey;
    fkey.open("keyfile");
    while(1)
    {
        numkeys++;
        if(numkeys%5==0)
        {
            tmptime.tv_nsec=nowtime.tv_nsec;
            tmptime.tv_sec=nowtime.tv_sec;
            clock_gettime(CLOCK_REALTIME,&nowtime);
            if(tmptime.tv_nsec>nowtime.tv_nsec)
            {
                cout<<"test 5 keys,costs "<<nowtime.tv_sec-tmptime.tv_sec-1<<"seconds and "<<1000000000-tmptime.tv_nsec+nowtime.tv_nsec<<"nanoseconds"<<endl;
            }
            else
            {
                cout<<"test 5 keys,costs "<<nowtime.tv_sec-tmptime.tv_sec<<"seconds and "<<nowtime.tv_nsec-tmptime.tv_nsec<<"nanoseconds"<<endl;
            }
        }
        fkey>>key;
        if(fkey.fail())
        {
            cout<<"The key is not in the file"<<endl;
            break;
        }
        else if(testKey(salt,s,iv,key)==1)
        {
            cout<<"Key founded;"<<endl;
            cout<<"We have tested "<<numkeys<<" keys"<<endl;
            cout<<key<<endl;
            break;
        }
    }
    fkey.close();

    clock_gettime(CLOCK_REALTIME,&nowtime);
    if(startime.tv_nsec>nowtime.tv_nsec)
    {
        cout<<"it costs "<<nowtime.tv_sec-startime.tv_sec-1<<"seconds and "<<1000000000-startime.tv_nsec+nowtime.tv_nsec<<"nanoseconds"<<endl;
    }
    else
    {
        cout<<"it costs "<<nowtime.tv_sec-startime.tv_sec<<"seconds and "<<nowtime.tv_nsec-startime.tv_nsec<<"nanoseconds"<<endl;
    }

    return 0;
}
