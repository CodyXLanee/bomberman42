
#ifndef LOADER_HPP
# define LOADER_HPP

# include "rapidjson/document.h"
# include "rapidjson/stringbuffer.h"
# include "rapidjson/writer.h"
# include "Player.hpp"
# include "includes/IGameEntity.hpp"
# include <iostream>
# include <fstream>
# include <vector>
# include <glm/gtc/matrix_transform.hpp>

class Loader {
	public:
		Loader();
		Loader(std::string jsonPath);
		Loader(Loader const & src);

		~Loader();

		Loader &			operator=(Loader const & rhs);

		void				setPath(std::string jsonPath);
		std::string			getPath() const;
		rapidjson::Value *	getValue(std::string key);
		int					getState() const;

		int					load();

	private:
		std::string				_jsonPath;
		rapidjson::Document		_document;
		int						_state;
};

#endif