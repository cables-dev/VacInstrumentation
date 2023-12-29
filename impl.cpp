#include "pch.h"
#include "impl.h"

HMODULE VacModule::LoadFile(std::string_view module_name)
{
    return LoadLibraryA(module_name.data());
}

VacModule::VacModule(std::string_view module_path)
    : m_file{ LoadFile(module_path) }
{
}

void* VacModule::GetImagebase()
{
    return (void*)m_file;
}

void* VacModule::GetRunFuncAddr()
{
    return GetProcAddress(m_file, "_runfunc@20");
}

int VacModule::RunFuncBreakpoint(int run_code, void* input_buffer, size_t input_buffer_size, void* output_buffer, size_t* p_output_buffer_size)
{
    auto runfunc_addr = GetRunFuncAddr();
    if (!runfunc_addr)
        return -1;

    using RunFunc_t = int(__stdcall*)(int, void*, size_t, void*, size_t*);
    RunFunc_t runfunc = reinterpret_cast<RunFunc_t>(runfunc_addr);

    DebugBreak();
    return runfunc(run_code, input_buffer, input_buffer_size, output_buffer, p_output_buffer_size);
}

int VacModule::RunFuncWithoutBreakpoint(int run_code, void* input_buffer, size_t input_buffer_size, void* output_buffer, size_t* p_output_buffer_size)
{
    auto runfunc_addr = GetRunFuncAddr();

    if (!runfunc_addr)
        return -1;

    using RunFunc_t = int(__stdcall*)(int, void*, size_t, void*, size_t*);
    RunFunc_t runfunc = reinterpret_cast<RunFunc_t>(runfunc_addr);

    return runfunc(run_code, input_buffer, input_buffer_size, output_buffer, p_output_buffer_size);
}

VacPacket::VacPacket(std::string_view packet_file_path)
{
    std::ifstream read_file{ packet_file_path.data(), std::ifstream::in | std::ifstream::binary };

    read_file.seekg(0, read_file.end); 
    m_packet_size = read_file.tellg();
    read_file.seekg(0, read_file.beg); 

    m_packet = new BYTE[m_packet_size]{};

    read_file.read((char*)m_packet, m_packet_size);
    read_file.close();
}

size_t VacPacket::GetRunCode()
{
    return *reinterpret_cast<size_t*>(m_packet+30);
}

void* VacPacket::GetInputBuffer()
{
    return reinterpret_cast<void*>(m_packet+42);
}

size_t VacPacket::GetInputBufferSize()
{
    return *reinterpret_cast<size_t*>(m_packet+38);
}

size_t VacPacket::GetArg5()
{
    return *reinterpret_cast<size_t*>(GetInputBuffer());
}



