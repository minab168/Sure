#pragma once


#ifdef __cplusplus
    #define NULL_PTR nullptr
#else
    #define NULL_PTR ((void*)0)
#endif


#define let const auto
#define mut auto


#define loop while(true)

#define comma_(a_, b_)          a_, b_
#define max_(val1_, val2_)      ((val1_) > (val2_) ? (val1_) : (val2_))
#define min_(val1_, val2_)      ((val1_) < (val2_) ? (val1_) : (val2_))
#define array_len_(array_name_) (sizeof(array_name_) / sizeof(*array_name_))


#if IS_CPP_ 
    #define CONSTEXPR_ constexpr
#else 
    #define CONSTEXPR_
#endif