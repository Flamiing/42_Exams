#ifndef SPELLBOOK_HPP
# define SPELLBOOK_HPP

# include <iostream>
# include <map>
# include "ASpell.hpp"
# include "ATarget.hpp"

class ASpell;
class ATarget;

class SpellBook
{
    public:
        SpellBook(void);
        ~SpellBook(void);
        
        void learnSpell(ASpell* spell);
        void forgetSpell(const std::string& spellName);
        ASpell* createSpell(const std::string& spellName);
    
        std::map<std::string, ASpell*> spells;
    private:
        SpellBook(const SpellBook& other);
        SpellBook& operator=(const SpellBook& other);
};

#endif
