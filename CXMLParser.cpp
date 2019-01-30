/*
 * CXmlParser.cpp
 *
 *  Created on: 16 de ene. de 2018
 *      Author: fjimartinez
 */

//xml m_xmlFile structure example

/*<LABELS>
  	  <LABEL value="name" ...>
  	      <VALUE>val</VALUE>
	  	  <VALUE>val</VALUE>
	  </LABEL>
</LABELS>*/

#include "CXMLParser.h"

CXMLParser::CXMLParser()
{
	Initialize();
}

void CXMLParser::Initialize()
{
	m_xmlInfo.encode.clear();
	m_xmlInfo.version.clear();
	m_xmlInfo.info_lst.clear();
	m_xmlFile.clear();
	m_nodes = 0;
	m_index = 0;
}

bool CXMLParser::LoadXMLFile(const std::string& file)
{
	Node nod;
	Node new_node;
	PARSED par;
	bool is_new_node = false;

	m_xmlFile.open(file);

	//Test if the file was opened
	if (!m_xmlFile.is_open())
		return false;

	//Read labels from xml file
	while ((par = SearchTag(nod)) != _ERROR)
	{
		//Adds the ROOT node to Tree
		switch (par)
		{
			case _LABEL:
				is_new_node = true;
				new_node.label = nod.label;
			break;
			case _XML_INFO:
				m_xmlInfo.version = nod.list_of_values.at(0).value;
				m_xmlInfo.encode = nod.list_of_values.at(1).value;
			break;
			case _LABEL_VALUE:
				is_new_node = true;
				new_node.label = nod.label;
				for (auto i : nod.list_of_values)
				{
					new_node.list_of_values.push_back(i);
				}
				m_nodes++;
			break;
			case _VALUE:
				for (auto i : nod.list_of_values)
				{
					new_node.list_of_values.push_back(i);
				}
			break;
			case _LABEL_END:
				if (is_new_node)
				{
					m_xmlInfo.info_lst.push_back(new_node);
					new_node.label.clear();
					new_node.list_of_values.clear();
					is_new_node = false;
				}
			break;
			case _END:
				return true;
			break;
			default:
				std::cerr << "ERROR parsing file " << file << "\n";
				m_nodes = 0;
				return false;
			break;
		}
	}

	if (par == _ERROR)
	{
		std::cerr << "ERROR parsing file " << file << "\n";
		m_nodes = 0;
		return false;
	}
	else
		return true;
}

bool CXMLParser::getNodes(const std::string& label)
{
	//Returns true if there is a node with given label
	for (unsigned int i = m_index; i < m_xmlInfo.info_lst.size() ; i++)
	{
		if (!m_xmlInfo.info_lst.at(m_index).label.compare(label))
		{
			return true;
		}

		m_index++;
	}

	return false;
}

int CXMLParser::getNodesCount()
{
	return m_nodes;
}

std::string CXMLParser::getAttributeValue(const std::string& name)
{
	if (m_index < m_xmlInfo.info_lst.size())
	{
		for (auto &i : m_xmlInfo.info_lst.at(m_index).list_of_values)
		{
			if (!i.name.compare(name))
				return i.value;
		}
	}

	return std::string("");
}

void CXMLParser::Next()
{
	m_index++;
}

PARSED CXMLParser::SearchTag(Node &n)
{
	std::string line;
	PARSED readed_tag = _ERROR;

	n.label.clear();
	n.list_of_values.clear();

	if (m_xmlFile.good() && !m_xmlFile.eof())
	{
		//Get lines from m_xmlFile
		std::getline(m_xmlFile, line);

		readed_tag = ParseAnyTag(line, n);
	}

	if (m_xmlFile.eof())
		return _END;
	else
		return readed_tag;
}

