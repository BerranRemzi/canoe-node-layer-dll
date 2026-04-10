#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define USECDLL_FEATURE
#define _BUILDNODELAYERDLL
#include "cdll.h"
#include "VIA.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;

static VIAService *g_via_service = NULL;
static void log_line(const char *fmt, ...);

extern "C" unsigned long CAPLEXPORT CAPLPASCAL SNL_TestAdd(unsigned long a, unsigned long b)
{
    unsigned long result = a + b;
    log_line("SNL_TestAdd(a=%lu, b=%lu) -> %lu", a, b, result);
    return result;
}

static void get_log_path(char *path, size_t path_len)
{
    DWORD n;
    DWORD module_len;
    char *slash;

    if (path == NULL || path_len == 0) {
        return;
    }

    n = GetEnvironmentVariableA("SIMPLE_NODELAYER_LOG", path, (DWORD)path_len);
    if (n > 0 && n < path_len) {
        return;
    }

    module_len = GetModuleFileNameA((HMODULE)&__ImageBase, path, (DWORD)path_len);
    if (module_len > 0 && module_len < path_len) {
        slash = strrchr(path, '\\');
        if (slash != NULL) {
            slash[1] = '\0';
            lstrcpynA(slash + 1, "SimpleNodeLayer.log", (int)(path_len - (size_t)(slash + 1 - path)));
            return;
        }
    }

    n = GetTempPathA((DWORD)path_len, path);
    if (n == 0 || n >= path_len) {
        lstrcpynA(path, "SimpleNodeLayer.log", (int)path_len);
        return;
    }

    lstrcpynA(path + n, "SimpleNodeLayer.log", (int)(path_len - n));
}

static void log_line(const char *fmt, ...)
{
    FILE *f;
    SYSTEMTIME st;
    char log_path[MAX_PATH];
    va_list args;

    get_log_path(log_path, sizeof(log_path));
    f = fopen(log_path, "a");
    if (f == NULL) {
        return;
    }

    GetLocalTime(&st);
    fprintf(
        f,
        "%04u-%02u-%02u %02u:%02u:%02u.%03u [tid=%lu] ",
        st.wYear,
        st.wMonth,
        st.wDay,
        st.wHour,
        st.wMinute,
        st.wSecond,
        st.wMilliseconds,
        (unsigned long)GetCurrentThreadId()
    );

    va_start(args, fmt);
    vfprintf(f, fmt, args);
    va_end(args);

    fputc('\n', f);
    fclose(f);
}

class SimpleModuleApi : public VIAModuleApi
{
public:
    VIASTDDEF Init() override
    {
        log_line("VIAModuleApi::Init()");
        return kVIA_OK;
    }

    VIASTDDEF GetVersion(char *buffer, int32 bufferLength) override
    {
        if (buffer != NULL && bufferLength > 0) {
            lstrcpynA(buffer, "SimpleNodeLayer 1.0", (int)bufferLength);
        }
        log_line("VIAModuleApi::GetVersion(len=%ld)", (long)bufferLength);
        return kVIA_OK;
    }

    VIASTDDEF GetModuleParameters(char *pathBuff, int32 pathBuffLength, char *versionBuff, int32 versionBuffLength) override
    {
        char module_path[MAX_PATH];

        module_path[0] = '\0';
        GetModuleFileNameA((HMODULE)&__ImageBase, module_path, (DWORD)sizeof(module_path));

        if (pathBuff != NULL && pathBuffLength > 0) {
            lstrcpynA(pathBuff, module_path, (int)pathBuffLength);
        }

        if (versionBuff != NULL && versionBuffLength > 0) {
            lstrcpynA(versionBuff, "SimpleNodeLayer 1.0", (int)versionBuffLength);
        }

        log_line("VIAModuleApi::GetModuleParameters()");
        return kVIA_OK;
    }

    VIASTDDEF GetNodeInfo(const char *nodename, char *shortNameBuf, int32 shortBufLength, char *longNameBuf, int32 longBufLength) override
    {
        const char *name = (nodename != NULL) ? nodename : "Node";

        if (shortNameBuf != NULL && shortBufLength > 0) {
            lstrcpynA(shortNameBuf, name, (int)shortBufLength);
        }

        if (longNameBuf != NULL && longBufLength > 0) {
            lstrcpynA(longNameBuf, "SimpleNodeLayer", (int)longBufLength);
        }

        log_line("VIAModuleApi::GetNodeInfo(nodename=%s)", name);
        return kVIA_OK;
    }

    VIASTDDEF CreateObject(VIANodeLayerApi **object, VIANode *node, int32 argc, char *argv[]) override
    {
        (void)node;
        (void)argv;

        if (object != NULL) {
            *object = NULL;
        }

        log_line("VIAModuleApi::CreateObject(argc=%ld) -> kVIA_ObjectUndesired", (long)argc);
        return kVIA_ObjectUndesired;
    }

    VIASTDDEF ReleaseObject(VIANodeLayerApi *object) override
    {
        (void)object;
        log_line("VIAModuleApi::ReleaseObject()", 0);
        return kVIA_OK;
    }

    VIASTDDEF InitMeasurement() override
    {
        log_line("VIAModuleApi::InitMeasurement()");
        return kVIA_OK;
    }

    VIASTDDEF EndMeasurement() override
    {
        log_line("VIAModuleApi::EndMeasurement()");
        return kVIA_OK;
    }

