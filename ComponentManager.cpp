/*
 * ComponentManager.cpp
 *
 *  Created on: 24 de nov. de 2017
 *      Author: fjimartinez
 */

#include "CText.h"
#include "ComponentManager.h"
#include "GameObjectManager.h"
#include "CTerrain.h"
#include "CMesh.h"
#include "CMd2model.h"
#include "CCubemap.h"
#include "CXMLParser.h"
#include "CRenderingSystem.h"
#include "CTransformComponent.h"
#include "utils.h"
#include "CGameObject.h"

using namespace Engine::Managers;

const std::string ComponentsManagerFile("objetos\\components.xml");

//Get the Mesh Manager instance
ComponentManager& ComponentManager::getComponentManagerInstance()
{
	static ComponentManager cm;

	return cm;
}

ComponentManager::ComponentManager()
{
	//Constructor is empty
}

ComponentManager::~ComponentManager()
{
	//Destructor is empty
}

void ComponentManager::startup()
{
	load(ComponentsManagerFile);
}

void ComponentManager::load(const std::string& file)
{
	std::shared_ptr<CXMLParser> pfile = std::make_shared<CXMLParser>();

	if (pfile->LoadXMLFile(file))
	{
		//Read and create the game components
		while (pfile->getNodes("COMPONENT"))
		{
			//Component to create
			std::shared_ptr<Engine::Component::CRenderingComponent> render_component;
			std::shared_ptr<Engine::Component::CTransformComponent> transform_component;

			//Common values for all components
			std::string name = pfile->getAttributeValue("name");             //Its name
			std::string gameobject = pfile->getAttributeValue("GAMEOBJECT"); //Its Gameobject

			//Values for Rendering components
			std::string type = pfile->getAttributeValue("TYPE");             //Its type (Rendering, Transform, ...)
			std::string mesh_type;
			std::string mesh_file;
			std::string shader;

			//Values for Transform component
			std::string position;
			std::string scale;

			//Read the proper component
			switch (Engine::Component::componentType(type))
			{
				//Rendering Component
				case Engine::Component::COMPONENT_TYPE::RENDERING:
					mesh_type = pfile->getAttributeValue("MESH_TYPE");
					mesh_file = pfile->getAttributeValue("FILE");
					shader = pfile->getAttributeValue("SHADER");

					//Creates the component
					render_component = createRenderingComponent(name,
																mesh_type,
																Engine::System::CRenderingSystem::getRenderingSystem().getShaderIdByName(shader),
																mesh_file);

					if (render_component == nullptr)
						Message::MessageBOX("ComponentManager", "Component not valid '" + name + "'" + " type='" + mesh_type + "'");
					else
					{
						//Attach the render component to its gameobject
						Engine::Managers::GameObjectManager::getGameObjectManagerInstance().AttachComponentToGameObject(render_component, gameobject);

						//Register the Rendering component
						Engine::System::CRenderingSystem::getRenderingSystem().RegisterRenderingComponent(render_component);
					}
				break;

				//Transform Component
				case Engine::Component::COMPONENT_TYPE::TRANSFORM:
					position = pfile->getAttributeValue("POSITION");
					scale = pfile->getAttributeValue("SCALE");

					transform_component = createTransformComponent(name,
																   Engine::Component::stringToVec3(position),
																   Engine::Component::stringToVec3(scale));

					//Engine::System::CTransformSystem::getTransformSystem().RegisterObject(obj);
				break;
				default:
					Message::MessageBOX("ComponentManager", "Type of component not valid: '" + type + "'");
				break;
			}

			pfile->Next();
		}
	}
}

std::shared_ptr<Engine::Component::CTransformComponent> ComponentManager::createTransformComponent(const std::string& name, glm::vec3 position, glm::vec3 scale)
{
	std::shared_ptr<Engine::Component::CTransformComponent> component = std::make_shared<Engine::Component::CTransformComponent>(name);

	component->m_position = position;
	component->m_scale = scale;

	return component;
}

std::shared_ptr<Engine::Component::CRenderingComponent> ComponentManager::createRenderingComponent(const std::string& name, const std::string& typ, GLuint shaderid, const std::string& file)
{
	std::shared_ptr<Engine::Component::CRenderingComponent> component;

	if (!typ.compare(Engine::Component::TEXT_MESH))
	{
		component = std::make_shared<Engine::Graphics::CText>(name, shaderid, file);
		component->setRenderingComponentType(Engine::Component::renderingComponentType::TEXT_MESH);
	}
	else if (!typ.compare(Engine::Component::TERRAIN_MESH))
	{
		component = std::make_shared<Engine::Graphics::CTerrain>(name, shaderid, file);
		component->setRenderingComponentType(Engine::Component::renderingComponentType::TERRAIN_MESH);
	}
	else if (!typ.compare(Engine::Component::BLENDER_MESH))
	{
		component = std::make_shared<Engine::Graphics::CMesh>(name, shaderid, file);
		component->setRenderingComponentType(Engine::Component::renderingComponentType::BLENDER_MESH);
	}

		/*case Engine::Component::renderingComponentType::MD2:
			//Create MD2 Model object
			//object = std::make_shared<Engine::Graphics::CMd2model>(file);
		break;

		case Engine::Component::renderingComponentType::CUBEMAP:
			//Create MD2 Model object
			//object = std::make_shared<Engine::Graphics::CCubemap>(file);
		break;

		default:
			return nullptr;
		break;
	}*/

	return component;
}

void ComponentManager::release()
{
	m_components.clear();
}
