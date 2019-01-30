////////////////////////////////////////////////////////////////////////////////
// CMesh.cpp
// Author       : Fernando Jiménez Martínez
//
// Copyright    : (C) 2015 by Fernando Jiménez
// Email        : fernandojim@gmail.com
////////////////////////////////////////////////////////////////////////////////

#include "CMesh.h"

#define SZ_MTLLIB sizeof("mtllib")-1
#define SZ_V      sizeof("v ")-1
#define SZ_VT     sizeof("vt")-1
#define SZ_VN     sizeof("vn")-1
#define SZ_F      sizeof("f ")-1
#define SZ_USEMTL sizeof("usemtl")-1
#define SZ_S      sizeof("s")-1

using namespace Engine::Graphics;

CMesh::CMesh(const std::string& name, GLuint shaderid, const std::string& file) : CRenderingComponent(name, shaderid)
{
	load(file);
}

/*
	Destructor
*/
CMesh::~CMesh()
{

}

/*
	Load submesh from file
*/
bool CMesh::load(const std::string& file)
{
	m_File = std::make_shared<CFileDef>(file);

	/* if exist */
	if ( m_File->getIsOpen() )
	{
		/* Try read the vertex information in order to create the VAO object */
		loadMesh();

		/* Get the size of vector */
		getVAO()->setNumVertex(getVAO()->getvVertex().size());
		getVAO()->setNumIndex(getVAO()->getvVertex().size());

		/* Create VAO with the vertex arrays loaded */
		if (getVAO()->getNumVertex() > 0)
		{
			getVAO()->CreateArrayBuffer(&getVAO()->getvVertex()[0], getVAO()->getNumVertex() * sizeof(float) * 3, GL_STATIC_DRAW);
			getVAO()->CreateAttribArrayBuffer(5, 3);

			if (getVAO()->getvTexel().size() > 0)
			{
				getVAO()->CreateArrayBuffer(&getVAO()->getvTexel()[0], getVAO()->getNumVertex() * sizeof(float) * 2, GL_STATIC_DRAW);
				getVAO()->CreateAttribArrayBuffer(6, 2);
			}
			
			if (getVAO()->getvNormal().size() > 0)
			{
				getVAO()->CreateArrayBuffer(&getVAO()->getvNormal()[0], getVAO()->getNumVertex() * sizeof(float) * 3, GL_STATIC_DRAW);
				getVAO()->CreateAttribArrayBuffer(7, 3);
			}
		}
		else
		{
			return false;
		}
						
		/* load was OK */
		//m_bIsValid = true;
		return true;
	}
		
	return false;
}

