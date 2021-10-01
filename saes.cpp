#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<cstring>
#include<unistd.h>
using namespace std;
//Simplified AES
class AES
{
public:
string key, k1, k2, k3, w0, w1, w2, w3, w4, w5;
string plain, encrypt, decrypt;
//SBOX
unordered_map<string, string> sbox {{"0000", "1001"}, {"0001", "0100"}, {"0010",
"1010"}, {"0011", "1011"},
{"0100", "1101"}, {"0101", "0001"}, {"0110", "1000"}, {"0111", "0101"}, {"1000", "0110"},
{"1001", "0010"}, {"1010", "0000"}, {"1011", "0011"}, {"1100", "1100"}, {"1101", "1110"},
{"1110", "1111"}, {"1111", "0111"} };
//Inverse of Sbox
unordered_map<string, string> rsbox {{"0000", "1010"}, {"0001", "0101"}, {"0010",
"1001"}, {"0011", "1011"},
{"0100", "0001"}, {"0101", "0111"}, {"0110", "1000"}, {"0111", "1111"}, {"1000", "0110"},
{"1001", "0000"}, {"1010", "0010"}, {"1011", "0011"}, {"1100", "1100"}, {"1101", "0100"},
{"1110", "1101"}, {"1111", "1110"} };
void initialize(string k)
{
if(k.size()!=16){
cout<<"\n InValid Key-Length! Please try again... "<<k<<" "<<k.size();
exit(1);
}
key = k;
Key_Formation();
}
void Key_Formation()
{
int i=0;
string sub = "";
w0 = key.substr(0,8);
w1 = key.substr(8,8);
k1 = w0+w1; //K1 = W0W1
// w2 = w0 XOR 10000000 XOR substitute(Rotate(w1))
string temp = "10000000";
w2 = EX_OR(w0,temp);
temp = Rotate(w1);
while(i<temp.length())
{
sub = sub + substituteNibble(temp.substr(i, 4));
i=i+4;
}
temp = sub;
w2 = EX_OR(w2, temp);
w3 = EX_OR(w2, w1);
k2 = w2 + w3; // K2 = W2W3
// w4 = w2 XOR 00110000 XOR substitute(Rotate(w3))
temp = "00110000";
w4 = EX_OR(w2,temp);
temp = Rotate(w3);
sub="";
i=0;
while(i<temp.length())
{
sub = sub + substituteNibble(temp.substr(i, 4));
i=i+4;
}
temp = sub;
w4 = EX_OR(w4, temp);
w5 = EX_OR(w4, w3);
k3 = w4+w5; //K3 = W4W5
cout<<"\nKey K1 : "<<k1;
cout<<"\nKey K2 : "<<k2;
cout<<"\nKey K3 : "<<k3;
cout<<"\n\n";
}
string EX_OR(string a,string b)
{
string output = "";
for(int i=0;i<a.size();i++)
{
if(a[i] == b[i])
output += "0";
else 
output += "1";
}
return output;
}
char exor(char a,char b)
{
if(a==b){
return '0';
}
return '1';
}
string substituteNibble(string a) //passing only one nibble at a time 
{
for(auto it=sbox.begin();it!=sbox.end();it++)
{
if(a.compare(it->first)==0)
{
return (it->second);
}
}
return "";
}
string invSubstituteNibble(string a) //passing only one nibble at a time 
{
for(auto it=rsbox.begin();it!=rsbox.end();it++)
{
if(a.compare(it->first)==0)
{
return (it->second);
}
}
return "";
}
string Rotate(string a)
{
string temp = "";
temp = temp + a.substr(4,4) + a.substr(0, 4);
return temp;
}
string invMixColumn(string p)
{
string Me[2][2] = {{"1001", "0010"}, {"0010", "1001"}};
string Sm[2][2];
Sm[0][0] = p.substr(0,4);
Sm[1][0] = p.substr(4,4);
Sm[0][1] = p.substr(8,4);
Sm[1][1] = p.substr(12,4);
string res = "";
res = res + multiply(Me[0][0], Me[0][1], Sm[0][0], Sm[1][0]);
res = res + multiply(Me[1][0], Me[1][1], Sm[0][0], Sm[1][0]);
res = res + multiply(Me[0][0], Me[0][1], Sm[0][1], Sm[1][1]);
res = res + multiply(Me[1][0], Me[1][1], Sm[0][1], Sm[1][1]);
return res;
}
string MixColumn(string p)
{
string Me[2][2] = {{"0001", "0100"}, {"0100", "0001"}};
string Sm[2][2];
Sm[0][0] = p.substr(0,4);
Sm[1][0] = p.substr(4,4);
Sm[0][1] = p.substr(8,4);
Sm[1][1] = p.substr(12,4);
string res = "";
res = res + multiply(Me[0][0], Me[0][1], Sm[0][0], Sm[1][0]);
res = res + multiply(Me[1][0], Me[1][1], Sm[0][0], Sm[1][0]);
res = res + multiply(Me[0][0], Me[0][1], Sm[0][1], Sm[1][1]);
res = res + multiply(Me[1][0], Me[1][1], Sm[0][1], Sm[1][1]);
return res;
}
string multiply(string a, string b, string c, string d)
{
string temp1;
string temp2;
int dec1 = stoi(a,0,2)*stoi(c,0,2);
int dec2 = stoi(b,0,2)*stoi(d,0,2);
temp1 = decToBin(dec1);
temp2 = decToBin(dec2);
if(dec1>128)
{
// temp1 = "1110";
dec1 = dec1-128;
dec1 = dec1<<1;
temp1 = decToBin(dec1);
}
else if(dec1 > 15)
{
temp1 = reduce(temp1);
}
if(dec2>128)
{
// temp2 = "1110";
dec2 = dec2-128;
dec2 = dec2<<1;
temp2 = decToBin(dec2);
}
else if(dec2 > 15)
{
temp2 = reduce(temp2);
}
// cout<<"\nTemp1: "<<temp1;
// cout<<"\nTemp2: "<<temp2;
temp1 = EX_OR(temp1, temp2);
return temp1;
}
//Reducing function -----------------------------------------------------------------------------------
/*
1 0 0 1 0 1 ---> rem
1 0 0 1 1 | ---> divisor
------------
rem 1
*/
string reduce(string s)
{
string key = "10011";
int datalen = s.length();
int keylen = key.length();
string red = "";
for(int i=0;i<keylen-1;i++)
red = red + "0";
string temp;
string rem;
for(int i=0;i<keylen;i++)
rem[i]=s[i];
for(int j=keylen;j<=datalen;j++)
{
for(int i=0;i<keylen;i++)
temp[i]=rem[i]; // remainder of previous step is divident of current
step
if(rem[0]=='0') //if 1's bit of remainder is 0 then shift the rem by 1 bit
{
for(int i=0;i<keylen-1;i++)
rem[i]=temp[i+1];
}
else //else exor the divident with generator key
{
for(int i=0;i<keylen-1;i++)
rem[i]=exor(temp[i+1],key[i+1]);
}
if(j!=datalen)
rem[keylen-1]=s[j]; //appending next bit of data to remainder obtain by
division
else 
rem[keylen-1]='\0';
}
for(int i=0;i<keylen-1;i++)
red[i]=rem[i]; //replace n-1 zeros with n-1 bit remainder
return red;
}
string decToBin(int n)
{
string s = bitset<64> (n).to_string();
const auto loc1 = s.find('1');
if(loc1 != string::npos)
{
string temp = s.substr(loc1);
if(temp.length() < 4)
{
return s.substr(loc1-(4-temp.length()));
}
return s.substr(loc1);
}
return "0";
}
//ENCRYPTION
void Encryption()
{
string p = plain;
string sub = "";
int i=0;
//Add round key 1
p = EX_OR(p, k1);
//subs nibble 
while(i<p.length())
{
sub = sub + substituteNibble(p.substr(i, 4));
i=i+4;
}
p = sub;
sub="";
//shift row
sub = p.substr(0,4) + p.substr(12,4) + p.substr(8,4) + p.substr(4,4);
p = sub;
sub="";
//Mix Column Matrix
p = MixColumn(p);
//Add round key 2
p = EX_OR(p, k2);
//subs nibble 
i=0;
while(i<p.length())
{
sub = sub + substituteNibble(p.substr(i, 4));
i=i+4;
}
p = sub;
sub="";
//shift row
sub = p.substr(0,4) + p.substr(12,4) + p.substr(8,4) + p.substr(4,4);
p = sub;
sub="";
//Add round key 3
p = EX_OR(p, k3);
encrypt = p;
cout<<"\n\n Encrypted Cipher Text : "<<encrypt<<" \n\n";
}
//DECRYPTION
void Decryption()
{
string p = encrypt;
string sub = "";
int i=0;
//Add round key 3
p = EX_OR(p, k3);
//Inverse shift row
sub = p.substr(0,4) + p.substr(12,4) + p.substr(8,4) + p.substr(4,4);
p = sub;
sub="";
//Inverse subs nibble 
while(i<p.length())
{
sub = sub + invSubstituteNibble(p.substr(i, 4));
i=i+4;
}
p = sub;
sub="";
//Add round key 2
p = EX_OR(p, k2);
//Mix Column Matrix
p = invMixColumn(p);
//Inverse shift row
sub = p.substr(0,4) + p.substr(12,4) + p.substr(8,4) + p.substr(4,4);
p = sub;
sub="";
//Inverse subs nibble 
i=0;
while(i<p.length())
{
sub = sub + invSubstituteNibble(p.substr(i, 4));
i=i+4;
}
p = sub;
sub="";
//Add round key 1
p = EX_OR(p, k1);
decrypt = p;
cout<<"\n\n Decrypted plain Text : "<<decrypt<<"\n\n";
}
};
int main()
{
string key, plain;
cout<<"Enter the 16 bit Key : ";
cin>>key;
AES S;
S.initialize(key);
cout<<"Enter the 16 bit plain text : ";
cin>>S.plain;
S.Encryption();
cout<<"\n Decrypting the cipher text ...\n\n";
sleep(2);
S.Decryption();
return 0;
}
