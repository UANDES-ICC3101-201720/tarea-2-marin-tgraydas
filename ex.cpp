#include <bits/stdc++.h>
#include <string>
using namespace std;

int main(int argc, char const *argv[])
{
    string array[9] = {"fifo-focus.txt", "fifo-scan.txt", "fifo-sort.txt", "random-sort.txt", "random-scan.txt", "random-focus.txt", "custom-sort.txt", "custom-scan.txt", "custom-focus.txt"};
    string arr[9] = {"fifo focus", "fifo scan", "fifo sort", "random sort", "random scan", "random focus", "custom sort", "custom scan", "custom focus"};
    for (int i = 0; i < 9; i++)
    {
        string name = "./a.out " + arr[i] + " > " + array[i];
        const char *c = name.c_str();
        system(c);
    }
    return 0;
}
