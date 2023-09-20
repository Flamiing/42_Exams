#include "SpellBook.hpp"

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

SpellBook::SpellBook(void) {}
SpellBook::SpellBook(const SpellBook& other) { (void)other; }
SpellBook& SpellBook::operator=(const SpellBook& other) { (void)other; return *this; }

void SpellBook::learnSpell(ASpell* spell)
{
    if (spell)
        this->spells[spell->getName()] = spell->clone();
}

void SpellBook::forgetSpell(const std::string& spellName)
{
    if (this->spells.find(spellName) != this->spells.end())
    {
        delete this->spells[spellName];
        this->spells.erase(this->spells.find(spellName));
    }
}

ASpell* SpellBook::createSpell(const std::string& spellName)
{
    if (this->spells.find(spellName) != this->spells.end())
        return this->spells[spellName];
    return NULL;
}
