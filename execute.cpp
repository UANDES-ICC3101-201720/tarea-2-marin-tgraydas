#include <bits/stdc++.h>
#include <string>
using namespace std;
int main(int argc, char const *argv[])
{

    for (int i = 2; i < 101; i++)
    {
        string name = "./virtmem 100 ";
        string num = to_string(i);
        name += num + " " + argv[1] + " " + argv[2];
        const char *c = name.c_str();
        system(c);
    }
    return 0;
}
