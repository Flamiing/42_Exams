#include "ASpell.hpp"

ASpell::ASpell(const std::string name, const std::string effects) : _name(name), _effects(effects) {}
ASpell::~ASpell(void) {}
ASpell::ASpell(void) {}
ASpell::ASpell(const ASpell& other) : _name(other._name), _effects(other._effects) {}
ASpell& ASpell::operator=(const ASpell& other)
{
    if (this != &other)
    {
        this->_name = other._name;
        this->_effects = other._effects;
    }
    return *this;
}

std::string ASpell::getName(void) const { return this->_name; }
std::string ASpell::getEffects(void) const { return this->_effects; }
void ASpell::launch(const ATarget& target) const
{
    target.getHitBySpell(*this);
}
