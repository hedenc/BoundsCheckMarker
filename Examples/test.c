

int wild_deref(int *ptr, int off)
{
    return ptr[off];
}

int safe_deref()
{
    int arr[] = {1, 2, 3};
    return arr[2];
}

int wild_deref2(int *ptr)
{
    return *ptr;
}

int unmod_cast()
{
    int val = 3;
    int addr = (int) &val;
    return *(int *) addr;
}

int mod_cast()
{
    int val = 3;
    int addr = (int) &val | 1;
    return *(int *) addr;
}

int main()
{
    int arr[] = {1, 2, 3};
    arr[0] = wild_deref(arr, 3);
    return 0;
}