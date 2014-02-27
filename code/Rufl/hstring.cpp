////////////////////////////////////////////////////////////////////////////////////////
// RAVEN STANDARD USEFUL FUNCTION LIBRARY
//  (c) 2002 Activision
//
//
// Handle String
// -------------
// Handle strings are allocated once in a static buffer (with a hash index), and are 
// never cleared out.  You should use these for very common string names which are
// redundant or intended to last a long time.
//
// Handle strings are also good for comparison and storage because they compare only
// the handles, which are simple unique integers.
//
////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////
#include "hstring.h"
#include <string.h>
#include "../Ratl/hash_pool_vs.h"



////////////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////////////
#define MAX_HASH		16384			// How Many Hash
#define	BLOCK_SIZE		65536			// Size of a string storage block in bytes.







////////////////////////////////////////////////////////////////////////////////////////
// The Hash Pool
////////////////////////////////////////////////////////////////////////////////////////
typedef	ratl::hash_pool<BLOCK_SIZE, MAX_HASH>		TStrPool;


TStrPool&	PoolStr()
{
	static TStrPool TSP;
	return TSP;
}




#ifdef _XBOX
namespace dllNamespace
{
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////////////
hstring::hstring()
{
	mHandle	= 0;
#ifdef _DEBUG
	mStr	= 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////////////
hstring::hstring(const char *str)
{
	init(str);
}

////////////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////////////
hstring::hstring(const hstring &str) 
{
	mHandle = str.mHandle;

#ifdef _DEBUG
	mStr	= str.mStr;
#endif
}


////////////////////////////////////////////////////////////////////////////////////////
// Assignment
////////////////////////////////////////////////////////////////////////////////////////
hstring& hstring::operator= (const char *str)
{
	init(str);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////////////
hstring& hstring::operator= (const hstring &str)
{
	mHandle = str.mHandle;

#ifdef _DEBUG
	mStr	= str.mStr;
#endif
	return *this;
}


////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
const char*	hstring::c_str(void) const
{
	if (!mHandle)
	{
		return("");
	}
	return ((const char*)PoolStr()[mHandle]);
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
const char*	hstring::operator *(void) const
{
	return c_str();
}

////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////
int			hstring::length() const
{
	return strlen(c_str());
}

////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////
int			hstring::handle() const
{
	return mHandle;
}



////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////
void		hstring::init(const char *str)
{
	if (!str)
	{
		mHandle = 0;
	}
	else
	{
		mHandle = PoolStr().get_handle(str, strlen(str)+1);		// +1 for null character
	}
	#ifdef _DEBUG
	mStr	= (char*)PoolStr()[mHandle];
	#endif
}


////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
float		hstring::ave_collisions()	{return PoolStr().average_collisions();}
int			hstring::total_strings()	{return PoolStr().total_allocs();}
int			hstring::total_bytes()		{return PoolStr().size();}
int			hstring::total_finds()		{return PoolStr().total_finds();}
int			hstring::total_collisions()	{return PoolStr().total_collisions();}
#endif

#ifdef _XBOX
} // dllNamespace
#endif
