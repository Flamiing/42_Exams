#ifndef TARGETGENERATOR_HPP
# define TARGETGENERATOR_HPP

# include <iostream>
# include <map>
# include "ATarget.hpp"

class ATarget;

class TargetGenerator
{
    public:
        TargetGenerator(void);
        ~TargetGenerator(void);
    
        void learnTargetType(ATarget* target);
        void forgetTargetType(const std::string& target);
        ATarget* createTarget(const std::string& target);
    
        std::map<std::string, ATarget*> targets;
    private:
        TargetGenerator(const TargetGenerator& other);
        TargetGenerator& operator=(const TargetGenerator& other);
};

#endif
