#include <stdio.h>
#include <stdlib.h>

int main ()
{
    int count = 20;
    int L, R, tL, tR;
    int l1, l2, l3, r1, r2, r3;
    int tl1, tl2, tl3, tr1, tr2, tr3;
    char A[3], B[3];

    FILE *fp;
    char *line = NULL;
    size_t len;
    ssize_t read;

    fp = fopen("/dev/rfcomm0", "r");
    //fp = fopen("/home/ubuntu/LEC/left.txt", "r");
    if (fp == NULL) {
        return -1;
    }

    int k = 0;
    L = 0;	R = 0;
	tl1 = tl2 = tl3 = 0;
	l1 = l2 = l3 = 0;
	tr1 = tr2 = tr3 = 0;
	r1 = r2 = r3 = 0;
    int space = 20;
	int wait = 0;
	while (true) {
    	if ((read = getline(&line, &len, fp)) != -1) {
		    sscanf(line, "%i %i %i %i %i %i %s %s", &tl1, &tl2, &tl3, &tr1, &tr2, &tr3, A, B);
		    //printf("%i %i %i %i %i %i %s %s\n", tl1, tl2, tl3, tr1, tr2, tr3, A, B);
		    tR = (int)(A[2] - '0');
		    tL = (int)(B[2] - '0');
		    // printf("R = %i; L = %i\n", R, L);

		    l1 |= tl1;  r1 |= tr1;
		    l2 |= tl2;  r2 |= tr2;
		    l3 |= tl3;  r3 |= tr3;

		    //L |= tL;    R |= tR;
			L = l1+l2+l3;
			R = r1+r2+r3;

			if (L > 3 || R > 3) {
				printf("R = %i; L = %i\n", R, L);
				continue;
			}

			/* after reading count consecutive values save them */
		   if (k == count) {
		   //printf("R = %i; L = %i\n", R, L);
				if (L == 0 && R == 0) {
					wait++;
					if (wait == 10) {
						space = 20;
						wait = 0;
					}
				}
			    else if (L > R) {
			        int nr = L - R;
			        space += nr;
			        for (int i=0; i<space; i++) {
			            printf(" ");
			        }
			        for (int i=0; i<nr; i++) {
			            printf("\\");
			        }
			        printf("\n");
			    } else if (R > L) {
			        int nr = R - L;
			        space -= nr;
			        for (int i=0; i<space; i++) {
			            printf(" ");
			        }
			        for (int i=0; i<nr; i++) {
			            printf("/");
			        }
			        printf("\n");
			    } else {
			        for (int i=0; i<space; i++) {
			            printf(" ");
			        }
			        printf("||\n");
			        // debug
			    }

			    l1 = l2 = l3 = 0;
			    r1 = r2 = r3 = 0;
			    L = R = 0;
			    k = 0;
			}

			k++;
		}
	}

    return 0;
}
