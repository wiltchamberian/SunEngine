#ifndef __METEOREXCEPTION_H
#define __METEOREXCEPTION_H

#define METEOR_ASSERT_FALSE 

namespace Sun {
class SException
{
public:
    SException();
    ~SException();

public:
    void assert_false();
};

}

#endif

