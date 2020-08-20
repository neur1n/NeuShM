#include "neu_shm.h"

typedef struct
{
  int x;
  int y;
} Inner;

typedef struct
{
  Inner inner;
  int num;
} Outter;

#define BUF_SIZE 256

int main(int argc, char *argv[])
{
  float d0 = 12.0f;
  char d1 = 89;

  Outter data[2];
  data[0].inner.x = 1;
  data[0].inner.y = 2;
  data[0].num = 2;
  data[1].inner.x = 10;
  data[1].inner.y = 20;
  data[1].num = 2;

#ifdef _WIN32
  NeuShM shm("Global\\NeuShM", BUF_SIZE);
#elif __linux__
  NeuShM shm("/NeuShM", BUF_SIZE);
#endif

  NRESULT nr;

  nr = shm.Create();
  if (NeuFail(nr))
  {
    return -1;
  }

  nr = shm.Write<float>(&d0, sizeof(float), 0);
  if (NeuFail(nr)) { return -1; }

  nr = shm.Write<char>(&d1, sizeof(char), sizeof(float));
  if (NeuFail(nr)) { return -1; }

  nr = shm.Write<Outter>(data, 2*sizeof(Outter), sizeof(float)+sizeof(char));
  if (NeuFail(nr)) { return -1; }

  while (1)
  {
#ifdef _WIN32
    if (GetAsyncKeyState(0x50))  // Key P
    {
      break;
    }
#elif __linux__
#endif
  }

  return 0;
}
