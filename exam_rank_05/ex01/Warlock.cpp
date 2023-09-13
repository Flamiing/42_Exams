#include "Warlock.hpp"

Warlock::Warlock(void) : _name(""), _title("") {}

Warlock::Warlock(const std::string& name, const std::string& title) : _name(name), _title(title)
{
	std::cout << name << ": This looks like another boring day." << std::endl;
}

Warlock::Warlock(const Warlock& other) : _name(other._name), _title(other._title) {}

Warlock::~Warlock(void)
{
	std::cout << this->_name << ": My job here is done!" << std::endl;
	std::map<std::string, ASpell*>::iterator it = this->spells.begin();
	while (it != this->spells.end())
	{
		delete it->second;
		it++;
	}
	this->spells.clear();
}


Warlock& Warlock::operator=(const Warlock& other)
{
	if (this != &other)
	{
		this->_name = other._name;
		this->_title = other._title;
	}
	return *this;
}

const std::string& Warlock::getName(void) const { return this->_name; }
const std::string& Warlock::getTitle(void) const { return this->_title; }
void Warlock::setTitle(const std::string& title) { this->_title = title; }

void Warlock::introduce(void) const
{
	std::cout << this->getName() << ": I am " << this->getName() << ", " << this->getTitle() << "!" << std::endl;
}

void Warlock::learnSpell(ASpell* spell)
{
	if (spell)
		this->spells[spell->getName()] = spell->clone();
}

void Warlock::forgetSpell(std::string spellName)
{
	if (this->spells.find(spellName) != this->spells.end())
	{
		delete this->spells.find(spellName)->second;
		this->spells.erase(this->spells.find(spellName));
	}
}

void Warlock::launchSpell(std::string spellName, const ATarget& target)
{
	if (this->spells.find(spellName) != this->spells.end())
		this->spells[spellName]->launch(target);
}
