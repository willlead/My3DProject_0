#include "Sample.h"
bool Sample::Init()
{	
	m_ParserASE.Open(L"../../data/obj/box.ase");
	return true;

}

Sample::Sample()
{
}


Sample::~Sample()
{
}

TBASIS_RUN(L"AseBox")