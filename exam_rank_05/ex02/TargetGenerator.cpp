#include "TargetGenerator.hpp"

TargetGenerator::TargetGenerator(void) {}

TargetGenerator::~TargetGenerator(void)
{
	std::map<std::string, ATarget*>::iterator it = this->targets.begin();
	while (it != this->targets.end())
	{
		delete it->second;
		it++;
	}
	this->targets.clear();
}

TargetGenerator::TargetGenerator(const TargetGenerator& other) { (void)other; }
TargetGenerator& TargetGenerator::operator=(const TargetGenerator& other) { (void)other; return *this; }

void TargetGenerator::learnTargetType(ATarget* target)
{
	if (target)
		this->targets[target->getType()] = target->clone();
}

void TargetGenerator::forgetTargetType(const std::string& target)
{
	if (this->targets.find(target) != this->targets.end())
	{
		delete this->targets.find(target)->second;	
		this->targets.erase(target);
	}
}

ATarget* TargetGenerator::createTarget(const std::string& target)
{
	if (this->targets.find(target) != this->targets.end())
		return this->targets[target];
	return NULL;
}