/*
	Read the geometric information from the file
*/
void CMesh::loadMesh()
{
	std::string line;
	std::string mtl; //Name of materials library
	std::string mat; //Name of material
	std::string aux;
	GLuint ind = 0;
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> texel;
	std::vector<face> faces;

	/* Clean variables */
	vertex.clear();
	texel.clear();
	normal.clear();
	faces.clear();

	if (m_File->m_Filedef.is_open())
	{
		while (!m_File->m_Filedef.eof())
		{
			// get line from file and stores into buffer
			std::getline(m_File->m_Filedef, line);

			// ignores '#' '\n' '{' or '\0' characters
			if (line.size() != 0 && line.at(0) != '#' && line.at(0) != '\n' && line.at(0) != '\0')
			{
				//Get the materials in the .mtl file associated to object from blender
				if (!line.compare(0, SZ_MTLLIB, "mtllib"))
				{
					//The properties of material will pass to shader
					std::string str(line);
					mtl = str.substr(7, line.length());
					mtl = "objetos\\" + mtl;

					//Stores materials from .mtl file
					//::getMaterialManager().loadMaterialsFromMTL(mtl);
				}
				else if (!line.compare(0, SZ_V, "v ")) //Vertex information
				{
					vertex.push_back(getVec3Info(line));
				}
				else if (!line.compare(0, SZ_VT, "vt")) //Texel information
				{
					texel.push_back(getVec2Info(line));
				}
				else if (!line.compare(0, SZ_VN, "vn")) //Normal information
				{
					normal.push_back(getVec3Info(line));
				}
				else if (!line.compare(0, SZ_F, "f "))  //Face information
				{
 					faces.push_back(getFaceInfo(line));
				}
				else if (!line.compare(0, SZ_USEMTL, "usemtl")) //Texture information
				{
					std::stringstream(line) >> aux >> mat;

					//m_Material = ::getMaterialManager().getMaterialByName(mat);
				}
				else if (!line.compare(0, SZ_S, "s")) // Shader model
				{
					std::stringstream(line) >> aux >> aux;

					if (!aux.compare("off"))
					{
						//m_bSmoothShading = false;
					}
					else
					{
						//m_bSmoothShading = true;
					}
				}
			}
		} //End of while loop

		/* Create the buffers */

		//Buffer for vertexs/texels (if exists)
		for (GLuint i = 0; i < faces.size(); i++)
		{
			for (GLuint j = 0; j < 3; j++)
			{
				//getVAO()->m_Vertex.push_back(vertex.at(faces.at(i).vertex[j] - 1));
				getVAO()->addVertex(vertex.at(faces.at(i).vertex[j] - 1));

				if (texel.size() > 0)
					//getVAO()->m_Texel.push_back(texel.at(faces.at(i).texel[j] - 1));
					getVAO()->addTexel(texel.at(faces.at(i).texel[j] - 1));

				if (normal.size() > 0)
					//getVAO()->m_Normal.push_back(normal.at(faces.at(i).normal[j] - 1));
					getVAO()->addNormal(normal.at(faces.at(i).normal[j] - 1));

				//getVAO()->m_Index.push_back(ind);
				getVAO()->addIndex(ind);
				ind++;
			}

			//If there is a fourth vertex in the face info, it is created a new triangle face
			if (faces.at(i).nverts == 4)
			{
				//getVAO()->m_Vertex.push_back(vertex.at(faces.at(i).vertex[2] - 1));
				getVAO()->addVertex(vertex.at(faces.at(i).vertex[2] - 1));


				if (texel.size() > 0)
					//getVAO()->m_Texel.push_back(texel.at(faces.at(i).texel[2] - 1));
					getVAO()->addTexel(texel.at(faces.at(i).texel[2] - 1));

				if (normal.size() > 0)
					//getVAO()->m_Normal.push_back(normal.at(faces.at(i).normal[2] - 1));
					getVAO()->addNormal(normal.at(faces.at(i).normal[2] - 1));

				//getVAO()->m_Index.push_back(ind);
				getVAO()->addIndex(ind);

				ind++;

				//getVAO()->m_Vertex.push_back(vertex.at(faces.at(i).vertex[3] - 1));
				getVAO()->addVertex(vertex.at(faces.at(i).vertex[3] - 1));

				if (texel.size() > 0)
					//getVAO()->m_Texel.push_back(texel.at(faces.at(i).texel[3] - 1));
					getVAO()->addTexel(texel.at(faces.at(i).texel[3] - 1));

				if (normal.size() > 0)
					//getVAO()->m_Normal.push_back(normal.at(faces.at(i).normal[3] - 1));
					getVAO()->addNormal(normal.at(faces.at(i).normal[3] - 1));

				//getVAO()->m_Index.push_back(ind);
				getVAO()->addIndex(ind);

				ind++;

				//getVAO()->m_Vertex.push_back(vertex.at(faces.at(i).vertex[0] - 1));
				getVAO()->addVertex(vertex.at(faces.at(i).vertex[0] - 1));

				if (texel.size() > 0)
					//getVAO()->m_Texel.push_back(texel.at(faces.at(i).texel[0] - 1));
					getVAO()->addTexel(texel.at(faces.at(i).texel[0] - 1));

				if (normal.size() > 0)
					//getVAO()->m_Normal.push_back(normal.at(faces.at(i).normal[0] - 1));
					getVAO()->addNormal(normal.at(faces.at(i).normal[0] - 1));

				//getVAO()->m_Index.push_back(ind);
				getVAO()->addIndex(ind);

				ind++;
			}
		}

		m_File->m_Filedef.close();
	}
}

/*
	Get a vec3 info from a cad like "v x y z"
*/
glm::vec3 CMesh::getVec3Info(const std::string& cad)
{
	glm::vec3 ret;
	std::string aux[4];

	std::stringstream(cad) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
	ret.x = std::stof(aux[1]);
	ret.y = std::stof(aux[2]);
	ret.z = std::stof(aux[3]);

	return ret;
}

/*
	Get a vec2 info from a cad like "v x y" (for texels)
*/
glm::vec2 CMesh::getVec2Info(const std::string& cad)
{
	glm::vec2 ret;
	std::string aux[3];

	std::stringstream(cad) >> aux[0] >> aux[1] >> aux[2];
	ret.x = std::stof(aux[1]);
	ret.y = std::stof(aux[2]);

	return ret;
}

/*
	Get a face info from a cad like:
	"f 1/1/1 2/2/2 3/3/3 (4/4/4)" -> vertex/texel/normal
	or "f 1//1 2//2 3//3 (4//4)" -> vertex/normal
	or "f 1/1 2/2 3/3 (4/4)  -> vertex/texel
	or "f 1 2 3 (4)" -> vertex
*/
face CMesh::getFaceInfo(const std::string& cad)
{
	face f;
	std::string _cad[5];
	std::string token;
	int j;

	f = {0};

	/* Get the three vertex info */
	std::stringstream(cad) >> _cad[0] >> _cad[1] >> _cad[2] >> _cad[3] >> _cad[4];

	if (_cad[4].length() > 0)
		f.nverts = 4;
	else
		f.nverts = 3;

	for (GLuint i = 0; i <= f.nverts - 1; i++)
	{
		j = 0;
		std::istringstream ss(_cad[i + 1]);
		while (std::getline(ss, token, '/'))
		{
			if (j == 0 && token.size() > 0)
				f.vertex[i] = std::stoi(token);
			else if (j == 1 && token.size() > 0)
				f.texel[i] = std::stoi(token);
			else if (token.size() > 0)
				f.normal[i] = std::stoi(token);

			j++;
		}
	}

	return f;
}
