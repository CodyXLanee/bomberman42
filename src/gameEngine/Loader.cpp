
#include "Loader.hpp"

Loader::Loader() : _jsonPath(""), _state(0)
{
}

Loader::Loader(std::string jsonPath) : _jsonPath(jsonPath), _state(0)
{
}

Loader::~Loader() {}

Loader &			Loader::operator=(Loader const & rhs)
{
	if (this != &rhs)
	{
		this->_jsonPath = rhs.getPath();
		this->_state = rhs.getState();
		this->load();
	}

	return *this;
}

void				Loader::setPath(std::string jsonPath)
{
	this->_jsonPath = jsonPath;
}

std::string			Loader::getPath() const
{
	return this->_jsonPath;
}

rapidjson::Value *	Loader::getValue(std::string key)
{
	if (!this->_document.HasMember(key.c_str()))
	{
		this->_state = -1;
    	return NULL;
	}
	return &(this->_document[key.c_str()]);
}

int					Loader::getState() const
{
	return this->_state;
}

int					Loader::load()
{
    std::string json;
    std::string str;
	std::ifstream infile;
	char * writable;

	this->_state = -1;

	// leave if no mapFile
	if (!this->_jsonPath.compare(""))
		return -1;

    // read the file
	infile.open(this->_jsonPath);
	if (!infile.is_open())
		return -1;
	while (std::getline(infile, str))
		json += str;
	infile.close();

	// parse the json
	writable = new char[json.size() + 1];
	std::copy(json.begin(), json.end(), writable);
	writable[json.size()] = '\0';
    this->_document.Parse(writable);
    delete writable;
    if (!this->_document.IsObject())
    	return -1;

    this->_state = 1;
    return 1;
}