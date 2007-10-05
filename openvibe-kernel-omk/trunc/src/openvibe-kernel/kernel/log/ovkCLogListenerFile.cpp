#include "ovkCLogListenerFile.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;

#define _do_stuff_on_file_(f, wm, stuff) \
	FILE* f=fopen("openvibe.log", wm); \
	if(f) \
	{ \
		stuff; \
		fclose(f); \
	}

CLogListenerFile::CLogListenerFile(const IKernelContext& rKernelContext)
	:TKernelObject<ILogListener>(rKernelContext)
{
	_do_stuff_on_file_(l_pFile, "wt", );
}

void CLogListenerFile::log(const uint64 ui64Value)
{
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%lld", ui64Value);
	);
}

void CLogListenerFile::log(const uint32 ui32Value)
{
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%ld", ui32Value);
	);
}

void CLogListenerFile::log(const uint16 ui16Value)
{
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%d", ui16Value);
	);
}

void CLogListenerFile::log(const uint8 ui8Value)
{
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%d", ui8Value);
	);
}

void CLogListenerFile::log(const int64 i64Value)
{
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%lld", i64Value);
	);
}

void CLogListenerFile::log(const int32 i32Value)
{
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%ld", i32Value);
	);
}

void CLogListenerFile::log(const int16 i16Value)
{
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%d", i16Value);
	);
}

void CLogListenerFile::log(const int8 i8Value)
{
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%d", i8Value);
	);
}

void CLogListenerFile::log(const float32 f32Value)
{
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%f", f32Value);
	);
}

void CLogListenerFile::log(const float64 f64Value)
{
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%lf", f64Value);
	);
}

void CLogListenerFile::log(const CIdentifier& rValue)
{
	CString l_sValue=rValue.toString();
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%s", (const char*)l_sValue);
	);
}

void CLogListenerFile::log(const CString& rValue)
{
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%s", (const char*)rValue);
	);
}

void CLogListenerFile::log(const char* pValue)
{
	_do_stuff_on_file_(l_pFile, "at",
		fprintf(l_pFile, "%s", pValue);
	);
}

void CLogListenerFile::log(const ELogLevel eLogLevel)
{
	switch(eLogLevel)
	{
		case LogLevel_Debug:
			{ _do_stuff_on_file_(l_pFile, "at", fprintf(l_pFile, "[ DEBUG ] "); ); }
			break;

		case LogLevel_Benchmark:
			{ _do_stuff_on_file_(l_pFile, "at", fprintf(l_pFile, "[ BENCH ] "); ); }
			break;

		case LogLevel_Trace:
			{ _do_stuff_on_file_(l_pFile, "at", fprintf(l_pFile, "[ TRACE ] "); ); }
			break;

		case LogLevel_Info:
			{ _do_stuff_on_file_(l_pFile, "at", fprintf(l_pFile, "[  INF  ] "); ); }
			break;

		case LogLevel_Warning:
			{ _do_stuff_on_file_(l_pFile, "at", fprintf(l_pFile, "[ DEBUG ] "); ); }
			break;

		case LogLevel_Error:
			{ _do_stuff_on_file_(l_pFile, "at", fprintf(l_pFile, "[ ERROR ] "); ); }
			break;

		case LogLevel_Fatal:
			{ _do_stuff_on_file_(l_pFile, "at", fprintf(l_pFile, "[ FATAL ] "); ); }
			break;

		default:
			{ _do_stuff_on_file_(l_pFile, "at", fprintf(l_pFile, "[UNKNOWN] "); ); }
			break;
	}
}

void CLogListenerFile::log(const ELogColor eLogColor)
{
}
