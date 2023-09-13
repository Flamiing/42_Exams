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
	this->spellBook.learnSpell(spell);
}

void Warlock::forgetSpell(std::string spellName)
{
	this->spellBook.forgetSpell(spellName);
}

void Warlock::launchSpell(std::string spellName, const ATarget& target)
{
	ASpell* spell = this->spellBook.createSpell(spellName);
	if (spell)
		spell->launch(target);
}
