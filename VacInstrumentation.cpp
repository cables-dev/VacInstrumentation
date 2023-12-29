#include "pch.h"
#include "impl.h"

int main()
{
    //example program:
    std::cout << std::hex;
    VacModule vac_mod{ R"(C:\Users\anspy\Desktop\joe budden\648B.dll)" };
    VacPacket test_packet{ R"(C:\Users\anspy\Desktop\joe budden\Vac_Packet_0_1.vacpak)" };

    std::cout << "Vac module loaded at address: " << vac_mod.GetImagebase() << '\n';
    std::cout << "Vac runfunc at address: " << vac_mod.GetRunFuncAddr() << '\n';

    //wait for input
    system("pause");

	auto output_packet = new BYTE[0x10000];
	size_t output_packet_size = test_packet.GetArg5();

	vac_mod.RunFuncWithoutBreakpoint(test_packet.GetRunCode(), test_packet.GetInputBuffer() ,test_packet.GetInputBufferSize(), output_packet, &output_packet_size );

    return 0;
}

