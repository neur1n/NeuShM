#ifndef WIN_SHM_H
#define WIN_SHM_H

#include <string>
#include <windows.h>
#include "neu.h"

class WinShM
{
public:
  WinShM(const std::string &name, const int &size);
  ~WinShM();

  NRESULT Create();  // Create for writting.

  template<class T>
  NRESULT Write(T *data, unsigned int bytes, unsigned int shift_bytes);

  NRESULT Open();  // Open for reading.

  template<class T>
  NRESULT Read(T **data, unsigned int bytes, unsigned int shift_bytes);

private:
  NRESULT SetName(const std::string &name);
  NRESULT Release();

  HANDLE m_mapping;
  LPVOID m_buf;

  char *m_name;
  int m_size;
};

//******************************************************************* Public{{{
WinShM::WinShM(const std::string &name, const int &size)
  :m_mapping(NULL), m_buf(NULL), m_name(nullptr), m_size(size)
{
  this->SetName(name);
}

WinShM::~WinShM()
{
  this->Release();
}

NRESULT WinShM::Create()
{
  this->m_mapping = CreateFileMapping(
      INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, this->m_size, this->m_name);

  if (this->m_mapping == NULL)
  {
    NeuErr("Failed to create file mapping. (%d)", (int)GetLastError());
    return NEU_FAIL;
  }

  return NEU_OK;
}

template<class T>
NRESULT WinShM::Write(T *data, unsigned int bytes, unsigned int shift_bytes)
{
  this->m_buf = MapViewOfFile(this->m_mapping, FILE_MAP_ALL_ACCESS, 0, 0, this->m_size);

  if (this->m_buf == NULL)
  {
    NeuErr("Failed to map view of file for writting. (%d)", (int)GetLastError());
    this->Release();
    return NEU_FAIL;
  }

  CopyMemory((T*)((char*)this->m_buf + shift_bytes), data, bytes);
  // UnmapViewOfFile(this->m_buf);

  return NEU_OK;
}

NRESULT WinShM::Open()
{
  this->m_mapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, this->m_name);

  if (this->m_mapping == NULL)
  {
    NeuErr("Failed to open file mapping object for reading (%d).", (int)GetLastError());
    return NEU_FAIL;
  }

  return NEU_OK;
}

template<class T>
NRESULT WinShM::Read(T **data, unsigned int bytes, unsigned int shift_bytes)
{
  this->m_buf = MapViewOfFile(this->m_mapping, FILE_MAP_ALL_ACCESS, 0, 0, this->m_size);

  if (this->m_buf == NULL)
  {
    NeuErr("Failed to map view of file for reading (%d).", (int)GetLastError());
    this->Release();
    return NEU_FAIL;
  }

  *data = new T[bytes/sizeof(T)];
  CopyMemory(*data, (T*)((char*)this->m_buf + shift_bytes), bytes);
  // UnmapViewOfFile(this->m_buf);

  return NEU_OK;
}
//******************************************************************* Public}}}

//****************************************************************** Private{{{
NRESULT WinShM::SetName(const std::string &name)
{
  size_t size = name.size();
  this->m_name = new char[size+1];
  copy(name.begin(), name.end(), this->m_name);
  this->m_name[size] = '\0';
  return NEU_OK;
}

NRESULT WinShM::Release()
{
  if (this->m_buf != NULL)
  {
    UnmapViewOfFile(this->m_buf);
    this->m_buf = NULL;
  }
  
  if (this->m_mapping != NULL)
  {
    CloseHandle(this->m_mapping);
    this->m_mapping = NULL;
  }

  NeuRelease(this->m_name);
  
  return NEU_OK;
}
//****************************************************************** Private}}}

#endif  // WIN_SHM_H
