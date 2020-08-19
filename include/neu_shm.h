#ifndef NEU_SHM_H
#define NEU_SHM_H

#include <string>
#include "neu_shm/neu.h"

#define USE_POSIX 1

#ifdef _WIN32
#include "neu_shm/win_shm.h"
#elif __linux__
#if USE_POSIX 
#include "neu_shm/posix_shm.h"
#endif
#endif

class NeuShM
{
public:
  NeuShM(const std::string &name, const int &size);
  ~NeuShM();

  NRESULT Create();  // Create for writting.

  template<class T>
  NRESULT Write(T *data, unsigned int bytes, unsigned int shift_bytes);

  NRESULT Open();  // Open for reading.

  template<class T>
  NRESULT Read(T **data, unsigned int bytes, unsigned int shift_bytes);

private:
#ifdef _WIN32
  WinShM *m_shm;
#elif __linux__
#if USE_POSIX
  POSIXShM *m_shm;
#endif
#endif
};

//******************************************************************* Public{{{
NeuShM::NeuShM(const std::string &name, const int &size)
  :m_shm(nullptr)
{
#ifdef _WIN32
  this->m_shm = new WinShM(name, size);
#elif __linux__
#if USE_POSIX
  this->m_shm = new POSIXShM(name, size);
#endif
#endif
}

NeuShM::~NeuShM()
{
  NeuRelease(this->m_shm);
  NeuInfo("NeuShM exits.");
}

NRESULT NeuShM::Create()
{
  return this->m_shm->Create();
}

template<class T>
NRESULT NeuShM::Write(T *data, unsigned int bytes, unsigned int shift_bytes)
{
  return this->m_shm->Write(data, bytes, shift_bytes);
}


NRESULT NeuShM::Open()
{
  return this->m_shm->Open();
}

template<class T>
NRESULT NeuShM::Read(T **data, unsigned int bytes, unsigned int shift_bytes)
{
  return this->m_shm->Read(data, bytes, shift_bytes);
}
//******************************************************************* Public}}}

//****************************************************************** Private{{{
//****************************************************************** Private}}}

#endif  // NEU_SHM_H
