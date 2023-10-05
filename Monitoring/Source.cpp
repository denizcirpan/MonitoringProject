#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <string>
#include <fstream>
#include <wbemidl.h>
#include <comdef.h>

class SystemInfo {
public:

    //Sistemin HDD'sindeki boþ alaný GB cinsinden döndüren fonksiyon 
    double GetHDDFreeSpaceGB(const std::wstring& drive) {
        ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

        if (GetDiskFreeSpaceEx(drive.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            return static_cast<double>(freeBytesAvailable.QuadPart) / (1024 * 1024 * 1024); //GB'a döndürür
        }

        return -1.0; 
    }

    //sistemin HDD'sindeki bütün alaný GB cinsinden döndüren fonksiyon 
    double GetHDDFullSpaceGB(const std::wstring& drive) {
        ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

        if (GetDiskFreeSpaceEx(drive.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            return static_cast<double>(totalNumberOfBytes.QuadPart) / (1024 * 1024 * 1024); //GB'a döndürür
        }

        return -1.0; 
    }

    //sistemin CPU'sundaki sýcaklýðýný derece cinsinden döndüren fonksiyon 
    double GetCPUTemperature() {
        std::string line;
        double temperature = -1.0;

        std::ifstream temperatureFile("/sys/class/thermal/thermal_zone0/temp"); // Linux için örnek yolu kullanýyoruz
        if (temperatureFile.is_open()) {
            getline(temperatureFile, line);
            temperature = std::stod(line) / 1000.0; //Dereceye döndürür
            temperatureFile.close();
        }

        return temperature;
    }

    //Sistemde CPU'nun yüzde kaç kullandýðýný döndüren fonksiyon
    double GetCPUUsage() {

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
            return -1.0; 
        }
    
    }

    // Sistemdeki RAM kullanýmýný yüzde olarak döndüren fonksiyon
    double GetRAMUsage() {
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);

        return static_cast<double>(statex.dwMemoryLoad); 
    }

    // Sistemde bulunan Toplam Ram boyutunu GB cinsinden döndüren fonksiyon
     int  GetRAMTotal() {

       MEMORYSTATUSEX statex1;
       statex1.dwLength = sizeof(statex1);
       GlobalMemoryStatusEx(&statex1);

       return statex1.ullTotalPhys / (1024 * 1024 * 1024); 
       
    }

     // Sistemde boþta olan Ram boyutunu GB cinsinden döndüren fonksiyon
     int  GetRAMEmpty() {

         MEMORYSTATUSEX statex2;
         statex2.dwLength = sizeof(statex2);
         GlobalMemoryStatusEx(&statex2);

         return statex2.ullAvailPhys / (1024 * 1024 * 1024); 
         
     }

 };



    int main() {

        SystemInfo systemInfo;
        
        while (true) {

            //veri aktarýmý için kullanýlan txt dosyasý
            std::ofstream outputFile("output.txt");


            //cpu 
            //CPU için oluþturulan fonskiyonlarýn kullanýlmasý ve bir deðiþkene atanmasý
            int cpuUsage = systemInfo.GetCPUUsage();


            //ram
            //RAM için oluþturulan fonskiyonlarýn kullanýlmasý ve bir deðiþkene atanmasý
            double ramUsage = systemInfo.GetRAMUsage();
            int ramTotal = systemInfo.GetRAMTotal();
            int ramEmpty = systemInfo.GetRAMEmpty();

            //hdd
            //HDD için oluþturulan fonskiyonlarýn kullanýlmasý ve bir deðiþkene atanmasý
            int hddFreeSpace = systemInfo.GetHDDFreeSpaceGB(L"C:");
            int hddFullSpace = systemInfo.GetHDDFullSpaceGB(L"C:");


            //sistemden alýnan deðerlerin dosyaya yazýlma iþlemi 
            if (outputFile.is_open()) {
                
                outputFile << cpuUsage << ",";
                outputFile << ramUsage << ",";
                outputFile << ramTotal << ",";
                outputFile << ramEmpty << ",";
                outputFile << hddFreeSpace << ",";
                outputFile << hddFullSpace << ",";
               

                outputFile.close();

                std::cout << "Variables written to output.txt" << std::endl;
            }
            else {
                std::cerr << "Unable to open the file for writing!" << std::endl;
            }

            Sleep(30000); // 30 saniyede bir veri al

        }
            return 0;

    }
         

         //verilerin pipe kullanýlarak aktarýlmasý denendi

          /*Named Pipe oluþturma
            HANDLE pipe = CreateNamedPipe(
                L"\\\\.\\pipe\\MyPipe",          // Pipe adý
                PIPE_ACCESS_DUPLEX,           // Yalnýzca yazma izni
                PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                1,                               // Maksimum istemci sayýsý
                0, 0, 0, NULL
              

            );
            std::cout << "pipe olustu " << std::endl;

             if (pipe == INVALID_HANDLE_VALUE) {
                            std::cerr << "Named Pipe oluþturma hatasý!" << std::endl;
                            return 1;
                        }

            HANDLE pipe2 = CreateFile(
                L"\\\\.\\pipe\\MyPipe",  // Pipe adý
                GENERIC_WRITE,           // Yalnýzca yazma izni
                0, NULL,
                OPEN_EXISTING,           // Varolan bir pipe'i açma
                0, NULL
            );
 
            if (pipe2 != INVALID_HANDLE_VALUE) {
                // Baðlantý baþarýlý

                // Veri yazma
                std::string data = "Merhaba, Named Pipe!";
                DWORD bytesWritten = 0;
                if (WriteFile(pipe2, data.c_str(), data.size(), &bytesWritten, NULL)) {
                    std::cout << "Veri yazildi: " << bytesWritten << " byte" << std::endl;
                }
                else {
                    std::cerr << "Veri yazma hatasi!" << std::endl;
                }

                // Pipe'i kapatma
                CloseHandle(pipe2);
            }
            else {
                std::cerr << "Baðlantý hatasý!" << std::endl;
    
            // Named Pipe'e baðlanma
        if (ConnectNamedPipe(pipe, NULL) != FALSE) {
            std::cout << "deneme0 " << std::endl;
            // Sistem bilgilerini hazýrla
            std::string systemInfo1 = "CPU Kullanimi: ";
            std::cout << "deneme1 " << std::endl;

                // Sistem bilgilerini Named Pipe üzerinden gönder
            WriteFile(pipe, systemInfo1.c_str(),NULL, NULL, NULL);
            std::cout << "deneme2 " << std::endl;

                // Pipe'i kapatma
            CloseHandle(pipe);
            std::cout << "deneme3 " << std::endl;
            return 0;
            std::cout << "deneme4 " << std::endl;
        }*/  

     
    
