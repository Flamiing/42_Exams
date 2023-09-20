#include "Warlock.hpp"

Warlock::Warlock(const std::string& name, const std::string& title) : _name(name), _title(title) { std::cout << this->getName() << ": This looks like another boring day." << std::endl; }
Warlock::~Warlock(void)
{
    std::cout << this->getName() << ": My job here is done!" << std::endl;
    std::map<std::string, ASpell*>::iterator it = this->spells.begin();
    while (it != this->spells.end())
    {
        delete it->second;
        it++;
    }
    this->spells.clear();
}

Warlock::Warlock(void) {}
Warlock::Warlock(const Warlock& other) { (void)other; }
Warlock& Warlock::operator=(const Warlock& other) { (void)other; return *this; }

const std::string& Warlock::getName(void) const { return this->_name; }
const std::string& Warlock::getTitle(void) const { return this->_title; }
void Warlock::setTitle(const std::string& title) { this->_title = title; }
void Warlock::introduce(void) const { std::cout << this->getName() << ": I am " << this->getName() << ", " << this->getTitle() << "!" << std::endl; }

void Warlock::learnSpell(ASpell* spell)
{
    if (spell)
        this->spells[spell->getName()] = spell->clone();
}

void Warlock::forgetSpell(std::string spellName)
{
    if (this->spells.find(spellName) != this->spells.end())
    {
        delete this->spells[spellName];
        this->spells.erase(this->spells.find(spellName));
    }
}


void Warlock::launchSpell(std::string spellName, const ATarget& target)
{
    if (this->spells.find(spellName) != this->spells.end())
        this->spells[spellName]->launch(target);
}
