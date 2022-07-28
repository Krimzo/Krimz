#include "Named/Named.h"


std::set<std::string> Krimz::Named::m_ExistingNames[] = {};

Krimz::Named::Named(Named::Type type, const std::string& name) : m_Type(type)
{
	updateName(name);
}
Krimz::Named::~Named()
{
	m_ExistingNames[uint(m_Type)].erase(m_Name);
}

const std::string& Krimz::Named::getName() const
{
	return m_Name;
}
void Krimz::Named::updateName(const std::string& newName)
{
	std::string candName = newName;
	uint64 nameCounter = 0;
	m_ExistingNames[uint(m_Type)].erase(m_Name);
	while (m_ExistingNames[uint(m_Type)].contains(candName))
	{
		candName = newName + std::to_string(++nameCounter);
	}
	m_ExistingNames[uint(m_Type)].insert(candName);
	m_Name = candName;
}
