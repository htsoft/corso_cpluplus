#include <iostream> 
#include <cstring> 
using namespace std;

int main()
{
    char src[] = "corsocpluplus";

    // Here destination is not large 
    // enough to store the src. so the 
    // behaviour of strcpy is unspecified. 
    // program may crashed, but its 
    // printing corsocpluplus 
    char dest[2];

    // copying src into dest. 
    strcpy(dest, src);
    cout << "Copied string: " << dest << endl;

    return 0;
}
