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
#ifdef _WIN32
  NeuShM shm("Global\\NeuShM", BUF_SIZE);
#elif __linux__
  NeuShM shm("/NeuShM", BUF_SIZE);
#endif

  float *d0 = 0;
  char *d1 = 0;
  Outter *data = nullptr;

  NRESULT nr;

  nr = shm.Open();
  if (NeuFail(nr))
  {
    return -1;
  }

  nr = shm.Read<float>(&d0, sizeof(float), 0);
  if (NeuFail(nr)) { return -1; }

  nr = shm.Read<char>(&d1, sizeof(char), sizeof(float));
  if (NeuFail(nr)) { return -1; }

  nr = shm.Read<Outter>(&data, 2*sizeof(Outter), sizeof(float)+sizeof(char));
  if (NeuFail(nr)) { return -1; }

  NeuInfo("d0: %f, d1: %d", *d0, *d1);

  for (int i = 0; i < 2; i++)
  {
    NeuInfo("x: %d, y: %d, num: %d", data[i].inner.x, data[i].inner.y, data[i].num);
  }

  delete data;

  return 0;
}
