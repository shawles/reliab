int h;

void f(int a, int b)
{
  int x, y; 		//h = {#, 2, g}, f_a = [-i, i], f_b = [-i, i], f_x = {#}, f_y = {#} 
  x = 3;			//h = {#, 2, g}, f_a = [-i, i], f_b = [-i, i], f_x = {#}, f_y = {#}
  y = 1;			//h = {#, 2, g}, f_a = [-i, i], f_b = [-i, i], f_x = {3}, f_y = {#}
  h = a;			//h = {#, 2, g}, f_a = [-i, i], f_b = [-i, i], f_x = {3}, f_y = {1}
  if (h < y + a)	//h = {f_a, 2, g}, f_a = [-i, i], f_b = [-i, i], f_x = {3}, f_y = {1}
    {
      y = 8;		//h = {f_a, 2 if f_a > 1, g in g < 1 + f_a, g}, f_a = [], f_b = [], f_x = {3}, f_y = {1}
    }
  else 				//h = {2 if f_a <= 1, g if g >= 1 + f_a, g}, f_a = [], f_b = [], f_x = {3}, f_y = {1}
    {
      if (x < 4)	//h = {2 if f_a <= 1, g if g >= 1 + f_a, g}, f_a = [], f_b = [], f_x = {3}, f_y = {1}
        {
          x = 4;	//h = {2 if f_a <= 1, g if g >= 1 + f_a, g}, f_a = [], f_b = [], f_x = {3}, f_y = {1}
        }
      x = 2;		//h = {2 if f_a <= 1, g if g >= 1 + f_a, g}, f_a = [], f_b = [], f_x = {4}, f_y = {1}
    }
}					//h = {2, f_a, g}, f_a = [], f_b = [], f_x = {3, 4}, f_y = {8, 1}

//f может сменить h только на f_a, причём это f_a не меняется вообще

void g(int a, int b)
{
  int x, y;			//h = {#, f_a}, g_a = [-int, int], g_b = [-int, int], g_x = {#}, g_y = {#}
  x = 3;			//h = {#, f_a}, g_a = [-int, int], g_b = [-int, int], g_x = {#}, g_y = {#}
  y = 5;			//h = {#, f_a}, g_a = [-int, int], g_b = [-int, int], g_x = {3}, g_y = {#}
  h = 2;			//h = {#, f_a}, g_a = [-int, int], g_b = [-int, int], g_x = {3}, g_y = {5}
  x = 1;			//h = {2, f_a}, g_a = [-int, int], g_b = [-int, int], g_x = {3}, g_y = {5}
  while (x < 7)		//h = {2, f_a, 3 if 6 < g_b}, g_a = [-int, int], g_b = [-int, int], g_x = {1, 3}, g_y = {5, 0}
    {
      if (h > 0)	//h = {2, f_a, 3 if 6 < g_b}, g_a = [-int, int], g_b = [-int, int], g_x = {1, 3}, g_y = {5, 0}
        break;		//h = {2, f_a, 3 if 6 < g_b}, g_a = [-int, int], g_b = [-int, int], g_x = {1, 3}, g_y = {5, 0}
      if (y < 5)	//h = {f_a}, g_a = [-int, int], g_b = [-int, int], g_x = {1, 3}, g_y = {5, 0}
        {
          h = x;	//h = {f_a}, g_a = [-i, i], g_b = [-i, i], g_x = {3}, g_y = {0}
        }
      if (h < b - x)//h = {f_a, 3}, g_a = [-i, i], g_b = [-i, i], g_x = {1, 3}, g_y = {5, 0}
        {
          if (x < 10)//h = {f_a, 3 if 6 < g_b}, g_a = [-i, i], g_b = [-i, i...], g_x = {1, 3}, g_y = {5, 0}
            {
              x = 3;//h = {f_a, 3 if 6 < g_b}, g_a = [-i, i], g_b = [-i, i...], g_x = {1, 3}, g_y = {5, 0}
            }
          y = 0;	//h = {f_a, 3 if 6 < g_b}, g_a = [-i, i], g_b = [-i, i...], g_x = {3}, g_y = {5, 0}
        }
    }
}					//h = {}, g_a = [], g_b = [], g_x = {}, g_y = {}
//g может сменить h только на f_х = 3
