#ifndef CRACKRAR_H
#define CRACKRAR_H
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include <time.h>
#include<vector>
#include<openssl/sha.h>
#include<openssl/aes.h>

typedef unsigned char muchar;



//overload the operator +
const std::vector<muchar> operator+(const std::vector<muchar>,const std::vector<muchar>);

std::vector<muchar> toWideChar(muchar *pass);

//get salt,iv,s from file descriptor fd
void getPara(int fd,std::vector<muchar> &salt,std::vector<muchar> &iv,std::vector<muchar> &s);



//the main loop, calculate sha1 digest and store it in B
void Bsha1(std::vector<muchar> M,muchar *B);

//Calculate C
void CAes(muchar *B,std::vector<muchar> s,muchar *C);

//test if the key if right
int testKey(std::vector<muchar> salt,std::vector<muchar> s,std::vector<muchar> iv,muchar *key);


//print the hex values of a vector
void prtvector(std::vector<muchar>);
#endif // CRACKRAR_H