    VIASTDDEF GetNodeInfoEx(VIDBNodeDefinition *nodeDefinition, char *shortNameBuf, int32 shortBufLength, char *longNameBuf, int32 longBufLength) override
    {
        (void)nodeDefinition;

        if (shortNameBuf != NULL && shortBufLength > 0) {
            lstrcpynA(shortNameBuf, "SimpleNL", (int)shortBufLength);
        }

        if (longNameBuf != NULL && longBufLength > 0) {
            lstrcpynA(longNameBuf, "SimpleNodeLayer", (int)longBufLength);
        }

        log_line("VIAModuleApi::GetNodeInfoEx()");
        return kVIA_OK;
    }

    VIASTDDEF DoInformOfChange(VIDBNodeDefinition *nodeDefinition,
        const uint32 changeFlags,
        const char *simBusName,
        const VIABusInterfaceType busType,
        const VIAChannel channel,
        const char *oldName,
        const char *newName,
        const int32 bValue) override
    {
        (void)nodeDefinition;
        (void)busType;
        (void)channel;
        (void)bValue;
        log_line(
            "VIAModuleApi::DoInformOfChange(flags=0x%08lX, bus=%s, old=%s, new=%s)",
            (unsigned long)changeFlags,
            (simBusName != NULL) ? simBusName : "",
            (oldName != NULL) ? oldName : "",
            (newName != NULL) ? newName : ""
        );
        return kVIA_OK;
    }
};

static SimpleModuleApi g_module_api;

static CAPL_DLL_INFO4 g_capl_table4[] = {
    { CDLL_VERSION_NAME, (CAPL_FARCALL)CDLL_VERSION, "", "", CAPL_DLL_CDECL, 0xABCD, CDLL_EXPORT, "", {0} },
    { "SNL_TestAdd", (CAPL_FARCALL)SNL_TestAdd, "CAPL_DLL", "Returns a+b for NodeLayer smoke test", 'L', 2, "LL", "\x0\x0", { "a", "b" } },
    { 0 }
};

extern "C" CAPLEXPORT CAPL_DLL_INFO2 *caplDllTable2 = NULL;
extern "C" CAPLEXPORT CAPL_DLL_INFO3 *caplDllTable3 = NULL;
extern "C" CAPLEXPORT CAPL_DLL_INFO4 *caplDllTable4 = g_capl_table4;

extern "C" VIACLIENT(void) VIARequiredVersion(int32 *majorversion, int32 *minorversion)
{
    if (majorversion != NULL) {
        *majorversion = VIAMajorVersion;
    }
    if (minorversion != NULL) {
        *minorversion = 29;
    }
    log_line("VIARequiredVersion(major=%p, minor=%p)", majorversion, minorversion);
}

extern "C" VIACLIENT(void) VIADesiredVersion(int32 *majorversion, int32 *minorversion)
{
    if (majorversion != NULL) {
        *majorversion = VIAMajorVersion;
    }
    if (minorversion != NULL) {
        *minorversion = VIAMinorVersion;
    }
    log_line("VIADesiredVersion(major=%p, minor=%p)", majorversion, minorversion);
}

extern "C" VIACLIENT(void) VIASetService(VIAService *service)
{
    g_via_service = service;
    log_line("VIASetService(service=%p)", service);
}

extern "C" VIACLIENT(VIAModuleApi *) VIAGetModuleApi(int32 argc, char *argv[])
{
    int i;

    log_line("VIAGetModuleApi(argc=%ld, argv=%p, service=%p)", (long)argc, argv, g_via_service);
    for (i = 0; i < argc && i < 8; ++i) {
        const char *arg = (argv != NULL && argv[i] != NULL) ? argv[i] : "";
        log_line("  argv[%d]=%s", i, arg);
    }

    return &g_module_api;
}

extern "C" VIACLIENT(void) VIAReleaseModuleApi(VIAModuleApi *api)
{
    log_line("VIAReleaseModuleApi(api=%p)", api);
}

extern "C" CAPLEXPORT CAPL_DLL_INFO2 *CAPLCDECL caplDllGetTable2(void)
{
    log_line("caplDllGetTable2()");
    return NULL;
}

extern "C" CAPLEXPORT CAPL_DLL_INFO3 *CAPLCDECL caplDllGetTable3(void)
{
    log_line("caplDllGetTable3()");
    return NULL;
}

extern "C" CAPLEXPORT CAPL_DLL_INFO4 *CAPLCDECL caplDllGetTable4(void)
{
    log_line("caplDllGetTable4()");
    return g_capl_table4;
}

extern "C" VIACLIENT(int32) NLGetModuleOptions(int32 option)
{
    if (option == kVIA_GetModuleOption_DLLType) {
        log_line("NLGetModuleOptions(DLLType) -> StandardNodeLayer");
        return kVIA_ModuleOption_DLLType_StandardNodeLayer;
    }
    if (option == kVIA_ModuleOption_LoadOption) {
        log_line("NLGetModuleOptions(LoadOption) -> Default");
        return kVIA_ModuleOption_LoadOption_Default;
    }
    if (option == kVIA_GetModuleOption_CAPLonBoard) {
        log_line("NLGetModuleOptions(CAPLonBoard) -> NotSupported");
        return kVIA_ModuleOption_CAPLonBoard_NotSupported;
    }

    log_line("NLGetModuleOptions(option=%ld) -> 0", (long)option);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved)
{
    (void)hInst;
    (void)reserved;

    if (reason == DLL_PROCESS_ATTACH) {
        log_line("DllMain: DLL_PROCESS_ATTACH");
    } else if (reason == DLL_PROCESS_DETACH) {
        log_line("DllMain: DLL_PROCESS_DETACH");
        g_via_service = NULL;
    }

    return TRUE;
}
