#include "stdafx.h"

#include <jd/base/assert.h>
#include <jd/base/lang.h>
#include <jd/base/log.h>

EXTERN_C_BEGIN

bool DoAssert( const char * file, int line, const char * msg )
{
    LOG("ASSERT failed in file %s, line %d:\n(%s)", file, line, msg);
    DEBUG_BREAK();
    return  false;
}

// @TODO add an iPhone popup, so testers can see and report asserts

EXTERN_C_END