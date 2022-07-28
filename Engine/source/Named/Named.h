#pragma once

#include "KrimzLib.h"


namespace Krimz
{
	class Named
	{
	public:
		enum class Type
		{
			Entity = 0,
			Mesh,
			Texture,
			Material,
			LAST_IGNORED
		};

		Named(Named::Type type, const std::string& name = "undefined");
		Named(const Krimz::Named&) = delete;
		void operator=(const Krimz::Named&) = delete;
		virtual ~Named();

		const std::string& getName() const;
		void updateName(const std::string& newName);

	private:
		static std::set<std::string> m_ExistingNames[int(Named::Type::LAST_IGNORED)];
		std::string m_Name;
		Type m_Type;
	};
}
