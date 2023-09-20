#ifndef WARLOCK_HPP
# define WARLOCK_HPP

# include <iostream>
# include <map>
# include "ASpell.hpp"
# include "ATarget.hpp"
# include "SpellBook.hpp"

class ASpell;
class ATarget;
class SpellBook;

class Warlock
{
    public:
        Warlock(const std::string& name, const std::string& title);
        ~Warlock(void);
        
        const std::string& getName(void) const;
        const std::string& getTitle(void) const;
        void setTitle(const std::string& title);
        void introduce(void) const;
        void learnSpell(ASpell* spell);
        void forgetSpell(std::string spellName);
        void launchSpell(std::string spellName, const ATarget& target);
    
        SpellBook spellBook;
    private:
        Warlock(void);
        Warlock(const Warlock& other);
        Warlock& operator=(const Warlock& other);
        std::string _name;
        std::string _title;
};

#endif