PARSED CXMLParser::ParseAnyTag(const std::string& tag, Node &n)
{
	//Line must have format '\t*</)[C]> c+="(C+c+)">' as regular expression
	//I.e. - <TAG>
	//       <TAG name="value" ...> or
	//     - <TAG>value</TAG> or
	//     - </TAG>
	int state = 0;
	std::size_t pos = 0;
	PARSED res = PARSED::_LABEL;
	Pair p;
	std::string end;

	n.label.clear();
	n.list_of_values.clear();

	while (pos < tag.size())
	{
		if (state == 0 && tag.at(pos) == '\t')
			state = 0;
		else if (state == 0 && tag.at(pos) == '<')
			state = 1;
		else if (state == 1 && tag.at(pos) == '?') //xml version and encoding values
		{
			res = PARSED::_XML_INFO;
			state = 17;
		}
		else if (state == 17 && tag.at(pos) == 'x')
		{
			state = 18;
		}
		else if (state == 18 && tag.at(pos) == 'm')
		{
			state = 19;
		}
		else if (state == 19 && tag.at(pos) == 'l')
		{
			state = 20;
		}
		else if (state == 20 && tag.at(pos) == 32) //white space
		{
			state = 6; //Go to read values state
		}
		else if (state == 1 && tag.at(pos) == '/')
		{
			state = 2;
			res = PARSED::_LABEL_END; //The first readed label indicates end of label </LABEL>
		}
		else if (state == 2 && ParseLabelCharacter(tag.at(pos)))
		{
			state = 4;
			n.label = n.label + tag.at(pos);
		}
		else if (state == 4 && ParseLabelCharacter(tag.at(pos)))
		{
			state = 4;
			n.label = n.label + tag.at(pos);
		}
		else if (state == 4 && tag.at(pos) == '>')
		{
			state = 5;
		}
		else if (state == 1 && ParseLabelCharacter(tag.at(pos)))
		{
			state = 3;
			n.label = n.label + tag.at(pos);
			res = PARSED::_LABEL;
		}
		else if (state == 3 && ParseLabelCharacter(tag.at(pos)))
		{
			state = 3;
			n.label = n.label + tag.at(pos);
		}
		else if (state == 3 && tag.at(pos) == '>')
		{
			state = 5;
		}
		else if (state == 3 && tag.at(pos) == 32) // White LABEL VALUE => <LABEL val="value" ...>
		{
			state = 6;
			res = PARSED::_LABEL_VALUE;
		}
		else if (state == 6 && ParseLabelCharacter(tag.at(pos)))
		{
			state = 7;
			p.name = p.name + tag.at(pos);
		}
		else if (state == 7 && ParseLabelCharacter(tag.at(pos)))
		{
			state = 7;
			p.name = p.name + tag.at(pos);
		}
		else if (state == 7 && tag.at(pos) == '=') // Equals
		{
			state = 8;
		}
		else if (state == 8 && tag.at(pos) == 34) // Doble quotes begin
		{
			state = 9;
		}
		else if (state == 9 && ParseValueCharacter(tag.at(pos)))
		{
			state = 10;
			p.value = p.value + tag.at(pos);
		}
		else if (state == 9 && tag.at(pos) == 34) // Doble quotes end value=null
		{
			state = 11;
			n.list_of_values.push_back(p); //Add the pair to the list)
			p.name.clear();
			p.value.clear();
		}
		else if (state == 10 && ParseValueCharacter(tag.at(pos)))
		{
			state = 10;
			p.value = p.value + tag.at(pos);
		}
		else if (state == 10 && tag.at(pos) == 34) // Doble quotes end
		{
			state = 11;
			n.list_of_values.push_back(p); //Add the pair to the list)
			p.name.clear();
			p.value.clear();
		}
		else if (state == 11 && tag.at(pos) == 32) // White => another name/value)
		{
			state = 6;
		}
		else if (state == 11 && tag.at(pos) == '?') // End of initial xml label
		{
			state = 21;
		}
		else if ((state == 11 || state == 21) && tag.at(pos) == '>') // End of label/values
		{
			state = 100;
		}
		else if (state == 5 && ParseValueCharacter(tag.at(pos))) // _VALUE label <VAL>value</VAL>
		{
			state = 12;
			res = PARSED::_VALUE;
			p.value = p.value + tag.at(pos);
		}
		else if (state == 5 && tag.at(pos) == '<')
		{
			state = 13;

			res = PARSED::_VALUE;
			p.name = n.label;
			p.value.clear();

			n.list_of_values.push_back(p);
		}
		else if (state == 12 && ParseValueCharacter(tag.at(pos)))
		{
			state = 12;
			p.value = p.value + tag.at(pos);
		}
		else if (state == 12 && tag.at(pos) == '<') //End of VALUE ...</VAL>
		{
			p.name = n.label;
			n.list_of_values.push_back(p); //Add the pair to the list
			state = 13;
		}
		else if (state == 13 && tag.at(pos) == '/')
		{
			state = 14;
		}
		else if (state == 14 && ParseLabelCharacter(tag.at(pos)))
		{
			state = 15;
			end = end + tag.at(pos);
		}
		else if (state == 15 && ParseLabelCharacter(tag.at(pos)))
		{
			state = 15;
			end = end + tag.at(pos);
		}
		else if (state == 15 && tag.at(pos) == '>') //End of VALUE ...</VAL>
		{
			state = 16;

			//Test if the labels begin-end are differents <LABEL>..</LABEL>
			if (end.compare(n.label))
			{
				res = PARSED::_ERROR;
			}
		}
		else
		{
			res = PARSED::_ERROR;

			break;
		}

		pos++;
	}

	return res;
}

bool CXMLParser::ParseLabelCharacter(const char c)
{
	//return true if character 'c' is a..z,A..Z,0..9 or '_'
	if ( (c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <=57) || c == 95 )
		return true;
	else
		return false;
}

bool CXMLParser::ParseValueCharacter(const char c)
{
	//return true if character 'c' is a..z,A..Z,0..9, '.', ',', '_' or '-'
	if ( (c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <=57) || c == 92 || c == 45 || c == 46 || c == 95 || c == 44)
		return true;
	else
		return false;
}

void CXMLParser::Release()
{
	//Run the tree and delete all
}

void CXMLParser::debug()
{
	int n = 0;

	std::cout << "\nVersion=" << m_xmlInfo.version;
	std::cout << "\nEncode=" << m_xmlInfo.encode;
	std::cout << "\nNodes: " << m_xmlInfo.info_lst.size();
	std::cout << "\n----------";

	for (auto &i : m_xmlInfo.info_lst)
	{
		std::cout << "\n\nNode: " << n;
		std::cout << "\nLabel: " << i.label;

		for (auto &j : i.list_of_values)
		{
			std::cout << "\n ('" << j.name << "', '" << j.value << "')";
		}

		n++;
	}

	std::cout.flush();
}

CXMLParser::~CXMLParser()
{
	Release();
}

