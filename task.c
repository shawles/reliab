int h;

void f(int a, int b)
{
  int x, y; 		//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {} 
  x = 3;			//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {}
  y = 1;			//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {}
  h = a;			//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {}
  if (h < y + a)	//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {}
    {
      if (x > 7)	//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {}
        {
          x = 2;	//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {}
        }
      y = 8;		//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {}
    }
  else 				//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {}
    {
      if (x < 4)	//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {}
        {
          x = 4;	//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {}
        }
      x = 2;		//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {}
    }
}					//h = {}, f_a = [], f_b = [], f_x = {}, f_y = {}

void g(int a, int b)
{
  int x, y;			//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
  x = 3;			//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
  y = 5;			//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
  h = 2;			//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
  if (x > 6)		//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
    {
      if (h > x - a)//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
        {
          h = b;	//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
        }
      x = 3;		//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
      h = x - b;	//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
    }
  x = 1;			//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
  while (x < 7)		//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
    {
      if (h > 0)	//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
        break;		//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
      if (y < 5)	//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
        {
          h = x;	//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
        }
      if (h < b - x)//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
        {
          if (x < 10)//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
            {
              x = 3;//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
            }
          y = 0;	//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
        }
    }
}					//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
