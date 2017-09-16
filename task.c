int h;

void f(int a, int b)
{
  int x, y;
  x = 3;
  y = 1;
  h = a;
  if (h < y + a)
    {
      if (x > 7)
        {
          x = 2;
        }
      y = 8;
    }
  else
    {
      if (x < 4)
        {
          x = 4;
        }
      x = 2;
    }
}

void g(int a, int b)
{
  int x, y;
  x = 3;
  y = 5;
  h = 2;
  if (x > 6)
    {
      if (h > x - a)
        {
          h = b;
        }
      x = 3;
      h = x - b;
    }
  x = 1;
  while (x < 7)
    {
      if (h > 0)
        break;
      if (y < 5)
        {
          h = x;
        }
      if (h < b - x)
        {
          if (x < 10)
            {
              x = 3;
            }
          y = 0;
        }
    }
}
