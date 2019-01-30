/*********************************************
  File:       CFiledef.h
  Purpose:    Class for Definition Files
  class name: CFiledef
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef _CFILEDEF_H_
#define _CFILEDEF_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define MAX_KEYS 50
#define MAX_SUBKEYS 5

/********************
 * DEFINITION OF KEY:
 * {
 *      Key value1 value2 ... value[MAX_SUBKEYS]
 * }
 *
 ********************/
typedef struct _key
{
	std::string key;
	int n_values;
	std::string values[MAX_SUBKEYS];
} key;

class CFileDef
{
public:
	/* File of definition */
	std::ifstream m_Filedef;

	/* Num of keys loaded */
	int m_numkeys;

private:
	/* name of object */
	std::string m_name;

	/* array of loaded values */
	key m_keys[MAX_KEYS];

	/* If there is an error */
	bool m_opened;

	std::string GetValidLine();

public:
	/* Constructor */
	CFileDef(const std::string &file);
	CFileDef(const std::string &file, bool binary);
	~CFileDef();

	bool getIsOpen();

	/* Gets the name of object */
	std::string getObjectName();

	/* Get the number of same objects */
	int getCount(const std::string &_key);

	/* Gets the values from key */
	std::string* getStringObjectValues(const std::string &_key);
	float* getFloatObjectValues(const std::string &_key);
	int* getIntObjectValues(const std::string &_key);

	/* Gets the values from key at the 'index' position */
	std::string* getObjectValuesIndex(const std::string &_key, int index);

	/* Get the keys from file */
	std::string getObjectKey(int ind);

	/* Gets the keys/values from file */
	void readObjectKeysValues();
};

#endif
