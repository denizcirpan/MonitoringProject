#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <string>
#include <fstream>
#include <wbemidl.h>
#include <comdef.h>

class SystemInfo {
public:
    double GetHDDFreeSpaceGB(const std::wstring& drive) {
        ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

        if (GetDiskFreeSpaceEx(drive.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            return static_cast<double>(freeBytesAvailable.QuadPart) / (1024 * 1024 * 1024); // GB olarak döndür
        }

        return -1.0; // Hata durumu
    }

    double GetCPUTemperature() {
        std::string line;
        double temperature = -1.0;

        std::ifstream temperatureFile("/sys/class/thermal/thermal_zone0/temp"); // Linux için örnek yolu kullanýyoruz
        if (temperatureFile.is_open()) {
            getline(temperatureFile, line);
            temperature = std::stod(line) / 1000.0; // Sýcaklýðý derece cinsinden döndür
            temperatureFile.close();
        }

        return temperature;
    }

    double GetCPUUsage() {
    // CPU kullanýmýný almak için iþletim sistemi API'lerini kullanabiliriz.
    // Bu örnek, Windows için iþlemci kullanýmýný almak için GetSystemTimes iþlevini kullanýr.

        FILETIME idleTime, kernelTime, userTime;

        if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
            ULONGLONG sysTime = static_cast<ULONGLONG>(kernelTime.dwHighDateTime) << 32 | kernelTime.dwLowDateTime;
            ULONGLONG usrTime = static_cast<ULONGLONG>(userTime.dwHighDateTime) << 32 | userTime.dwLowDateTime;
            ULONGLONG idleTimeVal = static_cast<ULONGLONG>(idleTime.dwHighDateTime) << 32 | idleTime.dwLowDateTime;

            ULONGLONG totalTime = sysTime + usrTime;
            double usagePercent = (1.0 - (static_cast<double>(idleTimeVal) / totalTime)) * 1000.0;

            return usagePercent;
        }
        else {
            return -1.0; // Hata durumu
        }
    
    }

    double GetRAMUsage() {
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);

        return static_cast<double>(statex.dwMemoryLoad); // RAM kullanýmýný döndür
    }
};

    int main() {
        SystemInfo systemInfo;
        int count = 0;


        while (true) {

            std::ofstream outputFile("output.txt");

           

            int cpuUsage = systemInfo.GetCPUUsage();
            double ramUsage = systemInfo.GetRAMUsage();
            int hddFreeSpace = systemInfo.GetHDDFreeSpaceGB(L"C:");

            //double cpuTemperature = systemInfo.GetCPUTemperature();

            //  Verileri görselleþtir
            // std::cout << "CPU Kullanimi: " << cpuUsage << "%" << std::endl;
            // std::cout << "RAM Kullanimi: " << ramUsage << "%" << std::endl;
            // std::cout << "HDD Bos Alan (GB): " << hddFreeSpace << "GB" << std::endl;
            //std::cout << "------------------------------- " << std::endl;
            // std::cout << "CPU Sýcaklýðý (Celsius): " << cpuTemperature << std::endl;
           

            // Check if the file is opened successfully
            if (outputFile.is_open()) {
                // Write the variables to the file
                outputFile << cpuUsage << ",";
                outputFile << ramUsage << ",";
                outputFile << hddFreeSpace << ",";
                
                std::cout << "count" << count << std::endl;
                count++;

                // Close the file
                outputFile.close();

                std::cout << "Variables written to output.txt" << std::endl;
            }
            else {
                std::cerr << "Unable to open the file for writing!" << std::endl;
            }

            Sleep(10000); // 30 saniyede bir veri al

        }
        //}
        // 
        // 
         // Named Pipe oluþturma
            //HANDLE pipe = CreateNamedPipe(
            //    L"\\\\.\\pipe\\MyPipe",          // Pipe adý
            //    PIPE_ACCESS_DUPLEX,           // Yalnýzca yazma izni
            //    PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            //    1,                               // Maksimum istemci sayýsý
            //    0, 0, 0, NULL
            //  

            //);
            //std::cout << "pipe olustu " << std::endl;

            // if (pipe == INVALID_HANDLE_VALUE) {
            //                std::cerr << "Named Pipe oluþturma hatasý!" << std::endl;
            //                return 1;
            //            }


            //HANDLE pipe2 = CreateFile(
            //    L"\\\\.\\pipe\\MyPipe",  // Pipe adý
            //    GENERIC_WRITE,           // Yalnýzca yazma izni
            //    0, NULL,
            //    OPEN_EXISTING,           // Varolan bir pipe'i açma
            //    0, NULL
            //);

           


            //if (pipe2 != INVALID_HANDLE_VALUE) {
            //    // Baðlantý baþarýlý

            //    // Veri yazma
            //    std::string data = "Merhaba, Named Pipe!";
            //    DWORD bytesWritten = 0;
            //    if (WriteFile(pipe2, data.c_str(), data.size(), &bytesWritten, NULL)) {
            //        std::cout << "Veri yazildi: " << bytesWritten << " byte" << std::endl;
            //    }
            //    else {
            //        std::cerr << "Veri yazma hatasi!" << std::endl;
            //    }

            //    // Pipe'i kapatma
            //    CloseHandle(pipe2);
            //}
            //else {
            //    std::cerr << "Baðlantý hatasý!" << std::endl;


      
           
            
        //// Named Pipe'e baðlanma
        //if (ConnectNamedPipe(pipe, NULL) != FALSE) {
        //    std::cout << "deneme0 " << std::endl;
        //    // Sistem bilgilerini hazýrla
        //    std::string systemInfo1 = "CPU Kullanimi: ";
        //    std::cout << "deneme1 " << std::endl;

        //    // Sistem bilgilerini Named Pipe üzerinden gönder
        //    WriteFile(pipe, systemInfo1.c_str(),NULL, NULL, NULL);
        //    std::cout << "deneme2 " << std::endl;

        //    // Pipe'i kapatma
        //    CloseHandle(pipe);
        //    std::cout << "deneme3 " << std::endl;
        //    return 0;
        //    std::cout << "deneme4 " << std::endl;
        //}


           return 0;

      }
    
