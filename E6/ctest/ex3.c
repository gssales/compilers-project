// exemplo 3 da spec na secao C.1:
int mult (int z, int w)
{
    int x;
    if (z > 0) then {
        x = z * w;
    } else{
        x = w;
    };
    return x;
}
int main()
{
    int x;
    int y;
    x = 2;
    y = mult (x, x);
    return y;
}