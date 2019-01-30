/*********************************************
  File:       CXMLParser.h
  Purpose:    Class for parsing a litte xml file
  class name: CXMLParser
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef CXMLPARSER_H_
#define CXMLPARSER_H_

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>

/*******************
 * NODES STRUCTURE *
 *******************/
struct Pair
{
	std::string name;
	std::string value;
};

struct Node
{
	//Label of the node
	std::string label;

	//List of pairs: (name, value)
	std::vector<Pair> list_of_values;
};

struct xmlinfo
{
	std::string version;
	std::string encode;
	std::vector<Node> info_lst;
};

/****************
 * LABEL STRUCT *
 ****************/
/*
 * For example: <GAMEOBJECT name="terrain"> => label=GAMEOBJECT value={name, terrain}
 *           or <TYPE>value</TYPE> => label="" value={"TYPE","value"}
 */
/*struct Label
{
	int level;
	std::string label;
	std::vector<Pair> value;
};*/

/*
 * _ERROR => Error parsing a label
 * _LABEL => <LABEL> or <LABEL value="..."></LABEL>
 * _LABEL_END => </LABEL>
 * _VALUE => <LABEL>VALUE</LABEL>
 */
enum PARSED { _ERROR = 0, _XML_INFO, _LABEL, _LABEL_VALUE, _VALUE, _LABEL_END, _END };

class CXMLParser
{
public:
	CXMLParser();
	~CXMLParser();

	bool LoadXMLFile(const std::string& file);

	int getNodesCount();
	bool getNodes(const std::string& label);
	std::string getAttributeValue(const std::string& name);

	void Next();

private:
	//Tree for stores the info
	xmlinfo m_xmlInfo;

	//Xml file to parse
	std::ifstream m_xmlFile;

	//Num total of nodes
	std::size_t m_nodes;

	//Index for search nodes
	std::size_t m_index;

	//Search for tag. Returns the kind of readed label
	//<ROOT>
	//	...
	//</ROOT>
	PARSED SearchTag(Node &n);

	//Methods for parsing xml tags and chars
	PARSED ParseAnyTag(const std::string& tag, Node &n);
	bool ParseLabelCharacter(const char c); //<...> => A..Z,a..z,_,0..9
	bool ParseValueCharacter(const char c); //<...> => A..Z,a..z,_,0..9,\,.,_

	void Initialize();
	void Release();

public:
	//For visualize the loaded data
	void debug();
};

#endif /* CXMLPARSER_H_ */
