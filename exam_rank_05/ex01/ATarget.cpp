#include "ATarget.hpp"

ATarget::ATarget(void) : _type("") {}
ATarget::ATarget(const std::string& type) : _type(type) {}
ATarget::ATarget(const ATarget& other) : _type(other._type) {}
ATarget::~ATarget(void) {}


ATarget& ATarget::operator=(const ATarget& other)
{
	if (this != &other)
		this->_type = other._type;
	return *this;
}

const std::string& ATarget::getType(void) const { return this->_type; }

void ATarget::getHitBySpell(const ASpell& spell) const
{
	std::cout << this->getType() << " has been " << spell.getEffects() << "!" << std::endl;
}
