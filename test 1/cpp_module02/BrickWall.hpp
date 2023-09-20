#ifndef BRICKWALL_HPP
# define BRICKWALL_HPP

# include <iostream>
# include "ATarget.hpp"

class ATarget;

class BrickWall : public ATarget
{
    public:
        BrickWall(void);
        ~BrickWall(void);
        ATarget* clone(void) const;
};

#endif
