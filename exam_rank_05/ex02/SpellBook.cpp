#include "SpellBook.hpp"

SpellBook::SpellBook(void) {}

SpellBook::~SpellBook(void)
{
	std::map<std::string, ASpell*>::iterator it = this->spells.begin();
	while (it != this->spells.end())
	{
		delete it->second;
		it++;
	}
	this->spells.clear();
}

SpellBook::SpellBook(const SpellBook& other) { (void)other; }
SpellBook& SpellBook::operator=(const SpellBook& other) { (void)other; return *this; }

void SpellBook::learnSpell(ASpell* spell)
{
	if (spell)
		this->spells[spell->getName()] = spell->clone();
}

void SpellBook::forgetSpell(const std::string& spell)
{
	if (this->spells.find(spell) != this->spells.end())
	{
		delete this->spells.find(spell)->second;	
		this->spells.erase(spell);
	}
}

ASpell* SpellBook::createSpell(const std::string& spell)
{
	if (this->spells.find(spell) != this->spells.end())
		return this->spells[spell];
	return NULL;
}
