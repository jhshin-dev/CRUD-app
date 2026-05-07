#pragma once

#ifdef JSON_STATIC
    #define JSON_API
#elif defined(JSON_EXPORTS)
    #define JSON_API __declspec(dllexport)
#else
    #define JSON_API __declspec(dllimport)
#endif
