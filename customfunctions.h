// Number clamp function
int clamp(int x, int xmin, int xmax)
{
    if (x < xmin)
        return xmin;
    else if (x > xmax)
        return xmax;
    return x;
}
