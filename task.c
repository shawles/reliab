int h;

		void f(int a, int b)
		{
/*1  */	  int x, y;
/*2  */	  x = 3;
/*3  */	  y = 1;
/*4  */	  h = a;
/*5  */	  if (h < y + a)
			{
/*6  */		  if (x > 7)
				{
/*7  */			  x = 2;
				}
/*8  */		  y = 8;
			}
		  else
			{
/*9  */		  if (x < 4)
				{
/*10 */			  x = 4;
				}
/*11 */		  x = 2;
			}
/*12 */	}

//12 состояний

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
