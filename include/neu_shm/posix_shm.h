#ifndef POSIX_SHM_H
#define POSIX_SHM_H

#include <errno.h>
#include <fcntl.h>  // shm_open macro arguments
#include <sys/mman.h>  // shm_open, mmap, shm_unlink
#include <sys/unistd.h>  // ftruncate
#include <string>
#include "neu.h"

class POSIXShM
{
public:
  POSIXShM(const std::string &name, const int &size);
  ~POSIXShM();

  NRESULT Create();  // Create for writting.

  template<class T>
  NRESULT Write(T *data, int count);

  NRESULT Open();  // Open for reading.

  template<class T>
  NRESULT Read(T **data, int *count);
private:
  NRESULT SetName(const std::string &name);
  NRESULT Release();

  int m_mapping;
  void *m_buf;

  char *m_name;
  int m_size;
};

//******************************************************************* Public{{{
POSIXShM::POSIXShM(const std::string &name, const int &size)
  :m_mapping(0), m_buf(nullptr), m_name(nullptr), m_size(size)
{
  this->SetName(name);
}

POSIXShM::~POSIXShM()
{
  this->Release();
}

NRESULT POSIXShM::Create()
{
  this->m_mapping = shm_open(this->m_name, O_CREAT | O_RDWR, 0666);

  if (this->m_mapping == -1)
  {
    NeuErr("Failed to create file mapping. (%d)", errno);
    return NEU_FAIL;
  }

  int result = ftruncate(this->m_mapping, this->m_size);
  if (result != 0)
  {
    NeuErr("Failed to truncate file mapping. (%d)", errno);
    return NEU_FAIL;
  }

  return NEU_OK;
}

template<class T>
NRESULT POSIXShM::Write(T *data, int count)
{
  this->m_buf = mmap(NULL, this->m_size, PROT_WRITE, MAP_SHARED, this->m_mapping, 0);

  if (this->m_buf == MAP_FAILED)
  {
    NeuErr("Failed to map view of file for writting. (%d)", errno);
    this->Release();
    return NEU_FAIL;
  }

  memcpy((T*)this->m_buf, &count, sizeof(int));
  memcpy(((T*)this->m_buf + sizeof(int)), data, (count * sizeof(T)));
  munmap(this->m_buf, this->m_size);

  return NEU_OK;
}

NRESULT POSIXShM::Open()
{
  this->m_mapping = shm_open(this->m_name, O_RDONLY, 0666);

  if (this->m_mapping < 0)
  {
    NeuErr("Failed to open file mapping object for reading (%d).", errno);
    return NEU_FAIL;
  }

  return NEU_OK;
}

template<class T>
NRESULT POSIXShM::Read(T **data, int *count)
{
  this->m_buf = mmap(NULL, this->m_size, PROT_READ, MAP_SHARED, this->m_mapping, 0);

  if (this->m_buf == MAP_FAILED)
  {
    NeuErr("Failed to map view of file for reading (%d).", errno);
    this->Release();
    return NEU_FAIL;
  }

  memcpy(count, (T*)this->m_buf, sizeof(int));

  *data = new T[*count];
  memcpy(*data, ((T*)this->m_buf + sizeof(*count)), ((*count) * sizeof(T)));

  munmap(this->m_buf, this->m_size);

  return NEU_OK;
}
//******************************************************************* Public}}}

//****************************************************************** Private{{{
NRESULT POSIXShM::SetName(const std::string &name)
{
  size_t size = name.size();
  this->m_name = new char[size+1];
  copy(name.begin(), name.end(), this->m_name);
  this->m_name[size] = '\0';
  return NEU_OK;
}

NRESULT POSIXShM::Release()
{
  if (this->m_buf != nullptr)
  {
    munmap(this->m_buf, this->m_size);
    this->m_buf = nullptr;
  }
  
  if (this->m_mapping != 0)
  {
    shm_unlink(this->m_name);
    this->m_mapping = 0;
  }

  NeuRelease(this->m_name);
  
  return NEU_OK;
}
//****************************************************************** Private}}}

#endif  // POSIX_SHM_H
