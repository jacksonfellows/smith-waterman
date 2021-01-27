#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S(a, b) ((a) == (b) ? +3 : -3)
#define W(k) (0 + 2 * (k))

#define AT(i, j, m) ((i) * ((m) + 1) + (j))

#define MAX(a, b) (a) > (b) ? (a) : (b)
#define MIN(a, b) (a) < (b) ? (a) : (b)

typedef unsigned int u_i;

void ph(u_i *H, size_t n, size_t m) {
  for (size_t i = 0; i <= n; i++) {
    for (size_t j = 0; j <= m; j++) {
      u_i e = H[AT(i, j, m)];
      u_i val = e >> 2;
      int source = e & 3;
      printf("%s%-3u ",
             source == 0 ? "+"
                         : (source == 1 ? "\\" : (source == 2 ? "|" : "-")),
             val);
    }
    printf("\n");
  }
}

void tb(u_i *H, char *A, char *B, size_t i, size_t j, size_t m, char *Am,
        char *rm, char *Bm, size_t mlen) {
  u_i e = H[AT(i, j, m)];
  u_i val = e >> 2;
  u_i source = e & 3;
  if (val == 0 || source == 0) {
    printf("%s\n%s\n%s\n", Am + mlen, rm + mlen, Bm + mlen);
    return;
  } else if (source == 1) {
    Am[mlen - 1] = A[i - 1];
    rm[mlen - 1] = A[i - 1] == B[j - 1] ? '|' : ' ';
    Bm[mlen - 1] = B[j - 1];
    tb(H, A, B, i - 1, j - 1, m, Am, rm, Bm, mlen - 1);
  } else if (source == 2) {
    Am[mlen - 1] = A[i - 1];
    rm[mlen - 1] = ' ';
    Bm[mlen - 1] = '-';
    tb(H, A, B, i - 1, j, m, Am, rm, Bm, mlen - 1);
  } else if (source == 3) {
    Am[mlen - 1] = '-';
    rm[mlen - 1] = ' ';
    Bm[mlen - 1] = B[j - 1];
    tb(H, A, B, i, j - 1, m, Am, rm, Bm, mlen - 1);
  }
}

void sm(char *A, char *B, size_t n, size_t m) {
  size_t size = (n + 1) * (m + 1);
  u_i *H = malloc(size * sizeof(u_i));
  int Adel, Bdel, same;
  u_i val, source;
  u_i maxv = 0;
  size_t maxi, maxj;
  for (size_t i = 0; i <= n; i++) {
    for (size_t j = 0; j <= m; j++) {
      if (!i || !j) {
        H[AT(i, j, m)] = 0;
      } else {
        Adel = 0;
        for (size_t k = 1; k <= i; k++) {
          int gap_k = ((int)H[AT(i - k, j, m)] >> 2) - (int)W(k);
          Adel = MAX(Adel, gap_k);
        }
        Bdel = 0;
        for (size_t l = 1; l <= j; l++) {
          int gap_l = ((int)H[AT(i, j - l, m)] >> 2) - (int)W(l);
          Bdel = MAX(Bdel, gap_l);
        }
        same = (int)(H[AT(i - 1, j - 1, m)] >> 2) + (int)S(A[i - 1], B[j - 1]);
        if (same < 0 && Adel < 0 && Bdel < 0) {
          val = 0;
          source = 0;
        } else if (same > Adel && same > Bdel) {
          val = same;
          source = 1;
        } else if (Adel > Bdel) {
          val = Adel;
          source = 2;
        } else {
          val = Bdel;
          source = 3;
        }
        if (val > maxv) {
          maxv = val;
          maxi = i;
          maxj = j;
        }
        H[AT(i, j, m)] = (val << 2) | source;
      }
    }
  }
  ph(H, n, m);
  if (maxv == 0) {
    printf("no match\n");
    return;
  }
  size_t maxlen = MIN(maxi, maxj) + 1;
  char *Amatch = malloc(maxlen * sizeof(char));
  char *rmatch = malloc(maxlen * sizeof(char));
  char *Bmatch = malloc(maxlen * sizeof(char));
  Amatch[maxlen - 1] = rmatch[maxlen - 1] = Bmatch[maxlen - 1] = '\0';
  tb(H, A, B, maxi, maxj, m, Amatch, rmatch, Bmatch, maxlen - 1);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "requires 2 arguments\n");
    return 1;
  }
  sm(argv[1], argv[2], strlen(argv[1]), strlen(argv[2]));
  return 0;
}
