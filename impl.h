#pragma once


class VacModule
{
private:
    HMODULE m_file{};

    HMODULE LoadFile(std::string_view module_name);
public:
    VacModule(std::string_view module_path);

    void* GetImagebase();
    void* GetRunFuncAddr();

    int RunFuncBreakpoint(
        int run_code,
        void* input_buffer,
        size_t input_buffer_size,
        void* output_buffer,
        size_t* p_output_buffer_size
    );

    int RunFuncWithoutBreakpoint(
        int run_code,
        void* input_buffer,
        size_t input_buffer_size,
        void* output_buffer,
        size_t* p_output_buffer_size
    );
};

class VacPacket
{
private:
    size_t m_packet_size{};
    BYTE* m_packet{};

//  void ParseFile(std::string_view packet_file_path);
public:
    VacPacket(std::string_view packet_file_path);

    size_t GetRunCode();
    void* GetInputBuffer();
    size_t GetInputBufferSize();
    size_t GetArg5();
};

